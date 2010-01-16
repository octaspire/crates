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
pullerns = {}

function pullerns.register()
  game_entityregistersymbolforname("^.", "pullerns")
end

-------------------------------------------------------------------------------
pullerns.initial = {}

function pullerns.initial.init(id, attr)
  entity_setrenderable(id, true)
  entity_setcollidable(id, true)
  entity_setmesh(id, "cube.lua")
  entity_settexture(id, "pullerns.png")
  if pullere.activatesound == nil then pullere.activatesound = sound_nametoid("buzz.wav") end
end

function pullerns.initial.update(id, tpf)
end

function pullerns.initial.collision(id, oid)
  if entity_isname(oid, "player") then
    local side = entity_onwhichsideisother(id, oid)
    local x, y, z = entity_getlocation(id)
    local vx, vy, vz = entity_getvelocity(oid)
    if     side == "north" then
      if vz > 0 then
        block.initial.collision(id, oid)
        player.releasefollowers()
        player.followerid = id
        player.followerrelease = pullern.followerrelease
        sound_play(pullere.activatesound)
        entity_setstate(id, "activen")
        entity_setupdatable(id, true)
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
        return
      end
    elseif side == "east"  then
      if vx < 0 then
        block.initial.collision(id, oid)
        return
      end
    elseif side == "west"  then
      if vx > 0 then
        block.initial.collision(id, oid)
        return
      end
    end
  end
end

function pullerns.initial.event(id, evt)
end

function pullerns.initial.render(id)
end

-------------------------------------------------------------------------------
pullerns.actives = {}

function pullerns.actives.update(id, tpf)
  pullers.active.update(id, tpf)
end

function pullerns.actives.collision(id, oid)
  pullers.active.collision(id, oid)
end

function pullerns.actives.event(id, evt)
end

function pullerns.actives.render(id)
end

-------------------------------------------------------------------------------
pullerns.activen = {}

function pullerns.activen.update(id, tpf)
  pullern.active.update(id, tpf)
end

function pullerns.activen.collision(id, oid)
  pullern.active.collision(id, oid)
end

function pullerns.activen.event(id, evt)
end

function pullerns.activen.render(id)
end
