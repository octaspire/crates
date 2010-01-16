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
pullers = {}

function pullers.register()
  game_entityregistersymbolforname("..", "pullers")
end

function pullers.followerrelease()
  local vx, vy, vz = entity_getvelocity(player.id)
  local lx, ly, lz = entity_getlocation(player.id)

  entity_setlocation(player.followerid, lx, ly, lz - 2)
  entity_setstate(player.followerid, "initial")
  entity_setupdatable(player.followerid, false)
  player.followerid = nil
  player.followerrelease = nil
end

-------------------------------------------------------------------------------
pullers.initial = {}

function pullers.initial.init(id, attr)
  entity_setrenderable(id, true)
  entity_setcollidable(id, true)
  entity_setmesh(id, "cube.lua")
  entity_settexture(id, "pullers.png")
  if pullere.activatesound == nil then pullere.activatesound = sound_nametoid("buzz.wav") end
end

function pullers.initial.update(id, tpf)
end

function pullers.initial.collision(id, oid)
  if entity_isname(oid, "player") then
    local side = entity_onwhichsideisother(id, oid)
    local x, y, z = entity_getlocation(id)
    local vx, vy, vz = entity_getvelocity(oid)
    if     side == "north" then
      if vz > 0 then
        block.initial.collision(id, oid)
        return
      end
    elseif side == "south" then
      if vz < 0 then
        block.initial.collision(id, oid)
        player.releasefollowers()
        player.followerid = id
        player.followerrelease = pullers.followerrelease
        sound_play(pullere.activatesound)
        entity_setstate(id, "active")
        entity_setupdatable(id, true)
      end
    elseif side == "east"  then
      if vx < 0 then
        block.initial.collision(id, oid)
        return
      end
    elseif side == "west"  then
      if vx > 0 then
        block.initial.collision(id, oid)
      end
    end
  end
end

function pullers.initial.event(id, evt)
end

function pullers.initial.render(id)
end

-------------------------------------------------------------------------------
pullers.active = {}

function pullers.active.update(id, tpf)
  local vx, vy, vz = entity_getvelocity(player.id)
  local lx, ly, lz = entity_getlocation(player.id)
  if vz > 0 then
    entity_setlocation(id, lx, ly, lz - 2)
    player.followerid = id
    player.followerrelease = pullers.followerrelease
  elseif vz < 0 or vy ~= 0 or vx ~= 0 then
    player.releasefollowers()
  end
end

function pullers.active.collision(id, oid)
  if entity_isname(oid, "player") then
    block.initial.collision(id, oid)
  end
end

function pullers.active.event(id, evt)
end

function pullers.active.render(id)
end
