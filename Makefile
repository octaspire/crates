#
# Makefile for posix-systems.
# This Makefile was build automatically with perl-script: bm.pl.
# There might be other Makefiles available for different systems.
#

CC = gcc
OBJS = src/main.o src/oaudiomanager.o src/oentity.o src/oentitymanager.o src/oerror.o src/oevent.o src/ofile.o src/ogame.o src/oglapi.o src/olevel.o src/oluacapi.o src/omap.o src/omeshmanager.o src/omessagestack.o src/omission.o src/omissionmanager.o src/omusicmanager.o src/optrmap.o src/optrvector.o src/oscriptmanager.o src/osettings.o src/osoundmanager.o src/ostate.o src/ostring.o src/otextmanager.o src/otexturemanager.o src/ovector.o src/ovector3.o src/oversion.o src/posix/odir.o src/posix/otimer.o 
LUAIPATH = /usr/include/lua5.1
LUALPATH = /usr/lib/
CFLAGS  = -g -Wall -ansi -pedantic `sdl-config --cflags` -I $(LUAIPATH)
LFLAGS = `sdl-config --libs` -lSDL_image -lSDL_mixer -lGL -lGLU -L $(LUALPATH) -llua5.1

crates: $(OBJS)
	$(CC) -o crates $(OBJS) $(LFLAGS)

main.o: src/main.c src/odir.h src/otypes.h src/ogame.h \
 /usr/include/SDL/SDL.h /usr/include/SDL/SDL_main.h \
 /usr/include/SDL/SDL_stdinc.h /usr/include/SDL/SDL_config.h \
 /usr/include/SDL/SDL_platform.h /usr/include/SDL/begin_code.h \
 /usr/include/SDL/close_code.h /usr/include/SDL/SDL_audio.h \
 /usr/include/SDL/SDL_error.h /usr/include/SDL/SDL_endian.h \
 /usr/include/SDL/SDL_mutex.h /usr/include/SDL/SDL_thread.h \
 /usr/include/SDL/SDL_rwops.h /usr/include/SDL/SDL_cdrom.h \
 /usr/include/SDL/SDL_cpuinfo.h /usr/include/SDL/SDL_events.h \
 /usr/include/SDL/SDL_active.h /usr/include/SDL/SDL_keyboard.h \
 /usr/include/SDL/SDL_keysym.h /usr/include/SDL/SDL_mouse.h \
 /usr/include/SDL/SDL_video.h /usr/include/SDL/SDL_joystick.h \
 /usr/include/SDL/SDL_quit.h /usr/include/SDL/SDL_loadso.h \
 /usr/include/SDL/SDL_timer.h /usr/include/SDL/SDL_version.h \
 /usr/include/SDL/SDL_image.h /usr/include/SDL/SDL.h src/oentity.h \
 src/ovector3.h src/oscriptmanager.h /usr/include/lua5.1/lua.h \
 /usr/include/lua5.1/luaconf.h /usr/include/lua5.1/lauxlib.h \
 /usr/include/lua5.1/lua.h /usr/include/lua5.1/lualib.h src/optrvector.h \
 src/ostring.h src/ovector.h src/oevent.h /usr/include/SDL/SDL_opengl.h \
 src/oerror.h src/otimer.h src/ostate.h src/omessagestack.h \
 src/otextmanager.h src/otexturemanager.h src/omap.h src/omeshmanager.h \
 src/omissionmanager.h src/omission.h src/olevel.h src/osettings.h \
 src/oaudiomanager.h src/osoundmanager.h src/optrmap.h \
 /usr/include/SDL/SDL_mixer.h /usr/include/SDL/SDL_types.h \
 src/omusicmanager.h src/oentitymanager.h src/oversion.h
	$(CC) -c -o $@ $(CFLAGS) $<

