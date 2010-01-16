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
pulleres = {}

function pulleres.register()
  game_entityregistersymbolforname(">.", "pulleres")
end

-------------------------------------------------------------------------------
pulleres.initial = {}

function pulleres.initial.init(id, attr)
  entity_setrenderable(id, true)
  entity_setcollidable(id, true)
  entity_setmesh(id, "cube.lua")
  entity_settexture(id, "pulleres.png")
  if pullere.activatesound == nil then pullere.activatesound = sound_nametoid("buzz.wav") end
end

function pulleres.initial.update(id, tpf)
end

function pulleres.initial.collision(id, oid)
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
        entity_setstate(id, "actives")
        entity_setupdatable(id, true)
      end
    elseif side == "east"  then
      if vx < 0 then
        block.initial.collision(id, oid)
        player.releasefollowers()
        player.followerid = id
        player.followerrelease = pullere.followerrelease
        sound_play(pullere.activatesound)
        entity_setstate(id, "activee")
        entity_setupdatable(id, true)
        return
      end
    elseif side == "west"  then
      if vx > 0 then
        block.initial.collision(id, oid)
      end
    end
  end
end

function pulleres.initial.event(id, evt)
end

function pulleres.initial.render(id)
end

-------------------------------------------------------------------------------
pulleres.activee = {}

function pulleres.activee.update(id, tpf)
  pullere.active.update(id, tpf)
end

function pulleres.activee.collision(id, oid)
  pullere.active.collision(id, oid)
end

function pulleres.activee.event(id, evt)
end

function pulleres.activee.render(id)
end

-------------------------------------------------------------------------------
pulleres.actives = {}

function pulleres.actives.update(id, tpf)
  pullers.active.update(id, tpf)
end

function pulleres.actives.collision(id, oid)
  pullers.active.collision(id, oid)
end

function pulleres.actives.event(id, evt)
end

function pulleres.actives.render(id)
end
