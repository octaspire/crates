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
slopeen = {}

function slopeen.register()
  game_entityregistersymbolforname("|_", "slopeen")
end

-------------------------------------------------------------------------------
slopeen.initial = {}

function slopeen.initial.init(id, attr)
  entity_setrenderable(id, true)
  entity_setcollidable(id, true)
  entity_setmesh(id, "cube.lua")
  entity_settexture(id, "slopeen.png")
  if slopews.activesound == nil then slopews.activesound = sound_nametoid("buzz.wav") end
end

function slopeen.initial.update(id, tpf)
end

function slopeen.initial.collision(id, oid)
  if entity_gettypeof(oid) == "moving" then
    local side = entity_onwhichsideisother(id, oid)
    local x, y, z = entity_getlocation(id)
    local vx, vy, vz = entity_getvelocity(oid)
    if     side == "north" then
      if vz > 0 then
        if entity_iswalkable(level_getentityidat(x+2, y, z)) then
          sound_play(slopews.activesound)
          entity_setstate(id, "slidingne")
          entity_setupdatable(id, true)
          attribute_set(id, "oid", oid)
        else
          block.initial.collision(id, oid)
        end
        return
      end
    elseif side == "south" then
      if vz < 0 then
        block.initial.collision(id, oid)
      end
    elseif side == "east"  then
      if vx < 0 then
        if entity_iswalkable(level_getentityidat(x, y, z-2)) then
          sound_play(slopews.activesound)
          entity_setstate(id, "slidingen")
          entity_setupdatable(id, true)
          attribute_set(id, "oid", oid)
        else
          block.initial.collision(id, oid)
        end
        return
      end
    elseif side == "west"  then
      if vx > 0 then
        block.initial.collision(id, oid)
      end
    end
  end
end

function slopeen.initial.event(id, evt)
end

function slopeen.initial.render(id)
end

-------------------------------------------------------------------------------
slopeen.slidingne = {}

function slopeen.slidingne.update(id, tpf)
  local oid = attribute_get(id, "oid")
  local px, py, pz = entity_getlocation(oid)
  local tx, ty, tz = entity_getlocation(id)
  local vx, vy, vz = entity_getvelocity(oid)
  if (pz + vz*tpf) >= tz then
    entity_setlocation(oid, tx, ty, tz)
    if (oid == player.id) then player.releasefollowers() end
    entity_east(oid)
    entity_setstate(id, "lastphase")
  end
end

function slopeen.slidingne.collision(id, oid)
end

function slopeen.slidingne.event(id, evt)
end

function slopeen.slidingne.render(id)
end

-------------------------------------------------------------------------------
slopeen.slidingen = {}

function slopeen.slidingen.update(id, tpf)
  local oid = attribute_get(id, "oid")
  local px, py, pz = entity_getlocation(oid)
  local tx, ty, tz = entity_getlocation(id)
  local vx, vy, vz = entity_getvelocity(oid)
  if (px + vx*tpf) <= tx then
    entity_setlocation(oid, tx, ty, tz)
    if (oid == player.id) then player.releasefollowers() end
    entity_north(oid)
    entity_setstate(id, "lastphase")
  end
end

function slopeen.slidingen.collision(id, oid)
end

function slopeen.slidingen.event(id, evt)
end

function slopeen.slidingen.render(id)
end

-------------------------------------------------------------------------------
slopeen.lastphase = {}

function slopeen.lastphase.update(id, tpf)
  local oid = attribute_get(id, "oid")
  if not entity_hascollision(id, oid) or (entity_isstopped(oid)) then
    entity_setstate(id, "initial")
    entity_setupdatable(id, false)
  end
end

function slopeen.lastphase.collision(id, oid)
end

function slopeen.lastphase.event(id, evt)
end

function slopeen.lastphase.render(id)
end