oaudiomanager.o: src/oaudiomanager.c src/oaudiomanager.h \
 src/osoundmanager.h src/osettings.h src/oscriptmanager.h \
 /usr/include/lua5.1/lua.h /usr/include/lua5.1/luaconf.h \
 /usr/include/lua5.1/lauxlib.h /usr/include/lua5.1/lua.h \
 /usr/include/lua5.1/lualib.h src/otypes.h src/optrvector.h src/ostring.h \
 src/ovector.h src/optrmap.h /usr/include/SDL/SDL_mixer.h \
 /usr/include/SDL/SDL_types.h /usr/include/SDL/SDL_stdinc.h \
 /usr/include/SDL/SDL_config.h /usr/include/SDL/SDL_platform.h \
 /usr/include/SDL/begin_code.h /usr/include/SDL/close_code.h \
 /usr/include/SDL/SDL_rwops.h /usr/include/SDL/SDL_error.h \
 /usr/include/SDL/SDL_audio.h /usr/include/SDL/SDL_endian.h \
 /usr/include/SDL/SDL_mutex.h /usr/include/SDL/SDL_thread.h \
 /usr/include/SDL/SDL_version.h src/omusicmanager.h src/oerror.h
	$(CC) -c -o $@ $(CFLAGS) $<

oentity.o: src/oentity.c src/obits.h src/oentity.h src/ovector3.h \
 src/otypes.h src/oscriptmanager.h /usr/include/lua5.1/lua.h \
 /usr/include/lua5.1/luaconf.h /usr/include/lua5.1/lauxlib.h \
 /usr/include/lua5.1/lua.h /usr/include/lua5.1/lualib.h src/optrvector.h \
 src/ostring.h src/ovector.h src/oevent.h /usr/include/SDL/SDL_opengl.h \
 /usr/include/SDL/SDL_config.h /usr/include/SDL/SDL_platform.h \
 src/oerror.h src/odir.h src/ostate.h src/omessagestack.h \
 src/otextmanager.h src/otexturemanager.h src/omap.h
	$(CC) -c -o $@ $(CFLAGS) $<

oentitymanager.o: src/oentitymanager.c src/oentitymanager.h src/oentity.h \
 src/ovector3.h src/otypes.h src/oscriptmanager.h \
 /usr/include/lua5.1/lua.h /usr/include/lua5.1/luaconf.h \
 /usr/include/lua5.1/lauxlib.h /usr/include/lua5.1/lua.h \
 /usr/include/lua5.1/lualib.h src/optrvector.h src/ostring.h \
 src/ovector.h src/oevent.h /usr/include/SDL/SDL_opengl.h \
 /usr/include/SDL/SDL_config.h /usr/include/SDL/SDL_platform.h \
 src/optrmap.h src/oerror.h src/odir.h src/ofile.h
	$(CC) -c -o $@ $(CFLAGS) $<

oerror.o: src/oerror.c src/oerror.h src/otypes.h
	$(CC) -c -o $@ $(CFLAGS) $<

oevent.o: src/oevent.c src/oevent.h src/otypes.h
	$(CC) -c -o $@ $(CFLAGS) $<

ofile.o: src/ofile.c src/ofile.h src/otypes.h src/ostring.h src/ovector.h
	$(CC) -c -o $@ $(CFLAGS) $<

ogame.o: src/ogame.c src/ogame.h /usr/include/SDL/SDL.h \
 /usr/include/SDL/SDL_main.h /usr/include/SDL/SDL_stdinc.h \
 /usr/include/SDL/SDL_config.h /usr/include/SDL/SDL_platform.h \
 /usr/include/SDL/begin_code.h /usr/include/SDL/close_code.h \
 /usr/include/SDL/SDL_audio.h /usr/include/SDL/SDL_error.h \
 /usr/include/SDL/SDL_endian.h /usr/include/SDL/SDL_mutex.h \
 /usr/include/SDL/SDL_thread.h /usr/include/SDL/SDL_rwops.h \
 /usr/include/SDL/SDL_cdrom.h /usr/include/SDL/SDL_cpuinfo.h \
 /usr/include/SDL/SDL_events.h /usr/include/SDL/SDL_active.h \
 /usr/include/SDL/SDL_keyboard.h /usr/include/SDL/SDL_keysym.h \
 /usr/include/SDL/SDL_mouse.h /usr/include/SDL/SDL_video.h \
 /usr/include/SDL/SDL_joystick.h /usr/include/SDL/SDL_quit.h \
 /usr/include/SDL/SDL_loadso.h /usr/include/SDL/SDL_timer.h \
 /usr/include/SDL/SDL_version.h /usr/include/SDL/SDL_image.h \
 /usr/include/SDL/SDL.h src/oentity.h src/ovector3.h src/otypes.h \
 src/oscriptmanager.h /usr/include/lua5.1/lua.h \
 /usr/include/lua5.1/luaconf.h /usr/include/lua5.1/lauxlib.h \
 /usr/include/lua5.1/lua.h /usr/include/lua5.1/lualib.h src/optrvector.h \
 src/ostring.h src/ovector.h src/oevent.h /usr/include/SDL/SDL_opengl.h \
 src/oerror.h src/otimer.h src/ostate.h src/omessagestack.h \
 src/otextmanager.h src/otexturemanager.h src/omap.h src/omeshmanager.h \
 src/omissionmanager.h src/omission.h src/olevel.h src/osettings.h \
 src/oaudiomanager.h src/osoundmanager.h src/optrmap.h \
 /usr/include/SDL/SDL_mixer.h /usr/include/SDL/SDL_types.h \
 src/omusicmanager.h src/oentitymanager.h src/odir.h src/oluacapi.h \
 src/oglapi.h src/oversion.h
	$(CC) -c -o $@ $(CFLAGS) $<

