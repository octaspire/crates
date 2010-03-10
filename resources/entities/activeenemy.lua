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
activeenemy = {}

function activeenemy.register()
  game_entityregistersymbolforname("AE", "activeenemy")
end

function activeenemy.restartlevel(id)
  local px, py, pz = entity_getlocation(player.id)
  entity_setlocation(id, px, py, pz)
  sound_play(exit.errorsound)
  game.restartlevel()
end

-------------------------------------------------------------------------------
activeenemy.initial = {}

function activeenemy.initial.init(id, attr)
  entity_setrenderable(id, true)
  entity_setupdatable(id, true)
  entity_setcollidable(id, true)
  entity_setmovable(id, true)
  entity_setmesh(id, "cube.lua")
  entity_settexture(id, "activeenemy.png")
  attribute_set(id, "delay", tonumber(attr) or 1)
  attribute_set(id, "tpf", 0)
  if block.collisionsound == nil then block.collisionsound = sound_nametoid("thump.wav") end
  if exit.errorsound == nil then exit.errorsound = sound_nametoid("back.wav") end
end

function activeenemy.initial.update(id, tpf)
  local x, y, z = entity_getlocation(id)
  local delay   = attribute_get(id, "delay")
  attribute_inc(id, "tpf", tpf)
  if attribute_get(id, "tpf") >= delay then
    -- TODO handling of multiple seconds passed? Not probably, though.
    attribute_set(id, "tpf", (attribute_get(id, "tpf") - delay))
    local px, py, pz = entity_getlocation(player.id)
    local dx = x - px;
    local dy = y - py;
    local dz = z - pz;
    if dz ~= 0 and dx ~= 0 then
      if math.random(1, 2) == 1 then
        if     dz > 0 then z = z - 2
        elseif dz < 0 then z = z + 2 end
      else
        if     dx > 0 then x = x - 2
        elseif dx < 0 then x = x + 2 end
      end
    else
        if     dz > 0 then z = z - 2
        elseif dz < 0 then z = z + 2 end
        if     dx > 0 then x = x - 2
        elseif dx < 0 then x = x + 2 end
    end

    local oid = level_getentityidat(x, y, z)
    if     oid < 0 then
      entity_setlocation(id, x, y, z)
    elseif oid == player.id then
      activeenemy.restartlevel(id)
    end
    if entity_getdistancesquared(id, player.id) < 4 then activeenemy.restartlevel(id) end
  end
end

function activeenemy.initial.collision(id, oid)
  if entity_gettypeof(oid) == "moving" then
    local side = entity_onwhichsideisother(id, oid)
    local x, y, z = entity_getlocation(id)
    local vx, vy, vz = entity_getvelocity(oid)
    if     side == "north" then
      if vz > 0 then
        sound_play(block.collisionsound)
        entity_setvelocity(oid, 0, 0, 0)
        z = z - 2
        entity_setlocation(oid, x, y, z)
        if entity_isname(oid, "player") then activeenemy.restartlevel(id) end
      end
    elseif side == "south" then
      if vz < 0 then
        sound_play(block.collisionsound)
        entity_setvelocity(oid, 0, 0, 0)
        z = z + 2
        entity_setlocation(oid, x, y, z)
        if entity_isname(oid, "player") then activeenemy.restartlevel(id) end
      end
    elseif side == "east"  then
      if vx < 0 then
        sound_play(block.collisionsound)
        entity_setvelocity(oid, 0, 0, 0)
        x = x + 2
        entity_setlocation(oid, x, y, z)
        if entity_isname(oid, "player") then activeenemy.restartlevel(id) end
      end
    elseif side == "west"  then
      if vx > 0 then
        sound_play(block.collisionsound)
        entity_setvelocity(oid, 0, 0, 0)
        x = x - 2
        entity_setlocation(oid, x, y, z)
        if entity_isname(oid, "player") then activeenemy.restartlevel(id) end
      end
    end
  end
end

function activeenemy.initial.event(id, evt)
end

function activeenemy.initial.render(id)
end
