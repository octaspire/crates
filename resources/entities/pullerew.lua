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
pullerew = {}

function pullerew.register()
  game_entityregistersymbolforname("<>", "pullerew")
end

-------------------------------------------------------------------------------
pullerew.initial = {}

function pullerew.initial.init(id, attr)
  entity_setrenderable(id, true)
  entity_setcollidable(id, true)
  entity_setmesh(id, "cube.lua")
  entity_settexture(id, "pullerew.png")
  if pullere.activatesound == nil then pullere.activatesound = sound_nametoid("buzz.wav") end
end

function pullerew.initial.update(id, tpf)
end

function pullerew.initial.collision(id, oid)
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
        return
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
  elseif entity_gettypeof(oid) == "moving" then
    block.initial.collision(id, oid)
  end
end

function pullerew.initial.event(id, evt)
end

function pullerew.initial.render(id)
end

-------------------------------------------------------------------------------
pullerew.activew = {}

function pullerew.activew.update(id, tpf)
  pullerw.active.update(id, tpf)
end

function pullerew.activew.collision(id, oid)
  pullerw.active.collision(id, oid)
end

function pullerew.activew.event(id, evt)
end

function pullerew.activew.render(id)
end

-------------------------------------------------------------------------------
pullerew.activee = {}

function pullerew.activee.update(id, tpf)
  pullere.active.update(id, tpf)
end

function pullerew.activee.collision(id, oid)
  pullere.active.collision(id, oid)
end

function pullerew.activee.event(id, evt)
end

function pullerew.activee.render(id)
end