oglapi.o: src/oglapi.c src/oglapi.h src/ogame.h /usr/include/SDL/SDL.h \
 /usr/include/SDL/SDL_main.h /usr/include/SDL/SDL_stdinc.h \
 /usr/include/SDL/SDL_config.h /usr/include/SDL/SDL_platform.h \
 /usr/include/SDL/begin_code.h /usr/include/SDL/close_code.h \
 /usr/include/SDL/SDL_audio.h /usr/include/SDL/SDL_error.h \
 /usr/include/SDL/SDL_endian.h /usr/include/SDL/SDL_mutex.h \
 /usr/include/SDL/SDL_thread.h /usr/include/SDL/SDL_rwops.h \
 /usr/include/SDL/SDL_cdrom.h /usr/include/SDL/SDL_cpuinfo.h \
 /usr/include/SDL/SDL_events.h /usr/include/SDL/SDL_active.h \
 /usr/include/SDL/SDL_keyboard.h /usr/include/SDL/SDL_keysym.h \
 /usr/include/SDL/SDL_mouse.h /usr/include/SDL/SDL_video.h \
 /usr/include/SDL/SDL_joystick.h /usr/include/SDL/SDL_quit.h \
 /usr/include/SDL/SDL_loadso.h /usr/include/SDL/SDL_timer.h \
 /usr/include/SDL/SDL_version.h /usr/include/SDL/SDL_image.h \
 /usr/include/SDL/SDL.h src/oentity.h src/ovector3.h src/otypes.h \
 src/oscriptmanager.h /usr/include/lua5.1/lua.h \
 /usr/include/lua5.1/luaconf.h /usr/include/lua5.1/lauxlib.h \
 /usr/include/lua5.1/lua.h /usr/include/lua5.1/lualib.h src/optrvector.h \
 src/ostring.h src/ovector.h src/oevent.h /usr/include/SDL/SDL_opengl.h \
 src/oerror.h src/otimer.h src/ostate.h src/omessagestack.h \
 src/otextmanager.h src/otexturemanager.h src/omap.h src/omeshmanager.h \
 src/omissionmanager.h src/omission.h src/olevel.h src/osettings.h \
 src/oaudiomanager.h src/osoundmanager.h src/optrmap.h \
 /usr/include/SDL/SDL_mixer.h /usr/include/SDL/SDL_types.h \
 src/omusicmanager.h src/oentitymanager.h
	$(CC) -c -o $@ $(CFLAGS) $<

olevel.o: src/olevel.c src/olevel.h src/optrvector.h src/otypes.h \
 src/ostring.h src/ovector.h src/oerror.h
	$(CC) -c -o $@ $(CFLAGS) $<

