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

function manual.initial.init(arg)
  manual.arg = tostring(arg)
  manual.bw = 1024
  manual.bh = 1024
  manual.sw, manual.sh = settings_getscreendimensions()
  manual.x = 0;
  if manual.bh > manual.sh then
    manual.y = manual.sh - manual.bh
  else
    manual.y = 0
  end
  manual.bgid = texture_nametoid("manual" .. tostring(manual.bw) .. "x" .. tostring(manual.bh) .. ".png")
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

  local step = 50
  local scrollx = manual.bw > manual.sw
  local scrolly = manual.bh > manual.sh

  if evt == "down"  and scrolly and manual.y <= -step then
    manual.y = manual.y + step
  elseif evt == "up"  and scrolly and manual.y > (manual.sh - manual.bh) then
    manual.y = manual.y - step
  elseif evt == "right" and scrollx and manual.x > (manual.sw - manual.bw) then
    manual.x = manual.x - step
  elseif evt == "left" and scrollx and manual.x <= -step then
    manual.x = manual.x + step
  end
end

function manual.initial.render()
  opengl_color3(1, 1, 1)
  background_renderwithsizes(manual.bgid, manual.x, manual.y, manual.bw, manual.bh)
  opengl_color3(1, 1, 0)
end
