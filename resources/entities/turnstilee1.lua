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
turnstilee1 = {}

function turnstilee1.register()
  game_entityregistersymbolforname("1>", "turnstilee1")
end

-------------------------------------------------------------------------------
turnstilee1.initial = {}

function turnstilee1.initial.init(id, attr)
  entity_setrenderable(id, true)
  entity_setcollidable(id, true)
  entity_setmesh(id, "cube.lua")
  entity_settexture(id, "turnstilee1.png")
  if turnstilee.activesound == nil then turnstilee.activesound = sound_nametoid("buzz.wav") end
end

function turnstilee1.initial.update(id, tpf)
end

function turnstilee1.initial.collision(id, oid)
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
        return
      end
    elseif side == "east"  then
      if vx < 0 then
        block.initial.collision(id, oid)
      end
    elseif side == "west"  then
      if vx > 0 then
        if entity_iswalkable(level_getentityidat(x+2, y, z)) then
          entity_setlocation(player.id, x-2, y, z)
          player.releasefollowers()
          sound_play(turnstilee.activesound)
          entity_setvelocity(player.id, 0, 0, 0)
          x = x + 2
          entity_setlocation(player.id, x, y, z)
        else
          block.initial.collision(id, oid)
        end
        return
      end
    end
  end
end

function turnstilee1.initial.event(id, evt)
end

function turnstilee1.initial.render(id)
end
