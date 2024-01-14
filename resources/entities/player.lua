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
player = {}

function player.register()
  game_entityregistersymbolforname("&&", "player")
end

function player.releasefollowers()
  if player.followerid ~= nil and player.followerrelease ~= nil then
    player.followerrelease()
  end
end

-------------------------------------------------------------------------------
player.initial = {}

function player.initial.init(id, attr)
  player.id = id
  player.numkeys = 0
  player.numtoggles = 0
  player.speed = 10
  player.followerid = nil
  player.followerrelease = nil
  player.pendingevt = nil
  entity_setrenderable(id, true)
  entity_setmovable(id, true)
  entity_setcollidable(id, true)
  entity_seteventable(id, true)
  entity_setupdatable(id, true)
  entity_setmesh(id, "player.lua")
  entity_settexture(id, "player.png")
  attribute_set(id, "typeof", "moving")
end

function player.initial.update(id, tpf)
  if player.pendingevt then
    if entity_isstopped(id) then
      player.initial.event(id, player.pendingevt)
      player.pendingevt = nil
    end
  end
end

function player.initial.collision(id, oid)
end

function player.initial.event(id, evt)
  if not entity_isstopped(id) then
    player.pendingevt = evt
    return
  end
  if evt == "up"        then
    player.north()
  elseif evt == "down"  then
    player.south()
  elseif evt == "left"  then
    player.west()
  elseif evt == "right" then
    player.east()
  end
end

function player.initial.render(id)
end

function player.north()
  entity_north(player.id)
end

function player.south()
  entity_south(player.id)
end

function player.west()
  entity_west(player.id)
end

function player.east()
  entity_east(player.id)
end
