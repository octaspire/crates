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
switch = {}

function switch.register()
  game_entityregistersymbolforname("SW", "switch")
end

function switch.setstatusoftarget(id, status)
  local x, ty, z = entity_getlocation(id)
  local target = attribute_get(id, "target")
  local tx = target[1]
  local tz = target[2]

  if status then
    local targetid =  attribute_get(id, "targetid")
    if targetid > -1 then
      entity_setalive(targetid, status)
    end
    attribute_set(id, "targetid", -1)
  else
    local targetid = level_getentityidat(tx, ty, tz)
    if targetid > -1 then
      entity_setalive(targetid, status)
    end
    attribute_set(id, "targetid", targetid)
  end
end

function switch.storeattr(id, s)
  --delay, target.x, target.y
  local delay = 1
  local target = {}
  local i = 1
  for w in string.gmatch(s, ",?[%w%.]+") do
    if     i == 1 then delay        = tonumber(w)
    else               target[i-1]  = tonumber(w) * 2 end
    i = i + 1
  end
  if (i ~= 4) then error("Attribute string for switch must contain exactly 3 values (delay, target.x, target.y). Given string \"" .. s .. "\" has syntax error.") end
  attribute_set(id, "delay",  delay)
  attribute_set(id, "target", target)
end

-------------------------------------------------------------------------------
switch.initial = {}

function switch.initial.init(id, attr)
  entity_setrenderable(id, true)
  entity_setcollidable(id, true)
  entity_setmovable(id, false)
  entity_setupdatable(id, false)
  entity_setmesh(id, "bottom.lua")
  entity_settexture(id, "switch.png")
  switch.storeattr(id, attr)
  attribute_set(id, "depressed", false)
  attribute_set(id, "targetid", -1)
  attribute_set(id, "tpf", 0)
  if switch.statussound == nil then switch.statussound = sound_nametoid("select.wav") end
end

function switch.initial.update(id, tpf)
  local tmp   = attribute_get(id, "tpf")
  local delay = attribute_get(id, "delay")
  tmp = tmp + tpf
  if tmp >= delay then
    tmp = 0
    attribute_set(id, "depressed", false)
    entity_setupdatable(id, false)
    sound_play(switch.statussound)
    switch.setstatusoftarget(id, true)
  end
  attribute_set(id, "tpf", tmp)
end

function switch.initial.collision(id, oid)
  if entity_getdistancesquared(id, oid) >= 3.9 then return end
  if attribute_get(id, "depressed") then attribute_set(id, "tpf", 0); return end
  attribute_set(id, "depressed", true)
  sound_play(switch.statussound)
  switch.setstatusoftarget(id, false)
  entity_setupdatable(id, true)
end

function switch.initial.event(id, evt)
end

function switch.initial.render(id)
end
