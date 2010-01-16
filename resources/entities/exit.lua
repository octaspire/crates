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
exit = {}

function exit.register()
  game_entityregistersymbolforname("@@", "exit")
end

-------------------------------------------------------------------------------
exit.initial = {}

function exit.initial.init(id, attr)
  entity_setrenderable(id, true)
  entity_setcollidable(id, true)
  entity_setmesh(id, "cube.lua")
  entity_settexture(id, "exit.png")
  if exit.activesound == nil then exit.activesound = sound_nametoid("select.wav") end
  if exit.errorsound == nil then exit.errorsound = sound_nametoid("back.wav") end
end

function exit.initial.update(id, tpf)
end

function exit.initial.nextlevel(id, oid, x, y, z)
  if player.numkeys < game.numkeys or player.numtoggles < game.numtoggles then
    sound_play(exit.errorsound)
    block.initial.collision(id, oid)
    game.restartlevel()
    return
  end
  sound_play(exit.activesound)
  entity_setvelocity(oid, 0, 0, 0)
  entity_setlocation(oid, x, y, z)
  game.nextlevel()
end

function exit.initial.collision(id, oid)
  if entity_isname(oid, "player") then
    local side = entity_onwhichsideisother(id, oid)
    local x, y, z = entity_getlocation(id)
    local vx, vy, vz = entity_getvelocity(oid)
    if entity_getdistancesquared(id, oid) > 3 then return end
    if     side == "north" then
      if vz > 0 then
        exit.initial.nextlevel(id, oid, x, y, z)
      end
    elseif side == "south" then
      if vz < 0 then
        exit.initial.nextlevel(id, oid, x, y, z)
      end
    elseif side == "east"  then
      if vx < 0 then
        exit.initial.nextlevel(id, oid, x, y, z)
      end
    elseif side == "west"  then
      if vx > 0 then
        exit.initial.nextlevel(id, oid, x, y, z)
      end
    end
  end
end

function exit.initial.event(id, evt)
end

function exit.initial.render(id)
end