oluacapi.o: src/oluacapi.c src/oluacapi.h /usr/include/lua5.1/lua.h \
 /usr/include/lua5.1/luaconf.h src/ogame.h /usr/include/SDL/SDL.h \
 /usr/include/SDL/SDL_main.h /usr/include/SDL/SDL_stdinc.h \
 /usr/include/SDL/SDL_config.h /usr/include/SDL/SDL_platform.h \
 /usr/include/SDL/begin_code.h /usr/include/SDL/close_code.h \
 /usr/include/SDL/SDL_audio.h /usr/include/SDL/SDL_error.h \
 /usr/include/SDL/SDL_endian.h /usr/include/SDL/SDL_mutex.h \
 /usr/include/SDL/SDL_thread.h /usr/include/SDL/SDL_rwops.h \
 /usr/include/SDL/SDL_cdrom.h /usr/include/SDL/SDL_cpuinfo.h \
 /usr/include/SDL/SDL_events.h /usr/include/SDL/SDL_active.h \
 /usr/include/SDL/SDL_keyboard.h /usr/include/SDL/SDL_keysym.h \
 /usr/include/SDL/SDL_mouse.h /usr/include/SDL/SDL_video.h \
 /usr/include/SDL/SDL_joystick.h /usr/include/SDL/SDL_quit.h \
 /usr/include/SDL/SDL_loadso.h /usr/include/SDL/SDL_timer.h \
 /usr/include/SDL/SDL_version.h /usr/include/SDL/SDL_image.h \
 /usr/include/SDL/SDL.h src/oentity.h src/ovector3.h src/otypes.h \
 src/oscriptmanager.h /usr/include/lua5.1/lauxlib.h \
 /usr/include/lua5.1/lua.h /usr/include/lua5.1/lualib.h src/optrvector.h \
 src/ostring.h src/ovector.h src/oevent.h /usr/include/SDL/SDL_opengl.h \
 src/oerror.h src/otimer.h src/ostate.h src/omessagestack.h \
 src/otextmanager.h src/otexturemanager.h src/omap.h src/omeshmanager.h \
 src/omissionmanager.h src/omission.h src/olevel.h src/osettings.h \
 src/oaudiomanager.h src/osoundmanager.h src/optrmap.h \
 /usr/include/SDL/SDL_mixer.h /usr/include/SDL/SDL_types.h \
 src/omusicmanager.h src/oentitymanager.h src/odir.h src/oglapi.h \
 src/oversion.h
	$(CC) -c -o $@ $(CFLAGS) $<

omap.o: src/omap.c src/omap.h src/otypes.h src/optrvector.h src/ovector.h \
 src/oerror.h src/ostring.h
	$(CC) -c -o $@ $(CFLAGS) $<

omeshmanager.o: src/omeshmanager.c src/omeshmanager.h \
 src/oscriptmanager.h /usr/include/lua5.1/lua.h \
 /usr/include/lua5.1/luaconf.h /usr/include/lua5.1/lauxlib.h \
 /usr/include/lua5.1/lua.h /usr/include/lua5.1/lualib.h src/otypes.h \
 src/optrvector.h src/ostring.h src/ovector.h src/omap.h \
 /usr/include/SDL/SDL_opengl.h /usr/include/SDL/SDL_config.h \
 /usr/include/SDL/SDL_platform.h src/odir.h src/oerror.h \
 /usr/include/SDL/SDL.h /usr/include/SDL/SDL_main.h \
 /usr/include/SDL/SDL_stdinc.h /usr/include/SDL/begin_code.h \
 /usr/include/SDL/close_code.h /usr/include/SDL/SDL_audio.h \
 /usr/include/SDL/SDL_error.h /usr/include/SDL/SDL_endian.h \
 /usr/include/SDL/SDL_mutex.h /usr/include/SDL/SDL_thread.h \
 /usr/include/SDL/SDL_rwops.h /usr/include/SDL/SDL_cdrom.h \
 /usr/include/SDL/SDL_cpuinfo.h /usr/include/SDL/SDL_events.h \
 /usr/include/SDL/SDL_active.h /usr/include/SDL/SDL_keyboard.h \
 /usr/include/SDL/SDL_keysym.h /usr/include/SDL/SDL_mouse.h \
 /usr/include/SDL/SDL_video.h /usr/include/SDL/SDL_joystick.h \
 /usr/include/SDL/SDL_quit.h /usr/include/SDL/SDL_loadso.h \
 /usr/include/SDL/SDL_timer.h /usr/include/SDL/SDL_version.h \
 /usr/include/SDL/SDL_image.h /usr/include/SDL/SDL.h
	$(CC) -c -o $@ $(CFLAGS) $<

omessagestack.o: src/omessagestack.c src/omessagestack.h \
 src/otextmanager.h src/ostring.h src/otypes.h src/ovector.h \
 src/otexturemanager.h src/omap.h src/optrvector.h \
 /usr/include/SDL/SDL_opengl.h /usr/include/SDL/SDL_config.h \
 /usr/include/SDL/SDL_platform.h src/oerror.h
	$(CC) -c -o $@ $(CFLAGS) $<

