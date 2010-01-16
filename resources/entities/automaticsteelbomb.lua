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
automaticsteelbomb = {}

function automaticsteelbomb.register()
  game_entityregistersymbolforname("AB", "automaticsteelbomb")
  automaticsteelbomb.dangerdiameter = 4
  automaticsteelbomb.dangerdiametersquared = automaticsteelbomb.dangerdiameter ^ 2
end

-------------------------------------------------------------------------------
automaticsteelbomb.initial = {}

function automaticsteelbomb.initial.init(id, attr)
  entity_setrenderable(id, true)
  entity_setpostrenderable(id, true)
  entity_setcollidable(id, true)
  entity_setmesh(id, "cube.lua")
  entity_settexture(id, "steelbomb.png")
  attribute_set(id, "delay", tonumber(attr) or 10)
  attribute_set(id, "odelay", attribute_get(id, "delay"))
  attribute_set(id, "diameter", 2)
  attribute_set(id, "tpf", 0)
  if bomb.ticksound == nil then  bomb.ticksound = sound_nametoid("tick.wav") end
  if bomb.explosionsound == nil then  bomb.explosionsound = sound_nametoid("thump.wav") end
  if steelbomb.quadid == nil then steelbomb.quadid = opengl_newquadric() end
  entity_setstate(id, "countdown")
  entity_setupdatable(id, true)
end

function automaticsteelbomb.initial.update(id, tpf)
end

function automaticsteelbomb.initial.collision(id, oid)
  bomb.initial.collision(id, oid)
end

function automaticsteelbomb.initial.event(id, evt)
end

function automaticsteelbomb.initial.render(id)
  bomb.initial.render(id)
end

-------------------------------------------------------------------------------
automaticsteelbomb.countdown = {}

function automaticsteelbomb.countdown.update(id, tpf)
  bomb.countdown.update(id, tpf)
end

function automaticsteelbomb.countdown.collision(id, oid)
  if entity_isname(oid, "player") then
    local side = entity_onwhichsideisother(id, oid)
    local x, y, z = entity_getlocation(id)
    local vx, vy, vz = entity_getvelocity(oid)
    if     side == "north" then
      if vz > 0 then
        block.initial.collision(id, oid)
      end
    elseif side == "south" then
      if vz < 0 then
        block.initial.collision(id, oid)
      end
    elseif side == "east"  then
      if vx < 0 then
        block.initial.collision(id, oid)
      end
    elseif side == "west"  then
      if vx > 0 then
        block.initial.collision(id, oid)
      end
    end
  end
end

function automaticsteelbomb.countdown.event(id, evt)
end

function automaticsteelbomb.countdown.render(id)
  bomb.initial.render(id)
end

-------------------------------------------------------------------------------
automaticsteelbomb.explosion = {}

function automaticsteelbomb.explosion.update(id, tpf)
  local distance = entity_getdistancesquared(id, player.id)
  if (distance <= automaticsteelbomb.dangerdiametersquared) then sound_play(exit.errorsound); game.restartlevel(); return end
end

function automaticsteelbomb.explosion.collision(id, oid)
end

function automaticsteelbomb.explosion.event(id, evt)
end

function automaticsteelbomb.explosion.render(id)
  local diameter = attribute_get(id, "diameter")
  diameter = diameter + 0.2
  if diameter > automaticsteelbomb.dangerdiameter then
    entity_setalive(id, false)
    return;
  end
  opengl_pushmatrix()
  opengl_rotate(90, 1, 0, 0)
  --opengl_disk(steelbomb.quadid, diameter-0.5, diameter, 32, 32)
  opengl_disk(steelbomb.quadid, 2, diameter, 32, 32)
  opengl_popmatrix()
  attribute_set(id, "diameter", diameter)
end
