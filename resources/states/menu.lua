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
menu = {}
menu.initial = {}

function menu.selectmission()
  game_changestate("mission", "")
end

-------------------------------------------------------------------------------
function menu.initial.init(arg)
  menu.items = {}
  menu.sindex = 1
  menu.arg = ""
  menu.cr = 1
  menu.crvel = -0.08
  menu.arg = tostring(arg)
  menu.bw = 1024
  menu.bh = 1024
  menu.bgid = texture_nametoid("bg" .. tostring(menu.bw) .. "x" .. tostring(menu.bh) .. ".png")

  state_seteventable(true)
  state_setrenderable(true)

  menu.items[1] = "Start New Game"
  menu.items[2] = "Enter Password"
  menu.items[3] = "Select Mission"
  menu.items[4] = "Read The Manual"
  menu.items[5] = "See Hall Of Fame"
  menu.items[6] = "About Crates"
  menu.items[7] = "Quit And Exit!"

  game_parsemission(menu.arg)
end

function menu.initial.update(tpf)
end

function menu.initial.event(evt)
  if     evt == "exit" then
    sound_play(mission.backsound)
    menu.selectmission()
  elseif evt == "ok"   then
    sound_play(mission.selectsound)
    if menu.sindex == 1 then
      game_changestate("game", menu.arg)
    elseif menu.sindex == 2 then
      game_changestate("password", menu.arg)
    elseif menu.sindex ==  3 then
      menu.selectmission()
    elseif menu.sindex == 4 then
      game_changestate("manual", menu.arg)
   elseif menu.sindex ==  5 then
      game_changestate("halloffame", menu.arg)
    elseif menu.sindex ==  6 then
      game_changestate("about", menu.arg)
    elseif menu.sindex ==  7 then
      game_quit()
    end
  elseif evt == "down" then
    sound_play(mission.movesound)
    if menu.sindex < #menu.items then
      menu.sindex = menu.sindex + 1
    else
      menu.sindex = 1
    end
  elseif evt == "up"   then
    sound_play(mission.movesound)
    if menu.sindex > 1 then
      menu.sindex = menu.sindex - 1
    else
      menu.sindex = #menu.items
    end
  end
end

function menu.initial.render()
  opengl_color3(1, 1, 1)
  background_renderwithsizehints(menu.bgid, menu.bw, menu.bh)
  opengl_color3(1, 1, 0)
  local w, h, d = settings_getscreendimensions()
  local x = 10
  local dh = 32
  local y = h - dh
  text_printortho(x, y, "Mission: " .. menu.arg, 1)
  y = y - dh
  text_printortho(x, y, "Select Your Action", 0)
  y = y - dh
  text_printortho(x, y, "\127\127\127\127\127\127\127\127\127\127\127\127\127\127\127\127\127\127", 0)

  y = y - 2*dh
  local i = 1
  for k, v in ipairs(menu.items) do
    if i == menu.sindex then
      opengl_color3(menu.cr, 0, 0)
      menu.cr = menu.cr + menu.crvel
      if menu.cr <= 0.2 or menu.cr >= 1 then menu.crvel = -menu.crvel end
    else
      opengl_color3(1, 1, 1)
    end
    text_printortho(x, y, v, 0)
    y = y - dh
    i = i + 1
  end
  opengl_color3(1, 1, 1)
end