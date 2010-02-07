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
counter = {}

function counter.register()
  game_entityregistersymbolforname("##", "counter")
end

-------------------------------------------------------------------------------
counter.initial = {}

function counter.initial.init(id, attr)
  entity_setrenderable(id, true)
  entity_setpostrenderable(id, true)
  entity_setcollidable(id, true)
  entity_setmesh(id, "cube.lua")
  entity_settexture(id, "block.png")
  attribute_set(id, "count", tonumber(attr) or 10)
  if counter.activesound == nil then counter.activesound = sound_nametoid("tick.wav") end
  if block.collisionsound == nil then block.collisionsound = sound_nametoid("thump.wav") end
end

function counter.initial.update(id, tpf)
end

function counter.initial.collision(id, oid)
  if entity_isname(oid, "player") then
    local side = entity_onwhichsideisother(id, oid)
    local x, y, z = entity_getlocation(id)
    local vx, vy, vz = entity_getvelocity(oid)
    if     side == "north" then
      if vz > 0 then
        sound_play(block.collisionsound)
        sound_play(counter.activesound)
        entity_setvelocity(oid, 0, 0, 0)
        z = z - 2
        entity_setlocation(oid, x, y, z)
        if attribute_get(id, "count") > 1 then
          attribute_dec(id, "count")
        else
          entity_setalive(id, false)
          return
        end
      end
    elseif side == "south" then
      if vz < 0 then
        sound_play(block.collisionsound)
        sound_play(counter.activesound)
        entity_setvelocity(oid, 0, 0, 0)
        z = z + 2
        entity_setlocation(oid, x, y, z)
        if attribute_get(id, "count") > 1 then
          attribute_dec(id, "count")
        else
          entity_setalive(id, false)
          return
        end
      end
    elseif side == "east"  then
      if vx < 0 then
        sound_play(block.collisionsound)
        sound_play(counter.activesound)
        entity_setvelocity(oid, 0, 0, 0)
        x = x + 2
        entity_setlocation(oid, x, y, z)
        if attribute_get(id, "count") > 1 then
          attribute_dec(id, "count")
        else
          entity_setalive(id, false)
          return
        end
      end
    elseif side == "west"  then
      if vx > 0 then
        sound_play(block.collisionsound)
        sound_play(counter.activesound)
        entity_setvelocity(oid, 0, 0, 0)
        x = x - 2
        entity_setlocation(oid, x, y, z)
        if attribute_get(id, "count") > 1 then
          attribute_dec(id, "count")
        else
          entity_setalive(id, false)
          return
        end
      end
    end
  end
end

function counter.initial.event(id, evt)
end

function counter.initial.render(id)
  local x, y, z = entity_getlocation(id)
  opengl_rotate(-90, 1, 0, 0)
  opengl_translate(-1, -0.2, 1.03)
  opengl_scale(0.033, 0.033, 0.033)
  text_printperspective(tostring(attribute_get(id, "count")), 0)
end
