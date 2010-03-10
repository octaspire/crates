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
passiveenemy = {}

function passiveenemy.register()
  game_entityregistersymbolforname("PE", "passiveenemy")
end

function passiveenemy.restartlevel(id)
  local px, py, pz = entity_getlocation(player.id)
  entity_setlocation(id, px, py, pz)
  sound_play(exit.errorsound)
  game.restartlevel()
end

function passiveenemy.storeattr(id, s)
  --delay, commands
  local delay = 1
  local commands = ""
  local i = 1
  for w in string.gmatch(s, ",?[%w%.]+") do
    if     i == 1 then delay    = tonumber(w)
    elseif i == 2 then commands = w end
    i = i + 1
  end
  if (i ~= 3) then error("Attribute string for passiveenemy must contain exactly 2 values (delay, commands). Given string \"" .. s .. "\" has syntax error.") end
  attribute_set(id, "delay",    delay)
  attribute_set(id, "commands", commands)
end

-------------------------------------------------------------------------------
passiveenemy.initial = {}

function passiveenemy.initial.init(id, attr)
  entity_setrenderable(id, true)
  entity_setupdatable(id, true)
  entity_setcollidable(id, true)
  entity_setmovable(id, true)
  entity_setmesh(id, "cube.lua")
  entity_settexture(id, "passiveenemy.png")
  attribute_set(id, "cindex",   1)
  passiveenemy.storeattr(id, attr)
  attribute_set(id, "tpf", 0)
  if block.collisionsound == nil then block.collisionsound = sound_nametoid("thump.wav") end
  if exit.errorsound == nil then exit.errorsound = sound_nametoid("back.wav") end
end

function passiveenemy.initial.update(id, tpf)
  local x, y, z = entity_getlocation(id)
  local delay   = attribute_get(id, "delay")
  attribute_inc(id, "tpf", tpf)
  if attribute_get(id, "tpf") >= delay then
    -- TODO handling of multiple seconds passed? Not probably, though.
    attribute_set(id, "tpf", (attribute_get(id, "tpf") - delay))
    local px, py, pz = entity_getlocation(player.id)
    local cindex   = attribute_get(id, "cindex")
    local commands = attribute_get(id, "commands")
    local command  = string.sub(commands, cindex, cindex)
    cindex = cindex + 1
    if cindex > #commands then cindex = 1 end
    attribute_set(id, "cindex", cindex)

    if     command == "E" then
      x = x + 2
    elseif command == "N" then
      z = z - 2
    elseif command == "W" then
      x = x - 2
    elseif command == "S" then
      z = z + 2
    end

    local oid = level_getentityidat(x, y, z)
    if     entity_iswalkable(oid) then
      entity_setlocation(id, x, y, z)
    elseif oid == player.id then
      passiveenemy.restartlevel(id)
    end
    if entity_getdistancesquared(id, player.id) < 4 then passiveenemy.restartlevel(id) end
  end
end

function passiveenemy.initial.collision(id, oid)
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
        if entity_isname(oid, "player") then passiveenemy.restartlevel(id) end
      end
    elseif side == "south" then
      if vz < 0 then
        sound_play(block.collisionsound)
        entity_setvelocity(oid, 0, 0, 0)
        z = z + 2
        entity_setlocation(oid, x, y, z)
        if entity_isname(oid, "player") then passiveenemy.restartlevel(id) end
      end
    elseif side == "east"  then
      if vx < 0 then
        sound_play(block.collisionsound)
        entity_setvelocity(oid, 0, 0, 0)
        x = x + 2
        entity_setlocation(oid, x, y, z)
        if entity_isname(oid, "player") then passiveenemy.restartlevel(id) end
      end
    elseif side == "west"  then
      if vx > 0 then
        sound_play(block.collisionsound)
        entity_setvelocity(oid, 0, 0, 0)
        x = x - 2
        entity_setlocation(oid, x, y, z)
        if entity_isname(oid, "player") then passiveenemy.restartlevel(id) end
      end
    end
  end
end

function passiveenemy.initial.event(id, evt)
end

function passiveenemy.initial.render(id)
end
