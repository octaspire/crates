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
mission = {}
mission.initial = {}

function mission.nextpage()
  if mission.currentpage < mission.npages then
    mission.currentpage = mission.currentpage + 1
    return true
  end
  sound_play(mission.backsound)
  return false
end

function mission.prevpage()
  if mission.currentpage > 1 then
    mission.currentpage = mission.currentpage - 1
    return true
  end
  sound_play(mission.backsound)
  return false
end

-------------------------------------------------------------------------------
function mission.initial.init(arg)
  mission.names = {}
  mission.sindex = 1
  mission.cr = 1
  mission.crvel = -0.08
  mission.movesound = sound_nametoid("tick.wav")
  mission.selectsound = sound_nametoid("select.wav")
  mission.backsound = sound_nametoid("back.wav")
  mission.bw = 512
  mission.bh = 256
  mission.bgid = texture_nametoid("bg" .. tostring(mission.bw) .. "x" .. tostring(mission.bh) .. ".png")
  local dsep = dir_getdirectoryseparator()
  mission.helptext = "New Missions Can Be Dropped Into \"resources" .. dsep .. "missions" .. dsep .. "\"";
  mission.underline = string.rep("\127", #mission.helptext)
  mission.pagesize = 25
  mission.currentpage = 1

  state_seteventable(true)
  state_setrenderable(true)

  mission.names = game_getmissionnames()
  mission.npages = math.ceil(#mission.names / mission.pagesize)
  mission.pagestart = mission.pagesize * (mission.currentpage-1)
  mission.pageend   = mission.pagestart + mission.pagesize
  music_playrandom()
end

function mission.initial.update(tpf)
end

function mission.initial.event(evt)
  if     evt == "exit" then
    sound_play(mission.backsound)
    game_quit()
  elseif evt == "ok"   then
    sound_play(mission.selectsound)
    mission_reset()
    game_changestate("menu", mission.names[mission.sindex])
  elseif evt == "down" then
    if mission.sindex < #mission.names then
      sound_play(mission.movesound)
      mission.sindex = mission.sindex + 1
      if mission.sindex > mission.pageend then mission.nextpage() end
    else
      mission.sindex = 1
      mission.currentpage = 1
    end
  elseif evt == "up"   then
    if mission.sindex > 1 then
      sound_play(mission.movesound)
      mission.sindex = mission.sindex - 1
      if mission.sindex < mission.pagestart then mission.prevpage() end
    else
      mission.sindex = #mission.names
      mission.currentpage = mission.npages
    end
  elseif evt == "right" then
    if mission.nextpage() then
      mission.sindex = mission.sindex + mission.pagesize
      if (mission.sindex > #mission.names) then mission.sindex = #mission.names end
    end
  elseif evt == "left"  then
    if mission.prevpage() then
      mission.sindex = mission.sindex - mission.pagesize
      if (mission.sindex < 1) then mission.sindex = 1 end
    end
  end
end

function mission.initial.render()
  opengl_color3(1, 1, 1)
  background_renderwithsizehints(mission.bgid, mission.bw, mission.bh)
  opengl_color3(1, 1, 0)
  local w, h, d = settings_getscreendimensions()
  local x = 10
  local dh = 16
  local y = h - dh
  if (mission.npages > 1) then
    if     mission.currentpage <= 1 then
      text_printortho(x, y, "Select Mission (page "  .. mission.currentpage .. "/" .. mission.npages .. "\128)" , 1)
    elseif mission.currentpage >= mission.npages then
      text_printortho(x, y, "Select Mission (\130page "  .. mission.currentpage .. "/" .. mission.npages .. ")", 1)
    else
      text_printortho(x, y, "Select Mission (\130page "  .. mission.currentpage .. "/" .. mission.npages .. "\128)", 1)
    end
  else
    text_printortho(x, y, "Select Mission" , 1)
  end
  y = y - dh
  text_printortho(x, y, mission.helptext, 0)
  y = y - dh
  text_printortho(x, y, mission.underline, 0)

  y = y - 2*dh
  local i = 1
  mission.pagestart = 1 + (mission.pagesize * (mission.currentpage-1))
  mission.pageend   = mission.pagestart + (mission.pagesize-1)
  for k, v in ipairs(mission.names) do
    if i == mission.sindex then
      opengl_color3(mission.cr, 0, 0)
      mission.cr = mission.cr + mission.crvel
      if mission.cr <= 0.2 or mission.cr >= 1 then mission.crvel = -mission.crvel end
    else
      opengl_color3(1, 1, 1)
    end
    if i >= mission.pagestart and i <= mission.pageend then
      text_printortho(x, y, v, 0)
      y = y - dh
    end
    i = i + 1
  end
  opengl_color3(1, 1, 1)
end
