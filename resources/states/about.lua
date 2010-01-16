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
about = {}
about.initial = {}

function about.initial.init(arg)
  about.lines = {}
  about.arg = tostring(arg)
  about.bw = 1024
  about.bh = 1024
  about.bgid = texture_nametoid("bg" .. tostring(about.bw) .. "x" .. tostring(about.bh) .. ".png")

  state_seteventable(true)
  state_setrenderable(true)

  about.title     = "About Crates"
  about.subtitle  = "This is " .. game_getreleasestr() .. ""
  about.underline = string.rep("\127", #about.subtitle)

  about.sw, about.sh = settings_getscreendimensions()
  about.creditsid = texture_nametoid("credits.png")
end

function about.initial.update(tpf)
end

function about.initial.event(evt)
  if     evt == "exit" then
    sound_play(mission.backsound)
    game_changestate("menu", about.arg)
  elseif evt == "ok"   then
    sound_play(mission.selectsound)
    game_changestate("menu", about.arg)
  end
end

function about.initial.render()
  opengl_color3(1, 1, 1)
  background_renderwithsizehints(about.bgid, about.bw, about.bh)
  background_renderwithsizes(about.creditsid, 20, about.sh-600, 512, 512)
  opengl_color3(1, 1, 0)
  local w, h, d = settings_getscreendimensions()
  local x = 10
  local dh = 32
  local y = h - dh
  text_printortho(x, y, about.title, 1)
  y = y - dh
  text_printortho(x, y, about.subtitle, 0)
  y = y - dh
  text_printortho(x, y, about.underline, 0)
end