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

function halloffame.nextpage()
  if halloffame.currentpage < halloffame.npages then
    halloffame.currentpage = halloffame.currentpage + 1
    return true
  end
  sound_play(halloffame.backsound)
  return false
end

function halloffame.prevpage()
  if halloffame.currentpage > 1 then
    halloffame.currentpage = halloffame.currentpage - 1
    return true
  end
  sound_play(halloffame.backsound)
  return false
end

-------------------------------------------------------------------------------
function halloffame.initial.init(arg)
  halloffame.statistics = {}
  if arg ~= nil then halloffame.arg = tostring(arg) else halloffame.arg = nil end
  halloffame.bw = 512
  halloffame.bh = 256
  halloffame.bgid = texture_nametoid("bg" .. tostring(halloffame.bw) .. "x" .. tostring(halloffame.bh) .. ".png")
  halloffame.helptext = "Fastest Players Who Have Passed Mission At One Go";
  halloffame.underline = string.rep("\127", #halloffame.helptext)
  halloffame.pagesize = 25
  halloffame.currentpage = 1
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

  halloffame.npages = math.ceil(#halloffame.statistics / halloffame.pagesize)
  halloffame.pagestart = halloffame.pagesize * (halloffame.currentpage-1)
  halloffame.pageend   = halloffame.pagestart + halloffame.pagesize

  halloffame.backsound = sound_nametoid("back.wav")
  halloffame.movesound = sound_nametoid("tick.wav")
end

function halloffame.initial.update(tpf)
end

function halloffame.initial.event(evt)
  if evt == "exit" or evt == "ok" then
    if halloffame.arg ~= nil and halloffame.arg ~= "" then
      game_changestate("menu", halloffame.arg)
    else
      game_changestate("mission", "")
    end
  elseif evt == "right" or evt == "down" then
    halloffame.nextpage()
  elseif evt == "left"  or evt == "up"   then
    halloffame.prevpage()
  end
end

function halloffame.initial.render()
  opengl_color3(1, 1, 1)
  background_renderwithsizehints(halloffame.bgid, halloffame.bw, halloffame.bh)
  opengl_color3(1, 1, 0)
  local w, h, d = settings_getscreendimensions()
  local x = 10
  local dh = 16
  local y = h - dh
  if (halloffame.npages > 1) then
    if     halloffame.currentpage <= 1 then
      text_printortho(x, y, "Hall Of Fame (page "  .. halloffame.currentpage .. "/" .. halloffame.npages .. "\128)" , 1)
    elseif halloffame.currentpage >= halloffame.npages then
      text_printortho(x, y, "Hall Of Fame (\130page "  .. halloffame.currentpage .. "/" .. halloffame.npages .. ")", 1)
    else
      text_printortho(x, y, "Hall Of Fame (\130page "  .. halloffame.currentpage .. "/" .. halloffame.npages .. "\128)", 1)
    end
  else
    text_printortho(x, y, "Hall Of Fame" , 1)
  end
  y = y - dh
  text_printortho(x, y, halloffame.helptext, 0)
  y = y - dh
  text_printortho(x, y, halloffame.underline, 0)

  y = y - 2*dh
  opengl_color3(1, 1, 1)
  local i = 1
  halloffame.pagestart = 1 + (halloffame.pagesize * (halloffame.currentpage-1))
  halloffame.pageend   = halloffame.pagestart + (halloffame.pagesize-1)
  for k, v in ipairs(halloffame.statistics) do
    if i >= halloffame.pagestart and i <= halloffame.pageend then
      text_printortho(x, y, halloffame.item[k], 0)
      y = y - dh
    end
    i = i + 1
  end
end