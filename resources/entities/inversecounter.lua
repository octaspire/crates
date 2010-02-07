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
inversecounter = {}

function inversecounter.register()
  game_entityregistersymbolforname("!#", "inversecounter")
end

-------------------------------------------------------------------------------
inversecounter.initial = {}

function inversecounter.initial.init(id, attr)
  entity_setrenderable(id, true)
  entity_setpostrenderable(id, true)
  entity_setcollidable(id, true)
  entity_setmesh(id, "cube.lua")
  entity_settexture(id, "inversecounter.png")
  attribute_set(id, "count", tonumber(attr) or 10)
  if counter.activesound == nil then counter.activesound = sound_nametoid("tick.wav") end
end

function inversecounter.initial.update(id, tpf)
end

function inversecounter.initial.collision(id, oid)
  local name = entity_getname(oid)
  if name == "player" then
    local side = entity_onwhichsideisother(id, oid)
    local x, y, z = entity_getlocation(id)
    local vx, vy, vz = entity_getvelocity(oid)
    if     side == "north" then
      if vz > 0 then
        sound_play(counter.activesound)
        entity_setstate(id, "contact")
        entity_setupdatable(id, true)
      end
    elseif side == "south" then
      if vz < 0 then
        sound_play(counter.activesound)
        entity_setstate(id, "contact")
        entity_setupdatable(id, true)
      end
    elseif side == "east"  then
      if vx < 0 then
        sound_play(counter.activesound)
        entity_setstate(id, "contact")
        entity_setupdatable(id, true)
      end
    elseif side == "west"  then
      if vx > 0 then
        sound_play(counter.activesound)
        entity_setstate(id, "contact")
        entity_setupdatable(id, true)
      end
    end
  elseif string.match(name, "puller") then
    if player.followerid then
      entity_setstate(player.followerid, "initial")
      entity_setupdatable(player.followerid, false)
      resetpos(id, player.followerid)
      player.followerid = nil
    end
  end
end

function resetpos(id, oid)
  local side = entity_onwhichsideisother(id, oid)
  local x, y, z = entity_getlocation(id)
  local vx, vy, vz = entity_getvelocity(player.id)
  if     side == "north" then
    if vz > 0 then
      entity_setvelocity(oid, 0, 0, 0)
      z = z - 2
      entity_setlocation(oid, x, y, z)
    end
  elseif side == "south" then
    if vz < 0 then
      entity_setvelocity(oid, 0, 0, 0)
      z = z + 2
      entity_setlocation(oid, x, y, z)
    end
  elseif side == "east"  then
    if vx < 0 then
      entity_setvelocity(oid, 0, 0, 0)
      x = x + 2
      entity_setlocation(oid, x, y, z)
    end
  elseif side == "west"  then
    if vx > 0 then
      entity_setvelocity(oid, 0, 0, 0)
      x = x - 2
      entity_setlocation(oid, x, y, z)
    end
  end
end

function inversecounter.initial.event(id, evt)
end

function inversecounter.initial.render(id)
  local x, y, z = entity_getlocation(id)
  opengl_rotate(-90, 1, 0, 0)
  opengl_translate(-1, -0.2, 1.03)
  opengl_scale(0.033, 0.033, 0.033)
  text_printperspective(tostring(attribute_get(id, "count")), 0)
end

-------------------------------------------------------------------------------
inversecounter.contact = {}

function inversecounter.contact.update(id, tpf)
  local tx, ty, tz = entity_getlocation(id)
  local px, py, pz = entity_getlocation(player.id)
  if (not entity_hascollision(id, player.id)) or (entity_isstopped(player.id) and (tx ~= px or ty ~= py or tz ~= pz)) then
    if attribute_get(id, "count") > 1 then
      attribute_dec(id, "count")
      entity_setstate(id, "initial")
      return
    end
    entity_settexture(id, "block.png")
    entity_setstate(id, "final")
    return
  end
end

function inversecounter.contact.collision(id, oid)
  local name = entity_getname(oid)
  if string.match(name, "puller") then
    if player.followerid then
      entity_setstate(player.followerid, "initial")
      entity_setupdatable(player.followerid, false)
      resetpos(id, player.followerid)
      player.followerid = nil
    end
  end
end

function inversecounter.contact.event(id, evt)
end

function inversecounter.contact.render(id)
  local x, y, z = entity_getlocation(id)
  opengl_rotate(-90, 1, 0, 0)
  opengl_translate(-1, -0.2, 1.03)
  opengl_scale(0.033, 0.033, 0.033)
  text_printperspective(tostring(attribute_get(id, "count")), 0)
end

-------------------------------------------------------------------------------
inversecounter.final = {}

function inversecounter.final.update(id, tpf)
end

function inversecounter.final.collision(id, oid)
  block.initial.collision(id, oid)
end

function inversecounter.final.event(id, evt)
end

function inversecounter.final.render(id)
end
