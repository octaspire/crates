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
halloffame = {}
halloffame.initial = {}

function halloffame.sec2human(sec)
  local m = 60
  if (sec >= m) then
    local tmp = sec/m
    return (tmp-(tmp%1)) .. " minutes " .. (sec % m) .. " seconds"
  else
    return sec .. " seconds "
  end
end

function halloffame.initial.init(arg)
  halloffame.statistics = {}
  if arg ~= nil then halloffame.arg = tostring(arg) else halloffame.arg = nil end
  halloffame.bw = 512
  halloffame.bh = 256
  halloffame.bgid = texture_nametoid("bg" .. tostring(halloffame.bw) .. "x" .. tostring(halloffame.bh) .. ".png")
  halloffame.helptext = "Players Who Passed Mission At One Go";
  halloffame.underline = string.rep("\127", #halloffame.helptext)
  halloffame.statistics = halloffame_getstatistics()
  table.sort(halloffame.statistics, function (a, b)
    return a.seconds < b.seconds
  end)
  halloffame.longestname = 0
  for k, v in ipairs(halloffame.statistics) do
    if #v.name >  halloffame.longestname then  halloffame.longestname = #v.name + 1 end
  end
  halloffame.spaces = {}
  for k, v in ipairs(halloffame.statistics) do
    halloffame.spaces[k] = string.rep(" ", halloffame.longestname -  #v.name)
  end
  halloffame.item = {}
  for k, v in ipairs(halloffame.statistics) do
    halloffame.item[k] = k .. ". " .. v.name .. ":" .. (halloffame.spaces[k] or " ") .. halloffame.sec2human(v.seconds)
  end
  state_seteventable(true)
  state_setrenderable(true)
end

function halloffame.initial.update(tpf)
end

function halloffame.initial.event(evt)
  if halloffame.arg ~= nil and halloffame.arg ~= "" then
    game_changestate("menu", halloffame.arg)
  else
    game_changestate("mission", "")
  end
end

function halloffame.initial.render()
  opengl_color3(1, 1, 1)
  background_renderwithsizehints(halloffame.bgid, halloffame.bw, halloffame.bh)
  opengl_color3(1, 1, 0)
  local w, h, d = settings_getscreendimensions()
  local x = 10
  local dh = 32
  local y = h - dh
  text_printortho(x, y, "Hall Of Fame", 1)
  y = y - dh
  text_printortho(x, y, halloffame.helptext, 0)
  y = y - dh
  text_printortho(x, y, halloffame.underline, 0)

  y = y - 2*dh
  opengl_color3(1, 1, 1)
  for k, v in ipairs(halloffame.statistics) do
    text_printortho(x, y, halloffame.item[k], 0)
    y = y - dh
  end
end