omission.o: src/omission.c src/omission.h src/olevel.h src/optrvector.h \
 src/otypes.h src/ostring.h src/ovector.h src/oerror.h src/ofile.h \
 src/ogame.h /usr/include/SDL/SDL.h /usr/include/SDL/SDL_main.h \
 /usr/include/SDL/SDL_stdinc.h /usr/include/SDL/SDL_config.h \
 /usr/include/SDL/SDL_platform.h /usr/include/SDL/begin_code.h \
 /usr/include/SDL/close_code.h /usr/include/SDL/SDL_audio.h \
 /usr/include/SDL/SDL_error.h /usr/include/SDL/SDL_endian.h \
 /usr/include/SDL/SDL_mutex.h /usr/include/SDL/SDL_thread.h \
 /usr/include/SDL/SDL_rwops.h /usr/include/SDL/SDL_cdrom.h \
 /usr/include/SDL/SDL_cpuinfo.h /usr/include/SDL/SDL_events.h \
 /usr/include/SDL/SDL_active.h /usr/include/SDL/SDL_keyboard.h \
 /usr/include/SDL/SDL_keysym.h /usr/include/SDL/SDL_mouse.h \
 /usr/include/SDL/SDL_video.h /usr/include/SDL/SDL_joystick.h \
 /usr/include/SDL/SDL_quit.h /usr/include/SDL/SDL_loadso.h \
 /usr/include/SDL/SDL_timer.h /usr/include/SDL/SDL_version.h \
 /usr/include/SDL/SDL_image.h /usr/include/SDL/SDL.h src/oentity.h \
 src/ovector3.h src/oscriptmanager.h /usr/include/lua5.1/lua.h \
 /usr/include/lua5.1/luaconf.h /usr/include/lua5.1/lauxlib.h \
 /usr/include/lua5.1/lua.h /usr/include/lua5.1/lualib.h src/oevent.h \
 /usr/include/SDL/SDL_opengl.h src/otimer.h src/ostate.h \
 src/omessagestack.h src/otextmanager.h src/otexturemanager.h src/omap.h \
 src/omeshmanager.h src/omissionmanager.h src/osettings.h \
 src/oaudiomanager.h src/osoundmanager.h src/optrmap.h \
 /usr/include/SDL/SDL_mixer.h /usr/include/SDL/SDL_types.h \
 src/omusicmanager.h src/oentitymanager.h
	$(CC) -c -o $@ $(CFLAGS) $<

omissionmanager.o: src/omissionmanager.c src/omissionmanager.h \
 src/omission.h src/olevel.h src/optrvector.h src/otypes.h src/ostring.h \
 src/ovector.h src/oerror.h src/odir.h
	$(CC) -c -o $@ $(CFLAGS) $<

omusicmanager.o: src/omusicmanager.c src/omusicmanager.h src/osettings.h \
 src/oscriptmanager.h /usr/include/lua5.1/lua.h \
 /usr/include/lua5.1/luaconf.h /usr/include/lua5.1/lauxlib.h \
 /usr/include/lua5.1/lua.h /usr/include/lua5.1/lualib.h src/otypes.h \
 src/optrvector.h src/ostring.h src/ovector.h src/optrmap.h \
 /usr/include/SDL/SDL_mixer.h /usr/include/SDL/SDL_types.h \
 /usr/include/SDL/SDL_stdinc.h /usr/include/SDL/SDL_config.h \
 /usr/include/SDL/SDL_platform.h /usr/include/SDL/begin_code.h \
 /usr/include/SDL/close_code.h /usr/include/SDL/SDL_rwops.h \
 /usr/include/SDL/SDL_error.h /usr/include/SDL/SDL_audio.h \
 /usr/include/SDL/SDL_endian.h /usr/include/SDL/SDL_mutex.h \
 /usr/include/SDL/SDL_thread.h /usr/include/SDL/SDL_version.h src/odir.h \
 src/oerror.h
	$(CC) -c -o $@ $(CFLAGS) $<

