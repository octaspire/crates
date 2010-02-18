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
automaton = {}

function automaton.register()
  game_entityregistersymbolforname("AU", "automaton")
end

function automaton.startprogramming(id, oid)
  local x, y, z = entity_getlocation(id)
  sound_play(remotecontrol.statussound)
  entity_setvelocity(oid, 0, 0, 0)
  entity_setlocation(oid, x, y, z)
  entity_setrenderable(oid, false)
  entity_setmovable(oid, false)
  entity_setcollidable(oid, false)
  entity_seteventable(oid, false)
  entity_setstate(id, "programming")
  entity_seteventable(id, true)
  entity_setpostrenderable(id, true)
end

function automaton.stopprogramming(id, oid)
  sound_play(remotecontrol.statussound)
  entity_setrenderable(oid, true)
  entity_setmovable(oid, true)
  entity_setcollidable(oid, true)
  entity_seteventable(oid, true)
  entity_setstate(id, "running")
  entity_seteventable(id, false)
  entity_setupdatable(id, true)
end

function automaton.isemptyandinlevellimits(x, y, z)
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

function automaton.open(id)
  attribute_set(id, "isclosed", false)
  entity_settexture(id, "inversecounter.png")
end

function automaton.close(id)
  attribute_set(id, "isclosed", true)
  entity_settexture(id, "automaton.png")
end

function automaton.execute(id, command)
  local x, y, z = entity_getlocation(id)
  attribute_set(id, "delay", 1)
  if attribute_get(id, "isclosed") == false then
    automaton.close(id)
  end
  if     command == "\128" then
    x = x + 2
  elseif command == "\129" then
    z = z - 2
  elseif command == "\130" then
    x = x - 2
  elseif command == "\131" then
    z = z + 2
  elseif command == "\132" then
    attribute_set(id, "delay", 5)
    return
  elseif command == "\133" then
    attribute_set(id, "delay", 5)
    automaton.open(id)
    return
  end
  if automaton.isemptyandinlevellimits(x, y, z) then
    entity_setlocation(id, x, y, z)
  else
    sound_play(remotecontrol.errorsound)
  end
end

-------------------------------------------------------------------------------
automaton.initial = {}
automaton.instructionset = {"\128", "\129", "\130", "\131", "\132", "\133"}
automaton.memorysize     = 32

function automaton.initial.init(id, attr)
  entity_setrenderable(id, true)
  entity_setcollidable(id, true)
  entity_setmesh(id, "cube.lua")
  automaton.close(id)
  attribute_set(id, "commands", {automaton.instructionset[1]})
  attribute_set(id, "lastindex", 1)
  attribute_set(id, "tpf",   0)
  attribute_set(id, "delay", 1)
  attribute_set(id, "index", 1)
  if remotecontrol.activesound == nil then remotecontrol.activesound = sound_nametoid("tick.wav")   end
  if remotecontrol.statussound == nil then remotecontrol.statussound = sound_nametoid("select.wav") end
  if remotecontrol.errorsound  == nil then remotecontrol.errorsound  = sound_nametoid("back.wav")   end
  automaton.cr = 1
  automaton.crvel = -0.08
end

function automaton.initial.update(id, tpf)
end

function automaton.initial.collision(id, oid)
  if entity_isname(oid, "player") then
    local side = entity_onwhichsideisother(id, oid)
    local vx, vy, vz = entity_getvelocity(oid)
    if     side == "north" then
      if vz > 0 then
        automaton.startprogramming(id, oid)
      end
    elseif side == "south" then
      if vz < 0 then
        automaton.startprogramming(id, oid)
      end
    elseif side == "east"  then
      if vx < 0 then
        automaton.startprogramming(id, oid)
      end
    elseif side == "west"  then
      if vx > 0 then
        automaton.startprogramming(id, oid)
      end
    end
  elseif entity_gettypeof(oid) == "moving" then
    block.initial.collision(id, oid)
  end
end

function automaton.initial.event(id, evt)
end

function automaton.initial.render(id)
end

-------------------------------------------------------------------------------
automaton.programming = {}

function automaton.programming.update(id, tpf)
end

function automaton.programming.collision(id, oid)
  block.initial.collision(id, oid)
end

