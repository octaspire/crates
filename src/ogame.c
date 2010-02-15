/*
  Crates ‐ extensible 3D puzzle game.
  Copyright (C) 2008‐2010  Octaspire (www.octaspire.com)

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "ogame.h"
#include "odir.h"
#include "oevent.h"
#include "oluacapi.h"
#include "oglapi.h"
#include "oversion.h"
#include <math.h>

#ifdef OUSEGLHEADERSDIRECTLY
#include <GL/gl.h>
#include <GL/glu.h>
#else
#include "SDL_opengl.h"
#endif

static GLfloat ogame_lightambient[]  = {0.5f, 0.5f, 0.5f, 1.0f};
static GLfloat ogame_lightdiffuse[]  = {1.0f, 1.0f, 1.0f, 1.0f};
static GLfloat ogame_lightposition[] = {0.0, 0.0, 1.0, 1.0};

static oint32 resizeWindow(oint32 width, oint32 height)
{
  GLfloat ratio;

  if (height == 0)
    height = 1;

  ratio = (GLfloat)width / (GLfloat)height;
  glViewport(0, 0, (GLint)width, (GLint)height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f, ratio, 0.1f, 100.0f);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  return 1;
}

static oboolean ogame_sdlinit(OGame *game)
{
  oint32 videoFlags;
  oint32 ctrlidx = 0;
  oint32 i;
  const SDL_VideoInfo *videoInfo;
  SDL_Rect **modes;
  ouint32 flags = SDL_INIT_VIDEO;

  if (osettings_isverbose(game->settings))
    oerror_info("ogame_sdlinit: crates version \"%s\" in \"%s\"", oversion_getreleasestr(), odir_getexedirname());

  if (osettings_iscontrollers(game->settings))
    flags |= SDL_INIT_JOYSTICK;

  if (osettings_isaudio(game->settings))
    flags |= SDL_INIT_AUDIO;

  if (SDL_Init(flags) < 0)
    oerror_fatal("SDL initialization failed (%s)", SDL_GetError());

  if (game->settings->fullscreen)
    SDL_ShowCursor(SDL_DISABLE);

  videoInfo = SDL_GetVideoInfo();

  if (!videoInfo)
    oerror_fatal("video query failed (%s)", SDL_GetError());

  videoFlags  = SDL_OPENGL | SDL_HWSURFACE | SDL_HWACCEL | SDL_ANYFORMAT;

  if (osettings_isfullscreen(game->settings))
    videoFlags |= SDL_FULLSCREEN;

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  if (osettings_isverticalsynchronization(game->settings))
    SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);

  modes = SDL_ListModes(NULL, videoFlags);

  if (modes == (SDL_Rect **)0)
    oerror_fatal("%s", "no modes available");

  if (game->settings->verbose)
  {
    oerror_info("hw_available=%d. blit_hw=%d. all_resolutions=%d.", videoInfo->hw_available, videoInfo->blit_hw, modes == (SDL_Rect **)-1);
    if (modes != (SDL_Rect **)-1)
    {
      oerror_info("%s", "available modes:");
      for (i=0; modes[i]; ++i)
        oerror_info("  %d x %d", modes[i]->w, modes[i]->h);
    }
  }

  if (osettings_getscreenwidth(game->settings) && osettings_getscreenheight(game->settings) &&  osettings_getscreenbpp(game->settings))
  {
    game->screen = SDL_SetVideoMode(osettings_getscreenwidth(game->settings),
                                    osettings_getscreenheight(game->settings),
                                    osettings_getscreenbpp(game->settings),
                                    videoFlags);
  }
  else if (osettings_getscreenwidth(game->settings) && osettings_getscreenheight(game->settings) &&  !osettings_getscreenbpp(game->settings))
  {
    game->screen = SDL_SetVideoMode(osettings_getscreenwidth(game->settings),
                                    osettings_getscreenheight(game->settings),
                                    videoInfo->vfmt->BitsPerPixel,
                                    videoFlags);

    game->settings->screenbpp = videoInfo->vfmt->BitsPerPixel;
  }
  else
  {
    game->screen = SDL_SetVideoMode(0, 0, 0, videoFlags);
    game->settings->screenwidth = videoInfo->current_w;
    game->settings->screenheight = videoInfo->current_h;
    game->settings->screenbpp = videoInfo->vfmt->BitsPerPixel;
  }

  if (!game->screen)
    oerror_fatal("video mode set failed: (%s)", SDL_GetError());

  if (osettings_iscontrollers(game->settings) && SDL_NumJoysticks() >= 1)
  {
    if (game->settings->controllernthtouse > 0 && game->settings->controllernthtouse <= SDL_NumJoysticks())
      ctrlidx = game->settings->controllernthtouse-1;
    else
    {
      ctrlidx = 0;
      oerror_warning("no controller number %d. Using instead %d/%d.", game->settings->controllernthtouse, ctrlidx+1, SDL_NumJoysticks());
    }

    game->controller = SDL_JoystickOpen(ctrlidx);

    if (!game->controller)
      oerror_warning("controller %d cannot be opened", ctrlidx+1);
  }

  glEnable(GL_TEXTURE_2D);
  glShadeModel(GL_SMOOTH);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth(1.0f);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  if (game->settings->backfaceculling)
  {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
  }

  if (game->settings->lighting)
  {
    glEnable(GL_LIGHTING);
    glLightfv(GL_LIGHT1, GL_AMBIENT,  ogame_lightambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  ogame_lightdiffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, ogame_lightposition);
    glEnable(GL_LIGHT1);
  }

  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  resizeWindow(osettings_getscreenwidth(game->settings), osettings_getscreenheight(game->settings));
  SDL_WM_SetCaption("Crates!", NULL);
  SDL_EnableKeyRepeat(500, 100);

  if (osettings_isverbose(game->settings))
  {
    GLint value = 0;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &value);
    oerror_info("GL vendor=\"%s\" renderer=\"%s\" version=\"%s\" with maxtexturesize=\"%d\"", glGetString(GL_VENDOR), glGetString(GL_RENDERER), glGetString(GL_VERSION), value);
    oerror_info("ogame_sdlinit done");
  }

  return 1;
}

OGame *ogame_new(void)
{
  extern OGame *game;
  game = (OGame*)oerror_malloc(sizeof(OGame));
  game->nextstatename = ostring_newstr("");
  game->nextstateargument = ostring_newstr("");
  game->quit   = 0;
  game->active = 1;
  game->intermedsec = 0;
  game->delay = 0;
  game->controllerbuttonrepeat = 1;
  game->controllerbutton = -1;
  game->ctrlbuttonnrepeatdelay = 0;
  game->scriptmanager = oscriptmanager_new();
  oluacapi_init(game);
  game->settings = osettings_new(game->scriptmanager);
  game->ctrlbuttonnrepeattrigger = game->settings->controllerbuttonrepeatdelay;
  game->tpf = (1.0 / osettings_getfps(game->settings));

  if (!ogame_sdlinit(game))
    oerror_fatal("%s", "SDL init failed");

  if (osettings_isaudio(game->settings))
  {
    if (game->settings->verbose) oerror_info("oaudiomanager_new");
    game->audiomanager = oaudiomanager_new(game->settings);
    if (game->settings->verbose) oerror_info("oaudiomanager_new done");
  }

  if (game->settings->verbose) oerror_info("otexturemanager_new");
  game->texturemanager = otexturemanager_new(game->settings);                         if (game->settings->verbose) oerror_info("otexturemanager_new done");
  game->meshmanager = omeshmanager_new(game->scriptmanager);                          if (game->settings->verbose) oerror_info("omeshmanager_new    done");
  game->entitymanager = oentitymanager_new(game->scriptmanager);                      if (game->settings->verbose) oerror_info("oentitymanager_new  done");
  oentitymanager_init(game->entitymanager);                                           if (game->settings->verbose) oerror_info("oentitymanager_init done");
  game->missionmanager = omissionmanager_new();                                       if (game->settings->verbose) oerror_info("omissionmanager_new done");
  game->textmanager = otextmanager_new("font.png", game->texturemanager);             if (game->settings->verbose) oerror_info("otextmanager_new    done");
  game->state = ostate_new("mission", "", game->textmanager, game->scriptmanager);    if (game->settings->verbose) oerror_info("ostate_new          done");
  ostate_callluainit(game->state);                                                    if (game->settings->verbose) oerror_info("ostate_callluainit  done");
  game->fps = otimer_new();                                                           if (game->settings->verbose) oerror_info("otimer_new          done");
  oglapi_init(game);                                                                  if (game->settings->verbose) oerror_info("oglapi_init         done");

  if (osettings_isverbose(game->settings))
    oerror_info("ogame_new done");

  return game;
}

void ogame_release(OGame *game)
{
  oboolean verbose;

  if (!game || !game->settings)
    return;

  verbose = game->settings->verbose;

  if (verbose) oerror_info("ogame_release");

  if (game->settings->fullscreen && game->settings->grabinputfullscreen)
  {
    if (verbose) oerror_info("GrabInput OFF");
    SDL_WM_GrabInput(SDL_GRAB_OFF);
    if (verbose) oerror_info("GrabInput OFF done");
  }

  if (verbose) oerror_info("otimer_release");
  otimer_release(game->fps);                                    if (verbose) oerror_info("otimer_release          done");
  ostate_release(game->state);                                  if (verbose) oerror_info("ostate_release          done");
  otextmanager_release(game->textmanager);                      if (verbose) oerror_info("otextmanager_release    done");
  omissionmanager_release(game->missionmanager);                if (verbose) oerror_info("omissionmanager_release done");
  oentitymanager_release(game->entitymanager);                  if (verbose) oerror_info("oentitymanager_release  done");
  omeshmanager_release(game->meshmanager);                      if (verbose) oerror_info("omeshmanager_release    done");
  otexturemanager_release(game->texturemanager);                if (verbose) oerror_info("otexturemanager_release done");

  if (osettings_isaudio(game->settings))
  {
    if (verbose) oerror_info("oaudiomanager_release");
    oaudiomanager_release(game->audiomanager);
    if (verbose) oerror_info("oaudiomanager_release done");
  }

  osettings_release(game->settings);                            if (verbose) oerror_info("osettings_release                   done");
  oscriptmanager_release(game->scriptmanager);                  if (verbose) oerror_info("oscriptmanager_release              done");
  ostring_release(game->nextstatename);                         if (verbose) oerror_info("ostring_release (nextstatename)     done");
  ostring_release(game->nextstateargument);                     if (verbose) oerror_info("ostring_release (nextstateargument) done");
  oglapi_release();                                             if (verbose) oerror_info("oglapi_release                      done");

  if (game->controller)
  {
    if (verbose) oerror_info("JoystickClose");
    SDL_JoystickClose(game->controller);
    if (verbose) oerror_info("JoystickClose done");
  }

  SDL_Quit();                                                  if (verbose) oerror_info("SDL_Quit done");
  memset(game, 0, sizeof(OGame));
  oerror_free(game);
  if (verbose) oerror_info("ogame_release done");
}

void ogame_handleinput(OGame *game)
{
  while (SDL_PollEvent(&game->event))
  {
    if (game->event.type == SDL_QUIT)
    {
      game->quit = 1;
    }
    else if (game->event.type == SDL_ACTIVEEVENT)
    {
      game->active = game->event.active.gain;
    }
    else if (game->event.type == SDL_KEYDOWN)
    {
      switch (game->event.key.keysym.sym)
      {
        case SDLK_RETURN:
          ostate_events(game->state, OEVENT_OK);
          break;

        case SDLK_ESCAPE:
          ostate_events(game->state, OEVENT_EXIT);
          break;

        case SDLK_LEFT:
          ostate_events(game->state, OEVENT_LEFT);
          break;

        case SDLK_RIGHT:
          ostate_events(game->state, OEVENT_RIGHT);
          break;

        case SDLK_UP:
          ostate_events(game->state, OEVENT_UP);
          break;

        case SDLK_DOWN:
          ostate_events(game->state, OEVENT_DOWN);
          break;

        case SDLK_a:
          ostate_events(game->state, OEVENT_A);
          break;

        case SDLK_b:
          ostate_events(game->state, OEVENT_B);
          break;

        case SDLK_c:
          ostate_events(game->state, OEVENT_C);
          break;

        case SDLK_d:
          ostate_events(game->state, OEVENT_D);
          break;

        case SDLK_e:
          ostate_events(game->state, OEVENT_E);
          break;

        case SDLK_f:
          ostate_events(game->state, OEVENT_F);
          break;

        case SDLK_g:
          ostate_events(game->state, OEVENT_G);
          break;

        case SDLK_h:
          ostate_events(game->state, OEVENT_H);
          break;

        case SDLK_i:
          ostate_events(game->state, OEVENT_I);
          break;

        case SDLK_j:
          ostate_events(game->state, OEVENT_J);
          break;

        case SDLK_k:
          ostate_events(game->state, OEVENT_K);
          break;

        case SDLK_l:
          ostate_events(game->state, OEVENT_L);
          break;

        case SDLK_m:
          ostate_events(game->state, OEVENT_M);
          break;

        case SDLK_n:
          ostate_events(game->state, OEVENT_N);
          break;

        case SDLK_o:
          ostate_events(game->state, OEVENT_O);
          break;

        case SDLK_p:
          ostate_events(game->state, OEVENT_P);
          break;

        case SDLK_q:
          ostate_events(game->state, OEVENT_Q);
          break;

        case SDLK_r:
          ostate_events(game->state, OEVENT_R);
          break;

        case SDLK_s:
          ostate_events(game->state, OEVENT_S);
          break;

        case SDLK_t:
          ostate_events(game->state, OEVENT_T);
          break;

        case SDLK_u:
          ostate_events(game->state, OEVENT_U);
          break;

        case SDLK_v:
          ostate_events(game->state, OEVENT_V);
          break;

        case SDLK_w:
          ostate_events(game->state, OEVENT_W);
          break;

        case SDLK_x:
          ostate_events(game->state, OEVENT_X);
          break;

        case SDLK_y:
          ostate_events(game->state, OEVENT_Y);
          break;

        case SDLK_z:
          ostate_events(game->state, OEVENT_Z);
          break;

        case SDLK_0:
          ostate_events(game->state, OEVENT_0);
          break;

        case SDLK_1:
          ostate_events(game->state, OEVENT_1);
          break;

        case SDLK_2:
          ostate_events(game->state, OEVENT_2);
          break;

        case SDLK_3:
          ostate_events(game->state, OEVENT_3);
          break;

        case SDLK_4:
          ostate_events(game->state, OEVENT_4);
          break;

        case SDLK_5:
          ostate_events(game->state, OEVENT_5);
          break;

        case SDLK_6:
          ostate_events(game->state, OEVENT_6);
          break;

        case SDLK_7:
          ostate_events(game->state, OEVENT_7);
          break;

        case SDLK_8:
          ostate_events(game->state, OEVENT_8);
          break;

        case SDLK_9:
          ostate_events(game->state, OEVENT_9);
          break;

        case SDLK_BACKSPACE:
          ostate_events(game->state, OEVENT_BACKSPACE);
          break;

        default:
          break;
      }
    }
    else if (game->event.type == SDL_JOYAXISMOTION)
    {
      SDL_JoyAxisEvent* tmp = (SDL_JoyAxisEvent*)&game->event;

      if (tmp->value > game->settings->controlleraxistriggerdownrightmin && tmp->value < game->settings->controlleraxistriggerdownrightmax)
      {
        if (tmp->axis == game->settings->controlleraxisupdown1 || tmp->axis == game->settings->controlleraxisupdown2)
          ostate_events(game->state, OEVENT_DOWN);
        else if (tmp->axis == game->settings->controlleraxisrightleft1 || tmp->axis == game->settings->controlleraxisrightleft2)
          ostate_events(game->state, OEVENT_RIGHT);
      }
      else if (tmp->value > game->settings->controlleraxistriggerupleftmin && tmp->value < game->settings->controlleraxistriggerupleftmax)
      {
        if (tmp->axis == game->settings->controlleraxisupdown1 || tmp->axis == game->settings->controlleraxisupdown2)
          ostate_events(game->state, OEVENT_UP);
        else if (tmp->axis == game->settings->controlleraxisrightleft1 || tmp->axis == game->settings->controlleraxisrightleft2)
          ostate_events(game->state, OEVENT_LEFT);
      }
    }
    else if (game->event.type == SDL_JOYBUTTONDOWN)
    {
      SDL_JoyButtonEvent* tmp = (SDL_JoyButtonEvent*)&game->event;

      if (tmp->state == SDL_PRESSED)
      {
        if (tmp->button == (osettings_getcontrollerok(game->settings)-1))
          ostate_events(game->state, OEVENT_OK);
        else if (tmp->button == (osettings_getcontrollerexit(game->settings)-1))
          ostate_events(game->state, OEVENT_EXIT);
        else if (tmp->button == (osettings_getcontrollerrestart(game->settings)-1))
          ostate_events(game->state, OEVENT_R);
      }
    }
    else if (game->event.type == SDL_JOYBALLMOTION)
    {
    }
    else if (game->event.type == SDL_JOYHATMOTION)
    {
      SDL_JoyHatEvent* tmp = (SDL_JoyHatEvent*)&game->event;

      if (tmp->value == game->settings->controllerhatup)
      {
        ostate_events(game->state, OEVENT_UP);
        game->controllerbutton = OEVENT_UP;
      }
      else if (tmp->value == game->settings->controllerhatright)
      {
        ostate_events(game->state, OEVENT_RIGHT);
        game->controllerbutton =  OEVENT_RIGHT;
      }
      else if (tmp->value == game->settings->controllerhatdown)
      {
        ostate_events(game->state, OEVENT_DOWN);
        game->controllerbutton =  OEVENT_DOWN;
      }
      else if (tmp->value == game->settings->controllerhatleft)
      {
        ostate_events(game->state, OEVENT_LEFT);
        game->controllerbutton = OEVENT_LEFT;
      }
      else if (tmp->value == SDL_HAT_CENTERED)
      {
        game->controllerbutton = -1;
        game->ctrlbuttonnrepeatdelay = 0;
        game->ctrlbuttonnrepeattrigger = game->settings->controllerbuttonrepeatdelay;
      }
    }
  }
}

void ogame_loop(OGame *game)
{
  oint32 nframes = 0;
  oint32 fpsframes = 0;
  onumber lastreporttime = 0;
  const oint32 reportdelay = 5;

  if (game->settings->fullscreen && game->settings->grabinputfullscreen)
    SDL_WM_GrabInput(SDL_GRAB_ON);

  while (game->quit == 0)
  {
    otimer_resetintermediate(game->fps);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ogame_handleinput(game);

    if (game->controllerbuttonrepeat && game->controllerbutton != -1)
    {
      game->ctrlbuttonnrepeatdelay++;
      if (game->ctrlbuttonnrepeatdelay > game->ctrlbuttonnrepeattrigger)
      {
        ostate_events(game->state, (enum OEvent)game->controllerbutton);
        game->ctrlbuttonnrepeatdelay = 0;
        game->ctrlbuttonnrepeattrigger = game->settings->controllerbuttonrepeatinterval;
      }
    }

    if (game->active)
    {
      if (ostring_length(game->nextstatename) > 0)
      {
        ostate_release(game->state);
        game->state = ostate_new(ostring_tocstr(game->nextstatename), ostring_tocstr(game->nextstateargument), game->textmanager, game->scriptmanager);
        ostate_callluainit(game->state);
        ostring_setstr(game->nextstatename, "");
        ostring_setstr(game->nextstateargument, "");
      }

      ostate_update(game->state, game->tpf);
      ostate_move(game->state, game->tpf);
      ostate_collide(game->state);
      ostate_render(game->state);
      SDL_GL_SwapBuffers();

      game->intermedsec = otimer_getintermediateseconds(game->fps);
      if (game->settings->verbose > 1)
      {
        GLenum glerrornum;
        nframes++;
        fpsframes++;
        if (lastreporttime < otimer_gettotalseconds(game->fps) - reportdelay)
        {
          lastreporttime = otimer_gettotalseconds(game->fps);
          oerror_info("real_tpf=%f s. max_possible_fps=%.0f frames/s. total_frames=%d current_fps=%d", game->intermedsec, 1.0 / game->intermedsec, nframes, fpsframes/reportdelay);
          fpsframes = 0;
        }

        glerrornum = glGetError();
        if (glerrornum != GL_NO_ERROR)
          oerror_warning("GL error %d (%s)", glerrornum, gluErrorString(glerrornum));
      }
    }

    game->delay = fabs(game->tpf - game->intermedsec)*1000;

    if (game->delay > 100 || game->delay <= 1)
      game->delay = 10;

    SDL_Delay(game->delay);
  }
}

void ogame_quit(OGame *game)
{
  game->quit = 1;
}
