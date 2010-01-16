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
pullersw = {}

function pullersw.register()
  game_entityregistersymbolforname("<.", "pullersw")
end

-------------------------------------------------------------------------------
pullersw.initial = {}

function pullersw.initial.init(id, attr)
  entity_setrenderable(id, true)
  entity_setcollidable(id, true)
  entity_setmesh(id, "cube.lua")
  entity_settexture(id, "pullersw.png")
  if pullere.activatesound == nil then pullere.activatesound = sound_nametoid("buzz.wav") end
end

function pullersw.initial.update(id, tpf)
end

function pullersw.initial.collision(id, oid)
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
        return
      end
    elseif side == "west"  then
      if vx > 0 then
        block.initial.collision(id, oid)
        player.releasefollowers()
        player.followerid = id
        player.followerrelease = pullerw.followerrelease
        sound_play(pullere.activatesound)
        entity_setstate(id, "activew")
        entity_setupdatable(id, true)
      end
    end
  end
end

function pullersw.initial.event(id, evt)
end

function pullersw.initial.render(id)
end

-------------------------------------------------------------------------------
pullersw.activew = {}

function pullersw.activew.update(id, tpf)
  pullerw.active.update(id, tpf)
end

function pullersw.activew.collision(id, oid)
  pullerw.active.collision(id, oid)
end

function pullersw.activew.event(id, evt)
end

function pullersw.activew.render(id)
end

-------------------------------------------------------------------------------
pullersw.actives = {}

function pullersw.actives.update(id, tpf)
  pullers.active.update(id, tpf)
end

function pullersw.actives.collision(id, oid)
  pullers.active.collision(id, oid)
end

function pullersw.actives.event(id, evt)
end

function pullersw.actives.render(id)
end
