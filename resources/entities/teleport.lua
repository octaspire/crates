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
teleport = {}

function teleport.register()
  game_entityregistersymbolforname("TP", "teleport")
end

function teleport.parsetarget(s)
  --x, y
  local result = {}
  local i = 1
  for w in string.gmatch(s, ",?%d+") do
    result[i] = tonumber(w) * 2
    i = i + 1
  end
  if (i ~= 3) then error("Target string for teleport must contain exactly 2 coordinates. Given string \"" .. s .. "\" has syntax error.") end
  return result
end

-------------------------------------------------------------------------------
teleport.initial = {}

function teleport.initial.init(id, attr)
  entity_setrenderable(id, true)
  entity_setcollidable(id, true)
  entity_setmesh(id, "cube.lua")
  entity_settexture(id, "teleport.png")
  attribute_set(id, "target", teleport.parsetarget(attr))
  if teleport.activesound == nil then teleport.activesound = sound_nametoid("scifi.wav") end
end

function teleport.initial.update(id, tpf)
end

function teleport.initial.collision(id, oid)
  if entity_gettypeof(oid) == "moving" then
    local side = entity_onwhichsideisother(id, oid)
    local x, y, z = entity_getlocation(id)
    local vx, vy, vz = entity_getvelocity(oid)
    local target = attribute_get(id, "target")
    local tx = target[1]
    local tz = target[2]
    if     side == "north" then
      if vz > 0 then
        if entity_iswalkable(level_getentityidat(tx, y, tz+2)) then
          entity_setlocation(player.id, x, y, z)
          player.releasefollowers()
          sound_play(teleport.activesound)
          tz = tz + 2
          entity_setlocation(player.id, tx, y, tz)
        else
          block.initial.collision(id, oid)
        end
        return
      end
    elseif side == "south" then
      if vz < 0 then
        if entity_iswalkable(level_getentityidat(tx, y, tz-2)) then
          entity_setlocation(player.id, x, y, z)
          player.releasefollowers()
          sound_play(teleport.activesound)
          tz = tz - 2
          entity_setlocation(player.id, tx, y, tz)
        else
          block.initial.collision(id, oid)
        end
        return
      end
    elseif side == "east"  then
      if vx < 0 then
        if entity_iswalkable(level_getentityidat(tx-2, y, tz)) then
          entity_setlocation(player.id, x, y, z)
          player.releasefollowers()
          sound_play(teleport.activesound)
          tx = tx - 2
          entity_setlocation(player.id, tx, y, tz)
        else
          block.initial.collision(id, oid)
        end
        return
      end
    elseif side == "west"  then
      if vx > 0 then
        if entity_iswalkable(level_getentityidat(tx+2, y, tz)) then
          entity_setlocation(player.id, x, y, z)
          player.releasefollowers()
          sound_play(teleport.activesound)
          tx = tx + 2
          entity_setlocation(player.id, tx, y, tz)
        else
          block.initial.collision(id, oid)
        end
        return
      end
    end
  end
end

function teleport.initial.event(id, evt)
end

function teleport.initial.render(id)
end
