--[[
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
--]]
manual = {}
manual.initial = {}
manual.page2 = {}
manual.page3 = {}
manual.page4 = {}

function manual.initial.init(arg)
  manual.arg = tostring(arg)
  manual.bw = 512
  manual.bh = 512
  manual.sw, manual.sh = settings_getscreendimensions()
  manual.x = 0;
  if manual.bh > manual.sh then
    manual.y = manual.sh - manual.bh
  else
    manual.y = 0
  end
  manual.bgid1 = texture_nametoid("manual1.png")
  manual.bgid2 = texture_nametoid("manual2.png")
  manual.bgid3 = texture_nametoid("manual3.png")
  manual.bgid4 = texture_nametoid("manual4.png")
  if exit.errorsound == nil then exit.errorsound = sound_nametoid("back.wav") end
  state_seteventable(true)
  state_setrenderable(true)
end

function manual.initial.update(tpf)
end

function manual.initial.event(evt)
  if     evt == "exit" then
    sound_play(mission.backsound)
    game_changestate("menu", manual.arg)
  end

  if evt == "down"  or evt == "right" then
    state_setstate("page2")
  elseif evt == "up" or evt == "left" then
    sound_play(exit.errorsound)
  end
end

function manual.initial.render()
  opengl_color3(1, 1, 1)
  background_renderwithsizes(manual.bgid1, manual.x, manual.y, manual.bw, manual.bh)
  opengl_color3(1, 1, 0)
end

-------------------------------------------------------------------------------
function manual.page2.update(tpf)
end

function manual.page2.event(evt)
  if     evt == "exit" then
    sound_play(mission.backsound)
    game_changestate("menu", manual.arg)
  end

  if evt == "down"  or evt == "right" then
    state_setstate("page3")
  elseif evt == "up" or evt == "left" then
    state_setstate("initial")
  end
end

function manual.page2.render()
  opengl_color3(1, 1, 1)
  background_renderwithsizes(manual.bgid2, manual.x, manual.y, manual.bw, manual.bh)
  opengl_color3(1, 1, 0)
end

-------------------------------------------------------------------------------
function manual.page3.update(tpf)
end

function manual.page3.event(evt)
  if     evt == "exit" then
    sound_play(mission.backsound)
    game_changestate("menu", manual.arg)
  end

  if evt == "down"  or evt == "right" then
    state_setstate("page4")
  elseif evt == "up" or evt == "left" then
    state_setstate("page2")
  end
end

function manual.page3.render()
  opengl_color3(1, 1, 1)
  background_renderwithsizes(manual.bgid3, manual.x, manual.y, manual.bw, manual.bh)
  opengl_color3(1, 1, 0)
end

-------------------------------------------------------------------------------
function manual.page4.update(tpf)
end

function manual.page4.event(evt)
  if     evt == "exit" then
    sound_play(mission.backsound)
    game_changestate("menu", manual.arg)
  end

  if evt == "down"  or evt == "right" then
    sound_play(exit.errorsound)
  elseif evt == "up" or evt == "left" then
    state_setstate("page3")
  end
end

function manual.page4.render()
  opengl_color3(1, 1, 1)
  background_renderwithsizes(manual.bgid4, manual.x, manual.y, manual.bw, manual.bh)
  opengl_color3(1, 1, 0)
end