optrmap.o: src/optrmap.c src/optrmap.h src/otypes.h src/optrvector.h \
 src/oerror.h src/ostring.h src/ovector.h
	$(CC) -c -o $@ $(CFLAGS) $<

optrvector.o: src/optrvector.c src/optrvector.h src/otypes.h src/oerror.h
	$(CC) -c -o $@ $(CFLAGS) $<

oscriptmanager.o: src/oscriptmanager.c src/oscriptmanager.h \
 /usr/include/lua5.1/lua.h /usr/include/lua5.1/luaconf.h \
 /usr/include/lua5.1/lauxlib.h /usr/include/lua5.1/lua.h \
 /usr/include/lua5.1/lualib.h src/otypes.h src/optrvector.h src/oerror.h \
 src/ostring.h src/ovector.h
	$(CC) -c -o $@ $(CFLAGS) $<

osettings.o: src/osettings.c src/osettings.h src/oscriptmanager.h \
 /usr/include/lua5.1/lua.h /usr/include/lua5.1/luaconf.h \
 /usr/include/lua5.1/lauxlib.h /usr/include/lua5.1/lua.h \
 /usr/include/lua5.1/lualib.h src/otypes.h src/optrvector.h src/oerror.h \
 src/odir.h
	$(CC) -c -o $@ $(CFLAGS) $<

osoundmanager.o: src/osoundmanager.c src/osoundmanager.h src/osettings.h \
 src/oscriptmanager.h /usr/include/lua5.1/lua.h \
 /usr/include/lua5.1/luaconf.h /usr/include/lua5.1/lauxlib.h \
 /usr/include/lua5.1/lua.h /usr/include/lua5.1/lualib.h src/otypes.h \
 src/optrvector.h src/ostring.h src/ovector.h src/optrmap.h \
 /usr/include/SDL/SDL_mixer.h /usr/include/SDL/SDL_types.h \
 /usr/include/SDL/SDL_stdinc.h /usr/include/SDL/SDL_config.h \
 /usr/include/SDL/SDL_platform.h /usr/include/SDL/begin_code.h \
 /usr/include/SDL/close_code.h /usr/include/SDL/SDL_rwops.h \
 /usr/include/SDL/SDL_error.h /usr/include/SDL/SDL_audio.h \
 /usr/include/SDL/SDL_endian.h /usr/include/SDL/SDL_mutex.h \
 /usr/include/SDL/SDL_thread.h /usr/include/SDL/SDL_version.h src/odir.h \
 src/oerror.h
	$(CC) -c -o $@ $(CFLAGS) $<

ostate.o: src/ostate.c src/ostate.h src/oentity.h src/ovector3.h \
 src/otypes.h src/oscriptmanager.h /usr/include/lua5.1/lua.h \
 /usr/include/lua5.1/luaconf.h /usr/include/lua5.1/lauxlib.h \
 /usr/include/lua5.1/lua.h /usr/include/lua5.1/lualib.h src/optrvector.h \
 src/ostring.h src/ovector.h src/oevent.h /usr/include/SDL/SDL_opengl.h \
 /usr/include/SDL/SDL_config.h /usr/include/SDL/SDL_platform.h \
 src/omessagestack.h src/otextmanager.h src/otexturemanager.h src/omap.h \
 src/oerror.h src/odir.h
	$(CC) -c -o $@ $(CFLAGS) $<

ostring.o: src/ostring.c src/ostring.h src/otypes.h src/ovector.h \
 src/oerror.h
	$(CC) -c -o $@ $(CFLAGS) $<

