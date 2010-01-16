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
slopews = {}

function slopews.register()
  game_entityregistersymbolforname("\"|", "slopews")
end

-------------------------------------------------------------------------------
slopews.initial = {}

function slopews.initial.init(id, attr)
  entity_setrenderable(id, true)
  entity_setcollidable(id, true)
  entity_setmesh(id, "cube.lua")
  entity_settexture(id, "slopews.png")
  if slopews.activesound == nil then slopews.activesound = sound_nametoid("buzz.wav") end
end

function slopews.initial.update(id, tpf)
end

function slopews.initial.collision(id, oid)
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
        if entity_iswalkable(level_getentityidat(x-2, y, z)) then
          sound_play(slopews.activesound)
          entity_setstate(id, "slidingsw")
          entity_setupdatable(id, true)
        else
          block.initial.collision(id, oid)
        end
        return
      end
    elseif side == "east"  then
      if vx < 0 then
        block.initial.collision(id, oid)
      end
    elseif side == "west"  then
      if vx > 0 then
        if entity_iswalkable(level_getentityidat(x, y, z+2)) then
          sound_play(slopews.activesound)
          entity_setstate(id, "slidingws")
          entity_setupdatable(id, true)
        else
          block.initial.collision(id, oid)
        end
        return
      end
    end
  end
end

function slopews.initial.event(id, evt)
end

function slopews.initial.render(id)
end

-------------------------------------------------------------------------------
slopews.slidingsw = {}

function slopews.slidingsw.update(id, tpf)
  local px, py, pz = entity_getlocation(player.id)
  local tx, ty, tz = entity_getlocation(id)
  local vx, vy, vz = entity_getvelocity(player.id)
  if (pz + vz*tpf) <= tz then
    entity_setlocation(player.id, tx, ty, tz)
    player.releasefollowers()
    player.west()
    entity_setstate(id, "lastphase")
  end
end

function slopews.slidingsw.collision(id, oid)
end

function slopews.slidingsw.event(id, evt)
end

function slopews.slidingsw.render(id)
end

-------------------------------------------------------------------------------
slopews.slidingws = {}

function slopews.slidingws.update(id, tpf)
  local px, py, pz = entity_getlocation(player.id)
  local tx, ty, tz = entity_getlocation(id)
  local vx, vy, vz = entity_getvelocity(player.id)
  if (px + vx*tpf) >= tx then
    entity_setlocation(player.id, tx, ty, tz)
    player.releasefollowers()
    player.south()
    entity_setstate(id, "lastphase")
  end
end

function slopews.slidingws.collision(id, oid)
end

function slopews.slidingws.event(id, evt)
end

function slopews.slidingws.render(id)
end

-------------------------------------------------------------------------------
slopews.lastphase = {}

function slopews.lastphase.update(id, tpf)
  if not entity_hascollision(id, player.id) or (entity_isstopped(player.id)) then
    entity_setstate(id, "initial")
    entity_setupdatable(id, false)
  end
end

function slopews.lastphase.collision(id, oid)
end

function slopews.lastphase.event(id, evt)
end

function slopews.lastphase.render(id)
end
