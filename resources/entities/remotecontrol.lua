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
remotecontrol = {}

function remotecontrol.register()
  game_entityregistersymbolforname("RC", "remotecontrol")
end

function remotecontrol.startrc(id, oid)
  local x, y, z = entity_getlocation(id)
  sound_play(remotecontrol.statussound)
  entity_setvelocity(oid, 0, 0, 0)
  entity_setlocation(oid, x, y, z)
  entity_setrenderable(oid, false)
  entity_setmovable(oid, false)
  entity_setcollidable(oid, false)
  entity_seteventable(oid, false)
  entity_setstate(id, "active")
  entity_seteventable(id, true)
  entity_setpostrenderable(id, true)
end

function remotecontrol.stoprc(id, oid)
  sound_play(remotecontrol.statussound)
  entity_setrenderable(oid, true)
  entity_setmovable(oid, true)
  entity_setcollidable(oid, true)
  entity_seteventable(oid, true)
  entity_setstate(id, "initial")
  entity_seteventable(id, false)
  entity_setpostrenderable(id, false)
end

function remotecontrol.parsetarget(s)
  --x, y
  local result = {}
  local i = 1
  for w in string.gmatch(s, ",?%d+") do
    result[i] = tonumber(w) * 2
    i = i + 1
  end
  if (i ~= 3) then error("Target string for remote control must contain exactly 2 coordinates. Given string \"" .. s .. "\" has syntax error.") end
  return result
end

function remotecontrol.isemptyandinlevellimits(x, y, z)
  if x < -1 or x > game.levelmaxx or z < -1 or z > game.levelmaxy then
    sound_play(remotecontrol.errorsound)
    return false
  end
  if level_getentityidat(x, y, z) ~= -1 then
    sound_play(remotecontrol.errorsound)
    return false
  end
  return true
end

-------------------------------------------------------------------------------
remotecontrol.initial = {}

function remotecontrol.initial.init(id, attr)
  entity_setrenderable(id, true)
  entity_setcollidable(id, true)
  entity_setmesh(id, "cube.lua")
  entity_settexture(id, "remotecontrol.png")
  attribute_set(id, "target", remotecontrol.parsetarget(attr))
  if remotecontrol.activesound == nil then remotecontrol.activesound = sound_nametoid("tick.wav")   end
  if remotecontrol.statussound == nil then remotecontrol.statussound = sound_nametoid("select.wav") end
  if remotecontrol.errorsound  == nil then remotecontrol.errorsound  = sound_nametoid("back.wav")   end
end

function remotecontrol.initial.update(id, tpf)
end

function remotecontrol.initial.collision(id, oid)
  if entity_isname(oid, "player") then
    local side = entity_onwhichsideisother(id, oid)
    local vx, vy, vz = entity_getvelocity(oid)
    if     side == "north" then
      if vz > 0 then
        remotecontrol.startrc(id, oid)
      end
    elseif side == "south" then
      if vz < 0 then
        remotecontrol.startrc(id, oid)
      end
    elseif side == "east"  then
      if vx < 0 then
        remotecontrol.startrc(id, oid)
      end
    elseif side == "west"  then
      if vx > 0 then
        remotecontrol.startrc(id, oid)
      end
    end
  end
end

function remotecontrol.initial.event(id, evt)
end

function remotecontrol.initial.render(id)
end

-------------------------------------------------------------------------------
remotecontrol.active = {}

function remotecontrol.active.update(id, tpf)
end

function remotecontrol.active.collision(id, oid)
end

function remotecontrol.active.event(id, evt)
  local x, ty, z = entity_getlocation(id)
  local target = attribute_get(id, "target")
  local tx = target[1]
  local tz = target[2]
  local targetid = level_getentityidat(tx, ty, tz)
  if     evt == "ok"        then
    entity_setstate(id, "final")
    return
  elseif evt == "up"        then
    tz = tz-2
    if not remotecontrol.isemptyandinlevellimits(tx, ty, tz) then return end
    target[2] = tz
    entity_setlocation(targetid, tx, ty, tz)
  elseif evt == "down"  then
    tz = tz+2
    if not remotecontrol.isemptyandinlevellimits(tx, ty, tz) then return end
    target[2] = tz
    entity_setlocation(targetid, tx, ty, tz)
  elseif evt == "left"  then
    tx = tx-2
    if not remotecontrol.isemptyandinlevellimits(tx, ty, tz) then return end
    target[1] = tx
    entity_setlocation(targetid, tx, ty, tz)
  elseif evt == "right" then
    tx = tx+2
    if not remotecontrol.isemptyandinlevellimits(tx, ty, tz) then return end
    target[1] = tx
    entity_setlocation(targetid, tx, ty, tz)
  end
end

function remotecontrol.active.render(id)
  local x, y, z = entity_getlocation(id)
  opengl_rotate(-90, 1, 0, 0)
  opengl_translate(-1.34, -0.16, 1.03)
  opengl_scale(0.033, 0.033, 0.033)
  opengl_color3(1, 1, 1)
  text_printperspective("!", 1)
  opengl_color3(1, 1, 1)
end

-------------------------------------------------------------------------------
remotecontrol.final = {}

function remotecontrol.final.update(id, tpf)
end

function remotecontrol.final.collision(id, oid)
end

function remotecontrol.final.event(id, evt)
  local tx, ty, tz = entity_getlocation(id)
  if evt == "up"        then
    if entity_iswalkable(level_getentityidat(tx, ty, tz-2)) then
      entity_setlocation(player.id, tx, ty, tz-2)
      player.north()
      remotecontrol.stoprc(id, player.id)
    else
      sound_play(remotecontrol.errorsound)
    end
  elseif evt == "down"  then
    if entity_iswalkable(level_getentityidat(tx, ty, tz+2)) then
      entity_setlocation(player.id, tx, ty, tz+2)
      player.south()
      remotecontrol.stoprc(id, player.id)
    else
      sound_play(remotecontrol.errorsound)
    end
  elseif evt == "left"  then
    if entity_iswalkable(level_getentityidat(tx-2, ty, tz)) then
      entity_setlocation(player.id, tx-2, ty, tz)
      player.west()
      remotecontrol.stoprc(id, player.id)
    else
      sound_play(remotecontrol.errorsound)
    end
  elseif evt == "right" then
    if entity_iswalkable(level_getentityidat(tx+2, ty, tz)) then
      entity_setlocation(player.id, tx+2, ty, tz)
      player.east()
      remotecontrol.stoprc(id, player.id)
    else
      sound_play(remotecontrol.errorsound)
    end
  end
end

function remotecontrol.final.render(id)
  local x, y, z = entity_getlocation(id)
  opengl_rotate(-90, 1, 0, 0)
  opengl_translate(-1.34, -0.16, 1.03)
  opengl_scale(0.033, 0.033, 0.033)
  opengl_color3(0, 1, 0)
  text_printperspective("!", 1)
  opengl_color3(1, 1, 1)
end
