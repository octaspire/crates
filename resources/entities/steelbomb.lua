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
steelbomb = {}

function steelbomb.register()
  game_entityregistersymbolforname("SB", "steelbomb")
  steelbomb.dangerdiameter = 4
  steelbomb.dangerdiametersquared = steelbomb.dangerdiameter ^ 2
  steelbomb.explosionid = texture_nametoid("explosion.png")
end

-------------------------------------------------------------------------------
steelbomb.initial = {}

function steelbomb.initial.init(id, attr)
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
end

function steelbomb.initial.update(id, tpf)
end

function steelbomb.initial.collision(id, oid)
  bomb.initial.collision(id, oid)
end

function steelbomb.initial.event(id, evt)
end

function steelbomb.initial.render(id)
  bomb.initial.render(id)
end

-------------------------------------------------------------------------------
steelbomb.countdown = {}

function steelbomb.countdown.update(id, tpf)
  bomb.countdown.update(id, tpf)
end

function steelbomb.countdown.collision(id, oid)
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

function steelbomb.countdown.event(id, evt)
end

function steelbomb.countdown.render(id)
  bomb.initial.render(id)
end

-------------------------------------------------------------------------------
steelbomb.explosion = {}

function steelbomb.explosion.update(id, tpf)
  local distance = entity_getdistancesquared(id, player.id)
  if (distance <= steelbomb.dangerdiametersquared) then sound_play(exit.errorsound); game.restartlevel(); return end
end

function steelbomb.explosion.collision(id, oid)
end

function steelbomb.explosion.event(id, evt)
end

function steelbomb.explosion.render(id)
  local diameter = attribute_get(id, "diameter")
  diameter = diameter + 0.2
  if diameter > steelbomb.dangerdiameter then
    entity_setalive(id, false)
    return;
  end
  opengl_pushmatrix()
  opengl_rotate(-90, 1, 0, 0)
  --opengl_disk(steelbomb.quadid, diameter-0.5, diameter, 32, 32)
  opengl_bindtexture2d(steelbomb.explosionid)
  opengl_disk(steelbomb.quadid, 2, diameter, 32, 32)
  opengl_popmatrix()
  attribute_set(id, "diameter", diameter)
end
