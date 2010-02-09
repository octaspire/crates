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
trigger = {}

function trigger.register()
  game_entityregistersymbolforname("!!", "trigger")
end

-------------------------------------------------------------------------------
trigger.initial = {}

function trigger.initial.init(id, attr)
  entity_setrenderable(id, false)
  entity_setupdatable(id, false)
  entity_setcollidable(id, true)
  local la = {}
  la = game.parselookat(tostring(attr) or "")
  attribute_set(id, "lookat", la)
end

function cdist(a, b)
  local epsilon = 0.1
  if math.abs(a - b) > epsilon then
    return true
  else
    return false
  end
end

function trigger.initial.update(id, tpf)
end

function trigger.initial.collision(id, oid)
  if entity_gettypeof(oid) == "moving" then
    local side = entity_onwhichsideisother(id, oid)
    local x, y, z = entity_getlocation(id)
    local vx, vy, vz = entity_getvelocity(oid)
    if     side == "north" then
      if vz > 0 then
        entity_setstate(id, "running")
        entity_setupdatable(id, true)
      end
    elseif side == "south" then
      if vz < 0 then
        entity_setstate(id, "running")
        entity_setupdatable(id, true)
      end
    elseif side == "east"  then
      if vx < 0 then
        entity_setstate(id, "running")
        entity_setupdatable(id, true)
      end
    elseif side == "west"  then
      if vx > 0 then
        entity_setstate(id, "running")
        entity_setupdatable(id, true)
      end
    end
  end
end

function trigger.initial.event(id, evt)
end

function trigger.initial.render(id)
end

-------------------------------------------------------------------------------
trigger.running = {}


function trigger.running.update(id, tpf)
  --eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ
  local rcount = 0
  local la = attribute_get(id, "lookat")
  for i=1,9 do
    local stat = cdist(game.la[i], la[i])
    if stat and (game.la[i] < la[i]) then
      game.la[i] = game.la[i] + 0.1
    elseif stat and (game.la[i] > la[i]) then
      game.la[i] = game.la[i] - 0.1
    else
      rcount = rcount + 1
    end
  end
  if rcount == 9 then
    entity_setstate(id, "initial")
    entity_setupdatable(id, false)
  else
    opengl_lookat(unpack(game.la))
  end
end

function trigger.running.collision(id, oid)
end

function trigger.running.event(id, evt)
end

function trigger.running.render(id)
end
