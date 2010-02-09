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
bomb = {}

function bomb.register()
  game_entityregistersymbolforname("BB", "bomb")
end

-------------------------------------------------------------------------------
bomb.initial = {}

function bomb.initial.init(id, attr)
  entity_setrenderable(id, true)
  entity_setpostrenderable(id, true)
  entity_setcollidable(id, true)
  entity_setmesh(id, "cube.lua")
  entity_settexture(id, "bomb.png")
  attribute_set(id, "delay", tonumber(attr) or 10)
  attribute_set(id, "odelay", attribute_get(id, "delay"))
  attribute_set(id, "tpf", 0)
  if bomb.ticksound == nil then  bomb.ticksound = sound_nametoid("tick.wav") end
  if bomb.explosionsound == nil then  bomb.explosionsound = sound_nametoid("thump.wav") end
end

function bomb.initial.update(id, tpf)
end

function bomb.initial.collision(id, oid)
  if entity_gettypeof(oid) == "moving" then
    local side = entity_onwhichsideisother(id, oid)
    local x, y, z = entity_getlocation(id)
    local vx, vy, vz = entity_getvelocity(oid)
    if     side == "north" then
      if vz > 0 then
        block.initial.collision(id, oid)
        entity_setstate(id, "countdown")
        entity_setupdatable(id, true)
      end
    elseif side == "south" then
      if vz < 0 then
        block.initial.collision(id, oid)
        entity_setstate(id, "countdown")
        entity_setupdatable(id, true)
      end
    elseif side == "east"  then
      if vx < 0 then
        block.initial.collision(id, oid)
        entity_setstate(id, "countdown")
        entity_setupdatable(id, true)
      end
    elseif side == "west"  then
      if vx > 0 then
        block.initial.collision(id, oid)
        entity_setstate(id, "countdown")
        entity_setupdatable(id, true)
      end
    end
  end
end

function bomb.initial.event(id, evt)
end

function bomb.initial.render(id)
  local x, y, z = entity_getlocation(id)
  opengl_rotate(-90, 1, 0, 0)
  opengl_translate(-1, -0.2, 1.03)
  opengl_scale(0.066, 0.066, 0.066)
  local delay  = attribute_get(id, "delay")
  local odelay = attribute_get(id, "odelay")
  if odelay == 0 then odelay = 1 end
  local fraq = delay / odelay
  -- TODO should this division be removed if it is needed only in countdown-state?
  opengl_color3(1/fraq, fraq, fraq)
  text_printperspective(tostring(delay), 0)
  opengl_color3(1, 1, 1)
end

-------------------------------------------------------------------------------
bomb.countdown = {}

function bomb.countdown.update(id, tpf)
  attribute_inc(id, "tpf", tpf)
  if attribute_get(id, "tpf") >= 1 then
    -- TODO handling of multiple seconds passed? Not probably, though.
    attribute_set(id, "tpf", (attribute_get(id, "tpf") - 1))
    if tonumber(attribute_get(id, "delay")) <= 0 then
      sound_play(bomb.explosionsound)
      entity_setstate(id, "explosion")
      return
    end
    sound_play(bomb.ticksound)
    attribute_dec(id, "delay", 1)
  end
end

function bomb.countdown.collision(id, oid)
  if entity_gettypeof(oid) == "moving" then
    local side = entity_onwhichsideisother(id, oid)
    local x, y, z = entity_getlocation(id)
    local vx, vy, vz = entity_getvelocity(oid)
    if     side == "north" then
      if vz > 0 then
        block.initial.collision(id, oid)
        entity_setstate(id, "explosion")
      end
    elseif side == "south" then
      if vz < 0 then
        block.initial.collision(id, oid)
        entity_setstate(id, "explosion")
      end
    elseif side == "east"  then
      if vx < 0 then
        block.initial.collision(id, oid)
        entity_setstate(id, "explosion")
      end
    elseif side == "west"  then
      if vx > 0 then
        block.initial.collision(id, oid)
        entity_setstate(id, "explosion")
      end
    end
  end
end

function bomb.countdown.event(id, evt)
end

function bomb.countdown.render(id)
  bomb.initial.render(id)
end

-------------------------------------------------------------------------------
bomb.explosion = {}

function bomb.explosion.update(id, tpf)
  entity_setalive(id, false)
end

function bomb.explosion.collision(id, oid)
end

function bomb.explosion.event(id, evt)
end

function bomb.explosion.render(id)
end
