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
password = {}
password.initial = {}
password.items = {"|", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R",
"S", "T", "U", "V", "W", "X", "Y", "Z", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9"}

function password.initial.init(arg)
  password.sindex = 1
  password.pwd = {"|"}
  password.index = 1
  password.cr = 1
  password.crvel = -0.08
  password.arg = tostring(arg)
  password.bw = 512
  password.bh = 256
  password.bgid = texture_nametoid("bg" .. tostring(password.bw) .. "x" .. tostring(password.bh) .. ".png")
  password.maxpwdlen = 50
  if mission.movesound == nil then mission.movesound = sound_nametoid("tick.wav") end
  state_seteventable(true)
  state_setrenderable(true)
end

function password.initial.update(tpf)
end

function password.initial.event(evt)
  if     evt == "exit" then
    game_changestate("menu", password.arg)
  elseif evt == "ok"   then
    local pwd = table.concat(password.pwd)
    pwd = string.gsub(pwd, "|", "")
    if game_haslevelwithpassword(pwd) then
      game_setcurrentlevelfrompassword(pwd)
      opengl_color3(1, 1, 1)
      game_changestate("game", menu.arg)
    else
      -- wrong password
      sound_play(mission.movesound)
    end
  elseif evt == "down" then
    if password.sindex < #password.items then
      password.sindex = password.sindex + 1
    else
      password.sindex = 1
    end
    password.pwd[password.index] = password.items[password.sindex]
  elseif evt == "up"   then
    if password.sindex > 1 then
      password.sindex = password.sindex - 1
    else
      password.sindex = #password.items
    end
    password.pwd[password.index] = password.items[password.sindex]
  elseif evt == "left" or evt == "backspace" then
    if password.index > 1 then
      password.pwd[#password.pwd] = nil
      password.pwd[#password.pwd] = "|"
      password.index = password.index - 1
      for k, v in ipairs(password.items) do
        if v == password.pwd[password.index] then
          password.sindex = k
          break
        end
      end
    end
  elseif evt == "right" then
    if # password.pwd >= password.maxpwdlen then
      sound_play(mission.movesound)
      return
    end
    if password.pwd[#password.pwd] == "|" then
      password.pwd[#password.pwd] = "A"
    end
    password.pwd[#password.pwd + 1] = "|"
    password.index = password.index + 1
    password.sindex = 1
  else
    if # password.pwd >= password.maxpwdlen then
      sound_play(mission.movesound)
      return
    end
    password.pwd[#password.pwd] = evt
    password.pwd[#password.pwd + 1] = "|"
    password.index = password.index + 1
    password.sindex = 1
  end
end

function password.initial.render(tpf)
  opengl_color3(1, 1, 1)
  background_renderwithsizehints(password.bgid, password.bw, password.bh)
  opengl_color3(1, 1, 0)
  local w, h, d = settings_getscreendimensions()
  local x = 10
  local dh = 16
  local y = h - dh
  text_printortho(x, y, "Enter password:", 1)
  y = y - dh
  text_printortho(x, y, "\127\127\127\127\127\127\127\127\127\127\127\127\127\127\127", 0)

  y = y - 2*dh
  local i = 1
  for k, v in ipairs(password.pwd) do
    if i == #password.pwd then
      opengl_color3(password.cr, 0, 0)
      password.cr = password.cr + password.crvel
      if password.cr <= 0.2 or password.cr >= 1 then password.crvel = -password.crvel end
    else
      opengl_color3(1, 1, 1)
    end
    text_printortho(x + (i*8), y, v, 0)
    i = i + 1
  end
  opengl_color3(1, 1, 1)
end
