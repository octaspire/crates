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
pusherensw = {}

function pusherensw.register()
  game_entityregistersymbolforname("P+", "pusherensw")
end

-------------------------------------------------------------------------------
pusherensw.initial = {}

function pusherensw.initial.init(id, attr)
  entity_setrenderable(id, true)
  entity_setcollidable(id, true)
  entity_setmesh(id, "player.lua")
  entity_settexture(id, "pusherensw.png")
  attribute_set(id, "typeof", "moving")
  if pullere.activatesound == nil then pullere.activatesound = sound_nametoid("buzz.wav") end
end

function pusherensw.initial.update(id, tpf)
end

function pusherensw.initial.collision(id, oid)
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
        if (level_getentityidat(x, y, z + 4) == -1) then
          entity_setstate(id, "actives")
          entity_setvelocity(id, vx, vy, vz)
          entity_setmovable(id, true)
          entity_setcollidable(id, true)
          entity_setupdatable(id, true)
        end
      end
    elseif side == "south" then
      if vz < 0 then
        sound_play(block.collisionsound)
        entity_setvelocity(oid, 0, 0, 0)
        z = z + 2
        entity_setlocation(oid, x, y, z)
        if (level_getentityidat(x, y, z - 4) == -1) then
          entity_setstate(id, "activen")
          entity_setvelocity(id, vx, vy, vz)
          entity_setmovable(id, true)
          entity_setcollidable(id, true)
          entity_setupdatable(id, true)
        end
      end
    elseif side == "east"  then
      if vx < 0 then
        sound_play(block.collisionsound)
        entity_setvelocity(oid, 0, 0, 0)
        x = x + 2
        entity_setlocation(oid, x, y, z)
        if (level_getentityidat(x - 4, y, z) == -1) then
          entity_setstate(id, "activew")
          entity_setvelocity(id, vx, vy, vz)
          entity_setmovable(id, true)
          entity_setcollidable(id, true)
          entity_setupdatable(id, true)
        end
      end
    elseif side == "west"  then
      if vx > 0 then
        sound_play(block.collisionsound)
        entity_setvelocity(oid, 0, 0, 0)
        x = x - 2
        entity_setlocation(oid, x, y, z)
        if (level_getentityidat(x + 4, y, z) == -1) then
          entity_setstate(id, "activee")
          entity_setvelocity(id, vx, vy, vz)
          entity_setmovable(id, true)
          entity_setcollidable(id, true)
          entity_setupdatable(id, true)
        end
      end
    end
  end
end

function pusherensw.initial.event(id, evt)
end

function pusherensw.initial.render(id)
end

-------------------------------------------------------------------------------
pusherensw.activew = {}

function pusherensw.activew.update(id, tpf)
  if (entity_isstopped(id)) then pusherensw.reset(id) end
end

function pusherensw.activew.collision(id, oid)
end

function pusherensw.activew.event(id, evt)
end

function pusherensw.activew.render(id)
end

-------------------------------------------------------------------------------
pusherensw.activen = {}

function pusherensw.activen.update(id, tpf)
  if (entity_isstopped(id)) then pusherensw.reset(id) end
end

function pusherensw.activen.collision(id, oid)
end

function pusherensw.activen.event(id, evt)
end

function pusherensw.activen.render(id)
end

-------------------------------------------------------------------------------
pusherensw.activee = {}

function pusherensw.activee.update(id, tpf)
  if (entity_isstopped(id)) then pusherensw.reset(id) end
end

function pusherensw.activee.collision(id, oid)
end

function pusherensw.activee.event(id, evt)
end

function pusherensw.activee.render(id)
end

-------------------------------------------------------------------------------
pusherensw.actives = {}

function pusherensw.actives.update(id, tpf)
  if (entity_isstopped(id)) then pusherensw.reset(id) end
end

function pusherensw.actives.collision(id, oid)
end

function pusherensw.actives.event(id, evt)
end

function pusherensw.actives.render(id)
end

-------------------------------------------------------------------------------
function pusherensw.reset(id)
  entity_setstate(id, "initial")
  entity_setupdatable(id, false)
end
