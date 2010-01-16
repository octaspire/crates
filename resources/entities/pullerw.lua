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
pullerw = {}

function pullerw.register()
  game_entityregistersymbolforname("<<", "pullerw")
end

function pullerw.followerrelease()
  local vx, vy, vz = entity_getvelocity(player.id)
  local lx, ly, lz = entity_getlocation(player.id)

  entity_setlocation(player.followerid, lx + 2, ly, lz)
  entity_setstate(player.followerid, "initial")
  entity_setupdatable(player.followerid, false)
  player.followerid = nil
  player.followerrelease = nil
end

-------------------------------------------------------------------------------
pullerw.initial = {}

function pullerw.initial.init(id, attr)
  entity_setrenderable(id, true)
  entity_setcollidable(id, true)
  entity_setmesh(id, "cube.lua")
  entity_settexture(id, "pullerw.png")
  if pullere.activatesound == nil then pullere.activatesound = sound_nametoid("buzz.wav") end
end

function pullerw.initial.update(id, tpf)
end

function pullerw.initial.collision(id, oid)
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
      end
    elseif side == "east"  then
      if vx < 0 then
        block.initial.collision(id, oid)
        return
      end
    elseif side == "west"  then
      if vx > 0 then
        block.initial.collision(id, oid)
        player.releasefollowers()
        player.followerid = id
        player.followerrelease = pullerw.followerrelease
        sound_play(pullere.activatesound)
        entity_setstate(id, "active")
        entity_setupdatable(id, true)
      end
    end
  end
end

function pullerw.initial.event(id, evt)
end

function pullerw.initial.render(id)
end

-------------------------------------------------------------------------------
pullerw.active = {}

function pullerw.active.update(id, tpf)
  local vx, vy, vz = entity_getvelocity(player.id)
  local lx, ly, lz = entity_getlocation(player.id)
  if vx < 0 then
    entity_setlocation(id, lx + 2, ly, lz)
    player.followerid = id
    player.followerrelease = pullerw.followerrelease
  elseif vx > 0 or vy ~= 0 or vz ~= 0 then
    player.releasefollowers()
  end
end

function pullerw.active.collision(id, oid)
  if entity_isname(oid, "player") then
    block.initial.collision(id, oid)
  end
end

function pullerw.active.event(id, evt)
end

function pullerw.active.render(id)
end
