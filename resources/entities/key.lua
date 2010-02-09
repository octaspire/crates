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
key = {}

function key.register()
  game_entityregistersymbolforname("$$", "key")
end

-------------------------------------------------------------------------------
key.initial = {}

function key.initial.init(id, attr)
  entity_setrenderable(id, true)
  entity_setcollidable(id, true)
  entity_setmesh(id, "cube.lua")
  entity_settexture(id, "key.png")
  game.numkeys = game.numkeys + 1
  if key.picksound == nil then key.picksound = sound_nametoid("select.wav") end
end

function key.initial.update(id, tpf)
end

function key.initial.collision(id, oid)
  if entity_isname(oid, "player") then
    local side = entity_onwhichsideisother(id, oid)
    local x, y, z = entity_getlocation(id)
    local vx, vy, vz = entity_getvelocity(oid)
    if     side == "north" then
      if vz > 0 then
        sound_play(key.picksound)
        player.numkeys =  player.numkeys  + 1
        entity_setalive(id, false)
      end
    elseif side == "south" then
      if vz < 0 then
        sound_play(key.picksound)
        player.numkeys =  player.numkeys  + 1
        entity_setalive(id, false)
      end
    elseif side == "east"  then
      if vx < 0 then
        sound_play(key.picksound)
        player.numkeys =  player.numkeys  + 1
        entity_setalive(id, false)
      end
    elseif side == "west"  then
      if vx > 0 then
        sound_play(key.picksound)
        player.numkeys =  player.numkeys  + 1
        entity_setalive(id, false)
      end
    elseif side == "unknown" then
      if entity_getdistancesquared(id, oid) < 1.1 then
        sound_play(key.picksound)
        player.numkeys =  player.numkeys  + 1
        entity_setalive(id, false)
      end
    end
  elseif entity_gettypeof(oid) == "moving" then
    block.initial.collision(id, oid)
  end
end

function key.initial.event(id, evt)
end

function key.initial.render(id)
end
