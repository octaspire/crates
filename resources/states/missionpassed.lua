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
missionpassed = {}
missionpassed.initial = {}

function missionpassed.initial.init(arg)
  missionpassed.text = "Congrulations! Mission \"" .. arg .. "\" Passed!"
  missionpassed.underline = string.rep("\127", #missionpassed.text)
  missionpassed.bw = 1024
  missionpassed.bh = 512
  missionpassed.bgid = texture_nametoid("bg" .. tostring(mission.bw) .. "x" .. tostring(mission.bh) .. ".png")

  state_seteventable(true)
  state_setrenderable(true)
end

function missionpassed.initial.update(tpf)
end

function missionpassed.initial.event(evt)
  game_changestate("mission", "")
end

function missionpassed.initial.render()
  opengl_color3(1, 1, 1)
  background_renderwithsizehints(missionpassed.bgid, missionpassed.bw, missionpassed.bh)
  opengl_color3(1, 1, 0)
  local w, h, d = settings_getscreendimensions()
  local x = 10
  local dh = 32
  local y = h - dh
  text_printortho(x, y,  missionpassed.text, 0)
  y = y - dh
  text_printortho(x, y,  missionpassed.underline, 0)
  opengl_color3(1, 1, 1)
end
