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
ambushenemy = {}

function ambushenemy.register()
  game_entityregistersymbolforname("ME", "ambushenemy")
end

function ambushenemy.restartlevel(id)
  local px, py, pz = entity_getlocation(player.id)
  entity_setlocation(id, px, py, pz)
  sound_play(exit.errorsound)
  game.restartlevel()
end

function ambushenemy.storeattr(id, s)
  --startdistance, stopdistance, delay
  local startdistancesquared = 16
  local stopdistancesquared  = 100
  local delay = 1
  local i = 1
  for w in string.gmatch(s, ",?[%d%.]+") do
    if     i == 1 then startdistance = tonumber(w) * 2; startdistancesquared = startdistance ^ 2
    elseif i == 2 then stopdistance  = tonumber(w) * 2; stopdistancesquared  = stopdistance  ^ 2
    elseif i == 3 then delay         = tonumber(w)           end
    i = i + 1
  end
  if (i ~= 4) then error("Attribute string for ambushenemy must contain exactly 3 values (startdistance, stopdistance, delay). Given string \"" .. s .. "\" has syntax error.") end
  attribute_set(id, "startdistance",        startdistance)
  attribute_set(id, "stopdistance",         stopdistance)
  attribute_set(id, "startdistancesquared", startdistancesquared)
  attribute_set(id, "stopdistancesquared",  stopdistancesquared)
  attribute_set(id, "delay",                delay)
end

-------------------------------------------------------------------------------
ambushenemy.initial = {}

function ambushenemy.initial.init(id, attr)
  entity_setrenderable(id, true)
  entity_setupdatable(id, true)
  entity_setcollidable(id, true)
  entity_setmovable(id, true)
  entity_setpostrenderable(id, true)
  entity_setmesh(id, "cube.lua")
  entity_settexture(id, "ambushenemy.png")
  ambushenemy.storeattr(id, attr)
  attribute_set(id, "tpf", 0)
  attribute_set(id, "hunting", false)
  if block.collisionsound == nil then block.collisionsound = sound_nametoid("thump.wav") end
  if exit.errorsound == nil then exit.errorsound = sound_nametoid("back.wav") end
  if steelbomb.explosionid == nil then steelbomb.explosionid = texture_nametoid("explosion.png") end
  if steelbomb.quadid == nil then steelbomb.quadid = opengl_newquadric() end
end

function ambushenemy.initial.update(id, tpf)
  local x, y, z = entity_getlocation(id)
  local distsquaredtoplayer = entity_getdistancesquared(id, player.id)
  local ishunting = attribute_get(id, "hunting")
  if ishunting then
    if distsquaredtoplayer > attribute_get(id, "stopdistancesquared") then
      attribute_set(id, "hunting", false)
      return
    end
  end

  if (not ishunting) then
    if distsquaredtoplayer > attribute_get(id, "startdistancesquared") then return end
    attribute_set(id, "hunting", true)
  end

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
      ambushenemy.restartlevel(id)
    end
    if distsquaredtoplayer < 4 then ambushenemy.restartlevel(id) end
  end
end

function ambushenemy.initial.collision(id, oid)
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
        if entity_isname(oid, "player") then ambushenemy.restartlevel(id) end
      end
    elseif side == "south" then
      if vz < 0 then
        sound_play(block.collisionsound)
        entity_setvelocity(oid, 0, 0, 0)
        z = z + 2
        entity_setlocation(oid, x, y, z)
        if entity_isname(oid, "player") then ambushenemy.restartlevel(id) end
      end
    elseif side == "east"  then
      if vx < 0 then
        sound_play(block.collisionsound)
        entity_setvelocity(oid, 0, 0, 0)
        x = x + 2
        entity_setlocation(oid, x, y, z)
        if entity_isname(oid, "player") then ambushenemy.restartlevel(id) end
      end
    elseif side == "west"  then
      if vx > 0 then
        sound_play(block.collisionsound)
        entity_setvelocity(oid, 0, 0, 0)
        x = x - 2
        entity_setlocation(oid, x, y, z)
        if entity_isname(oid, "player") then ambushenemy.restartlevel(id) end
      end
    end
  end
end

function ambushenemy.initial.event(id, evt)
end

function ambushenemy.initial.render(id)
  local diameter = 4
  if attribute_get(id, "hunting") then
    diameter = attribute_get(id, "stopdistance")
  else
    diameter = attribute_get(id, "startdistance")
  end
  diameter = diameter + 0.2
  opengl_pushmatrix()
  opengl_rotate(-90, 1, 0, 0)
  opengl_translate(0, 0, -1.03)
  --opengl_disk(steelbomb.quadid, diameter-0.5, diameter, 32, 32)
  opengl_bindtexture2d(steelbomb.explosionid)
  --opengl_disk(steelbomb.quadid, 2, diameter, 32, 32)
  opengl_disk(steelbomb.quadid, diameter-0.16, diameter, 32, 32)
  opengl_popmatrix()
end
