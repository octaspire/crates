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
game = {}
game.initial = {}

function game.parselookat(s)
  --eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ
  local result = {}
  local i = 1
  for w in string.gmatch(s, "%-?%d+") do
    result[i] = tonumber(w)
    i = i + 1
  end
  return result
end

function game.initial.init(arg)
  attributes_reset()
  game.arg = tostring(arg)
  game.isnextlevel = false
  game.numkeys = 0
  game.numtoggles = 0
  game.isrestartlevel = false
  local leveltable = game_getcurrentlevel()
  music_stop()
  local x = 0
  local y = 0
  local z = 0
  local symbol
  local atrindex
  local atr
  local nplayers = 0;
  game.levelmaxx = 0
  game.levelmaxy = 0

  for k, v in ipairs(leveltable.lines) do
    for w in string.gmatch(v, "[%[%]%w%#%@%!%$%&%<%>%^%.%|%_\"T%+%-]+") do
      w = string.gsub(w, "%-", "")
      if string.len(w) > 1 then
        symbol   = string.sub(w, 1, 2)
        atrindex = string.sub(w, 3, 4)
        if string.len(atrindex) > 0 and  atrindex ~= "--" and atrindex ~= "  " then
          atr = leveltable.arguments[tonumber(atrindex)]
        else
          atr = ""
        end
        if atr == nil then atr = "" end
        local name, emsg = game_entitytranslatesymboltoname(symbol)
        if name then
          if name == "player" then nplayers = nplayers + 1 end
          if nplayers > 1 then error(string.format("level \"%s\" has more than one player", leveltable.name or "")) end
          local id = state_addentity(name, atr, x, z, y)
        else
          -- TODO use oerror_warning, etc.
          print(emsg)
        end
      end
      x = x + 2
      if x >= game.levelmaxx then game.levelmaxx = x end
    end
    y = y + 2
    x = 0
  end

  game.levelmaxy = y

  if nplayers < 1 then error(string.format("level \"%s\" does not have a player", leveltable.name or "")) end

  state_seteventable(true)
  state_setupdatable(true)
  state_setrenderable(true)

  game.la = {}
  game.la = game.parselookat(leveltable.lookAt)
  opengl_lookat(unpack(game.la))

  state_messagestackpush(leveltable.name)
  state_messagestackpush("Password: " .. leveltable.password)

  if game.errorsound == nil then game.errorsound = sound_nametoid("back.wav") end
  music_playrandom()

  if leveltable.postinit then
    game_dostring(leveltable.postinit)
  end

  if (mission_getcurrentlevelnum() == 0) then mission_resettimer() end
end

function game.restartlevel()
  game.isrestartlevel = true
end

function game.initial.update(tpf)
  opengl_lookat(unpack(game.la))

  if game.isrestartlevel then
    game_changestate("game", menu.arg)
  end

  local px, py, pz = entity_getlocation(player.id)
  if px < -1 or px > game.levelmaxx or pz < -1 or pz > game.levelmaxy then
    sound_play(game.errorsound)
    game_changestate("game", menu.arg)
  end

  if game.isnextlevel then
    if mission_hasnextlevel() then
      mission_nextlevel()
      game_changestate("game", game.arg)
    else
      if mission_getnumpassed() == (mission_getnumlevels()-1) then
        mission_stoptimer()
        game_changestate("missionpassedat1go", game.arg)
      else
        game_changestate("missionpassed", game.arg)
      end
    end
  end
end

function game.nextlevel()
  game.isnextlevel = true
end

function game.initial.event(evt)
  if     evt == "exit" then
    sound_play(mission.backsound)
    game_changestate("menu", game.arg)
  elseif evt == "R" then
    sound_play(game.errorsound)
    game_changestate("game", menu.arg)
  end
end

function game.initial.render()
end

-------------------------------------------------------------------------------
function attributes_reset()
  attributes = nil
  attributes = {}
end

function attribute_set(id, name, value)
  if not attributes[tonumber(id)] then attributes[tonumber(id)] = {} end
  attributes[tonumber(id)][tostring(name)] = value
end

function attribute_inc(id, name, amount)
  local amount = tonumber(amount or 1)
  attributes[tonumber(id)][tostring(name)] = attributes[tonumber(id)][tostring(name)] + amount
end

function attribute_dec(id, name, amount)
  local amount = tonumber(amount or 1)
  attributes[tonumber(id)][tostring(name)] = attributes[tonumber(id)][tostring(name)] - amount
end

function attribute_toggle(id, name)
  attributes[tonumber(id)][tostring(name)] = not attributes[tonumber(id)][tostring(name)]
end

function attribute_get(id, name)
  if attributes[tonumber(id)] == nil then return nil end
  return attributes[tonumber(id)][tostring(name)]
end

-------------------------------------------------------------------------------
function entity_iswalkable(id)
  if (id < 0) then return true end

  local name = entity_getname(id)
  if name == "key" then return true end
  if ((name == "inversecounter") and (entity_getstate(id) ~= "final")) then return true end
  return false
end

function entity_gettypeof(id)
  local typeof = attribute_get(id, "typeof");
  if typeof == nil then typeof = "unknown" end
  return typeof
end

function entity_north(id)
  entity_setvelocity(id, 0, 0, -player.speed)
end

function entity_south(id)
  entity_setvelocity(id, 0, 0, player.speed)
end

function entity_west(id)
  entity_setvelocity(id, -player.speed, 0, 0)
end

function entity_east(id)
  entity_setvelocity(id, player.speed, 0, 0)
end
