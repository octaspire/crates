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
missionpassedat1go = {}
missionpassedat1go.initial = {}
missionpassedat1go.items = {"|", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R",
"S", "T", "U", "V", "W", "X", "Y", "Z", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9"}

function missionpassedat1go.initial.init(arg)
  missionpassedat1go.text  = "Mission \"" .. arg .. "\" Passed At 1 Go!"
  missionpassedat1go.text2 = "Enter Your Name For The Hall Of Fame:"
  missionpassedat1go.underline = string.rep("\127", #missionpassedat1go.text2)
  missionpassedat1go.bw = 512
  missionpassedat1go.bh = 256
  missionpassedat1go.bgid = texture_nametoid("bg" .. tostring(missionpassedat1go.bw) .. "x" .. tostring(missionpassedat1go.bh) .. ".png")
  missionpassedat1go.sindex = 1
  missionpassedat1go.heroname = {"|"}
  missionpassedat1go.index = 1
  missionpassedat1go.cr = 1
  missionpassedat1go.crvel = -0.08
  missionpassedat1go.arg = tostring(arg)
  missionpassedat1go.maxheronamelen = 11
  if mission.movesound == nil then mission.movesound = sound_nametoid("tick.wav") end
  state_seteventable(true)
  state_setrenderable(true)
end

function missionpassedat1go.initial.update(tpf)
end

function missionpassedat1go.initial.event(evt)
  if     evt == "exit" then
     game_changestate("mission", "")
  elseif evt == "ok"   then
    local heroname = table.concat(missionpassedat1go.heroname)
    heroname = string.gsub(heroname, "|", "")
    halloffame_add(heroname)
    game_changestate("halloffame", "")
  elseif evt == "down" then
    if missionpassedat1go.sindex < #missionpassedat1go.items then
      missionpassedat1go.sindex = missionpassedat1go.sindex + 1
    else
      missionpassedat1go.sindex = 1
    end
    missionpassedat1go.heroname[missionpassedat1go.index] = missionpassedat1go.items[missionpassedat1go.sindex]
  elseif evt == "up"   then
    if missionpassedat1go.sindex > 1 then
      missionpassedat1go.sindex = missionpassedat1go.sindex - 1
    else
      missionpassedat1go.sindex = #missionpassedat1go.items
    end
    missionpassedat1go.heroname[missionpassedat1go.index] = missionpassedat1go.items[missionpassedat1go.sindex]
  elseif evt == "left" or evt == "backspace" then
    if missionpassedat1go.index > 1 then
      missionpassedat1go.heroname[#missionpassedat1go.heroname] = nil
      missionpassedat1go.heroname[#missionpassedat1go.heroname] = "|"
      missionpassedat1go.index = missionpassedat1go.index - 1
      for k, v in ipairs(missionpassedat1go.items) do
        if v == missionpassedat1go.heroname[missionpassedat1go.index] then
          missionpassedat1go.sindex = k
          break
        end
      end
    end
  elseif evt == "right" then
    if #missionpassedat1go.heroname >= missionpassedat1go.maxheronamelen then
      sound_play(mission.movesound)
      return
    end
    if missionpassedat1go.heroname[#missionpassedat1go.heroname] == "|" then
      missionpassedat1go.heroname[#missionpassedat1go.heroname] = "A"
    end
    missionpassedat1go.heroname[#missionpassedat1go.heroname + 1] = "|"
    missionpassedat1go.index = missionpassedat1go.index + 1
    missionpassedat1go.sindex = 1
  else
    if #missionpassedat1go.heroname >= missionpassedat1go.maxheronamelen then
      sound_play(mission.movesound)
      return
    end
    missionpassedat1go.heroname[#missionpassedat1go.heroname] = evt
    missionpassedat1go.heroname[#missionpassedat1go.heroname + 1] = "|"
    missionpassedat1go.index = missionpassedat1go.index + 1
    missionpassedat1go.sindex = 1
  end
end

function missionpassedat1go.initial.render()
  opengl_color3(1, 1, 1)
  background_renderwithsizehints(missionpassedat1go.bgid, missionpassedat1go.bw, missionpassedat1go.bh)
  opengl_color3(1, 1, 0)
  local w, h, d = settings_getscreendimensions()
  local x = 10
  local dh = 16
  local y = h - dh
  text_printortho(x, y,  missionpassedat1go.text, 0)
  y = y - dh
  text_printortho(x, y,  missionpassedat1go.text2, 0)
  y = y - dh
  text_printortho(x, y,  missionpassedat1go.underline, 0)

  y = y - 2*dh
  local i = 1
  for k, v in ipairs(missionpassedat1go.heroname) do
    if i == #missionpassedat1go.heroname then
      opengl_color3(missionpassedat1go.cr, 0, 0)
      missionpassedat1go.cr = missionpassedat1go.cr + missionpassedat1go.crvel
      if missionpassedat1go.cr <= 0.2 or missionpassedat1go.cr >= 1 then missionpassedat1go.crvel = -missionpassedat1go.crvel end
    else
      opengl_color3(1, 1, 1)
    end
    text_printortho(x + (i*8), y, v, 0)
    i = i + 1
  end
  opengl_color3(1, 1, 1)
end