function automaton.programming.event(id, evt)
  local commands  = attribute_get(id, "commands")
  local lastindex = attribute_get(id, "lastindex")
  local ncommands = #commands
  if     evt == "ok"        then
    entity_setstate(id, "exit")
    return
  elseif evt == "up"        then
    if lastindex > 1 then lastindex = lastindex - 1 else lastindex = #automaton.instructionset end
    attribute_set(id, "lastindex", lastindex)
    commands[#commands] = automaton.instructionset[lastindex]
  elseif evt == "down"  then
    if lastindex < #automaton.instructionset then lastindex = lastindex + 1 else lastindex = 1 end
    attribute_set(id, "lastindex", lastindex)
    commands[#commands] = automaton.instructionset[lastindex]
  elseif evt == "left"  then
    table.remove(commands)
    attribute_set(id, "lastindex", 1)
  elseif evt == "right" then
    if (#commands+1) < automaton.memorysize then
      commands[#commands+1] = automaton.instructionset[1]
      attribute_set(id, "lastindex", 1)
    else
      sound_play(remotecontrol.errorsound)
    end
  end
end

function automaton.programming.render(id)
  local commands  = attribute_get(id, "commands")
  opengl_rotate(-90, 1, 0, 0)
  --opengl_translate(-1.34 + (-0.5 * #commands), -0.16, 1.03)
  opengl_translate(-1.34 + (-0.64 * #commands), 0.4, 1.03)
  opengl_scale(0.16, 0.16, 0.16)
  opengl_color3(1, 1, 1)
  for i, command in ipairs(commands) do
      if i == #commands then opengl_color3(automaton.cr, 0, 0) end
      text_printperspective(command, 0)
      opengl_translate(0.1, 0, 0)
  end
  opengl_color3(1, 1, 1)
  automaton.cr = automaton.cr + automaton.crvel
  if automaton.cr <= 0.2 or automaton.cr >= 1 then automaton.crvel = -automaton.crvel end
end

-------------------------------------------------------------------------------
automaton.running = {}

function automaton.running.update(id, tpf)
  local commands  = attribute_get(id, "commands")
  local delay     = attribute_get(id, "delay")
  local index     = attribute_get(id, "index")
  attribute_inc(id, "tpf", tpf)
  if attribute_get(id, "tpf") >= delay then
    -- TODO handling of multiple seconds passed? Not probably, though.
    attribute_set(id, "tpf", (attribute_get(id, "tpf") - delay))
    if index <= #commands then
        automaton.execute(id, commands[index])
        if (commands[index] == "\132") or (commands[index] == "\133") then
          attribute_set(id, "delay", 5)
        else
          attribute_set(id, "delay", 1)
        end
        attribute_set(id, "index", index+1)
    else
      entity_setstate(id, "final")
      entity_setpostrenderable(id, false)
      automaton.close(id)
    end
  end
end

function automaton.running.collision(id, oid)
  local isclosed = attribute_get(id, "isclosed")
  if isclosed then
    block.initial.collision(id, oid)
  end
end

function automaton.running.event(id, evt)
end

function automaton.running.render(id)
  local x, y, z = entity_getlocation(id)
  local commands  = attribute_get(id, "commands")
  local index     = attribute_get(id, "index")
  opengl_rotate(-90, 1, 0, 0)
  opengl_translate(-1.34 + (-0.25 * #commands), 0.4, 1.03)
  --opengl_scale(0.16, 0.16, 0.16)
  opengl_scale(0.08, 0.08, 0.08)
  for i, command in ipairs(commands) do
      if i == index-1 then opengl_color3(1, 0, 0) else opengl_color3(1, 1, 1) end
      text_printperspective(command, 0)
      opengl_translate(0.05, 0, 0)
  end
  opengl_color3(1, 1, 1)
end

-------------------------------------------------------------------------------
automaton.exit = {}

function automaton.exit.update(id, tpf)
end

function automaton.exit.collision(id, oid)
end

function automaton.exit.event(id, evt)
  local tx, ty, tz = entity_getlocation(id)
  if evt == "up"        then
    if entity_iswalkable(level_getentityidat(tx, ty, tz-2)) then
      entity_setlocation(player.id, tx, ty, tz-2)
      player.north()
      automaton.stopprogramming(id, player.id)
    else
      sound_play(remotecontrol.errorsound)
    end
  elseif evt == "down"  then
    if entity_iswalkable(level_getentityidat(tx, ty, tz+2)) then
      entity_setlocation(player.id, tx, ty, tz+2)
      player.south()
      automaton.stopprogramming(id, player.id)
    else
      sound_play(remotecontrol.errorsound)
    end
  elseif evt == "left"  then
    if entity_iswalkable(level_getentityidat(tx-2, ty, tz)) then
      entity_setlocation(player.id, tx-2, ty, tz)
      player.west()
      automaton.stopprogramming(id, player.id)
    else
      sound_play(remotecontrol.errorsound)
    end
  elseif evt == "right" then
    if entity_iswalkable(level_getentityidat(tx+2, ty, tz)) then
      entity_setlocation(player.id, tx+2, ty, tz)
      player.east()
      automaton.stopprogramming(id, player.id)
    else
      sound_play(remotecontrol.errorsound)
    end
  end
end

function automaton.exit.render(id)
  local x, y, z = entity_getlocation(id)
  opengl_rotate(-90, 1, 0, 0)
  opengl_translate(-1.34, -0.16, 1.03)
  opengl_scale(0.066, 0.066, 0.066)
  opengl_color3(0, 1, 0)
  text_printperspective("!", 1)
  opengl_color3(1, 1, 1)
end


-------------------------------------------------------------------------------
automaton.final = {}

function automaton.final.update(id, tpf)
end

function automaton.final.collision(id, oid)
  block.initial.collision(id, oid)
end

function automaton.final.event(id, evt)
end

function automaton.final.render(id)
end