otextmanager.o: src/otextmanager.c src/otextmanager.h src/ostring.h \
 src/otypes.h src/ovector.h src/otexturemanager.h src/omap.h \
 src/optrvector.h /usr/include/SDL/SDL_opengl.h \
 /usr/include/SDL/SDL_config.h /usr/include/SDL/SDL_platform.h \
 src/oglapi.h src/ogame.h /usr/include/SDL/SDL.h \
 /usr/include/SDL/SDL_main.h /usr/include/SDL/SDL_stdinc.h \
 /usr/include/SDL/begin_code.h /usr/include/SDL/close_code.h \
 /usr/include/SDL/SDL_audio.h /usr/include/SDL/SDL_error.h \
 /usr/include/SDL/SDL_endian.h /usr/include/SDL/SDL_mutex.h \
 /usr/include/SDL/SDL_thread.h /usr/include/SDL/SDL_rwops.h \
 /usr/include/SDL/SDL_cdrom.h /usr/include/SDL/SDL_cpuinfo.h \
 /usr/include/SDL/SDL_events.h /usr/include/SDL/SDL_active.h \
 /usr/include/SDL/SDL_keyboard.h /usr/include/SDL/SDL_keysym.h \
 /usr/include/SDL/SDL_mouse.h /usr/include/SDL/SDL_video.h \
 /usr/include/SDL/SDL_joystick.h /usr/include/SDL/SDL_quit.h \
 /usr/include/SDL/SDL_loadso.h /usr/include/SDL/SDL_timer.h \
 /usr/include/SDL/SDL_version.h /usr/include/SDL/SDL_image.h \
 /usr/include/SDL/SDL.h src/oentity.h src/ovector3.h src/oscriptmanager.h \
 /usr/include/lua5.1/lua.h /usr/include/lua5.1/luaconf.h \
 /usr/include/lua5.1/lauxlib.h /usr/include/lua5.1/lua.h \
 /usr/include/lua5.1/lualib.h src/oevent.h src/oerror.h src/otimer.h \
 src/ostate.h src/omessagestack.h src/omeshmanager.h \
 src/omissionmanager.h src/omission.h src/olevel.h src/osettings.h \
 src/oaudiomanager.h src/osoundmanager.h src/optrmap.h \
 /usr/include/SDL/SDL_mixer.h /usr/include/SDL/SDL_types.h \
 src/omusicmanager.h src/oentitymanager.h
	$(CC) -c -o $@ $(CFLAGS) $<

otexturemanager.o: src/otexturemanager.c src/otexturemanager.h \
 src/otypes.h src/ostring.h src/ovector.h src/omap.h src/optrvector.h \
 /usr/include/SDL/SDL_opengl.h /usr/include/SDL/SDL_config.h \
 /usr/include/SDL/SDL_platform.h src/odir.h src/oerror.h \
 /usr/include/SDL/SDL.h /usr/include/SDL/SDL_main.h \
 /usr/include/SDL/SDL_stdinc.h /usr/include/SDL/begin_code.h \
 /usr/include/SDL/close_code.h /usr/include/SDL/SDL_audio.h \
 /usr/include/SDL/SDL_error.h /usr/include/SDL/SDL_endian.h \
 /usr/include/SDL/SDL_mutex.h /usr/include/SDL/SDL_thread.h \
 /usr/include/SDL/SDL_rwops.h /usr/include/SDL/SDL_cdrom.h \
 /usr/include/SDL/SDL_cpuinfo.h /usr/include/SDL/SDL_events.h \
 /usr/include/SDL/SDL_active.h /usr/include/SDL/SDL_keyboard.h \
 /usr/include/SDL/SDL_keysym.h /usr/include/SDL/SDL_mouse.h \
 /usr/include/SDL/SDL_video.h /usr/include/SDL/SDL_joystick.h \
 /usr/include/SDL/SDL_quit.h /usr/include/SDL/SDL_loadso.h \
 /usr/include/SDL/SDL_timer.h /usr/include/SDL/SDL_version.h \
 /usr/include/SDL/SDL_image.h /usr/include/SDL/SDL.h
	$(CC) -c -o $@ $(CFLAGS) $<

ovector.o: src/ovector.c src/ovector.h src/otypes.h src/oerror.h
	$(CC) -c -o $@ $(CFLAGS) $<

ovector3.o: src/ovector3.c src/ovector3.h src/otypes.h src/oerror.h
	$(CC) -c -o $@ $(CFLAGS) $<

oversion.o: src/oversion.c src/otypes.h src/oerror.h
	$(CC) -c -o $@ $(CFLAGS) $<

odir.o: src/posix/odir.c src/posix/../odir.h src/posix/../otypes.h \
 src/posix/../oerror.h src/posix/../ostring.h src/posix/../ovector.h
	$(CC) -c -o $@ $(CFLAGS) $<

otimer.o: src/posix/otimer.c src/posix/../otimer.h src/posix/../otypes.h \
 src/posix/../otypes.h src/posix/../oerror.h
	$(CC) -c -o $@ $(CFLAGS) $<

clean:
	rm -f src/*.o src/posix/*.o
	rm -f crates
