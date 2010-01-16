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
toggle = {}

function toggle.register()
  game_entityregistersymbolforname("[]", "toggle")
  if toggle.platemeshid == nil then toggle.platemeshid = mesh_nametoid("plate.lua") end
end

function toggle.ison(id)
  return (attribute_get(id, "east") and attribute_get(id, "west") and attribute_get(id, "north") and attribute_get(id, "south"))
end

-------------------------------------------------------------------------------
toggle.initial = {}

function toggle.initial.init(id, attr)
  entity_setrenderable(id, true)
  entity_setpostrenderable(id, true)
  entity_setcollidable(id, true)
  entity_setmesh(id, "player.lua")
  entity_settexture(id, "toggle.png")
  attribute_set(id, "east", false)
  attribute_set(id, "west", false)
  attribute_set(id, "north", false)
  attribute_set(id, "south", false)
  game.numtoggles = game.numtoggles + 1
end

function toggle.initial.update(id, tpf)
end

function toggle.initial.collision(id, oid)
  if entity_isname(oid, "player") then
    local side = entity_onwhichsideisother(id, oid)
    local x, y, z = entity_getlocation(id)
    local vx, vy, vz = entity_getvelocity(oid)
    if     side == "north" then
      if vz > 0 then
        block.initial.collision(id, oid)
        attribute_set(id, side, true)
      end
    elseif side == "south" then
      if vz < 0 then
        block.initial.collision(id, oid)
        attribute_set(id, side, true)
      end
    elseif side == "east"  then
      if vx < 0 then
        block.initial.collision(id, oid)
        attribute_set(id, side, true)
      end
    elseif side == "west"  then
      if vx > 0 then
        block.initial.collision(id, oid)
        attribute_set(id, side, true)
      end
    end
  end

  if toggle.ison(id) then
    entity_setstate(id, "final")
    player.numtoggles = player.numtoggles + 1
  end
end

function toggle.initial.event(id, evt)
end

function toggle.initial.render(id)
  opengl_pushenablebitattrib()
  opengl_disable2dtexturing()
  opengl_disablelighting()

  if attribute_get(id, "east") then opengl_color3(0, 1, 0) else opengl_color3(1, 0, 0) end
  opengl_pushmatrix()
  opengl_translate(0.9, 0, 0)
  opengl_rotate(90, 0, 1, 0)
  opengl_render(toggle.platemeshid);
  opengl_popmatrix()

  if attribute_get(id, "west") then opengl_color3(0, 1, 0) else opengl_color3(1, 0, 0) end
  opengl_pushmatrix()
  opengl_translate(-0.9, 0, 0)
  opengl_rotate(-90, 0, 1, 0)
  opengl_render(toggle.platemeshid);
  opengl_popmatrix()

  if attribute_get(id, "north") then opengl_color3(0, 1, 0) else opengl_color3(1, 0, 0) end
  opengl_pushmatrix()
  opengl_translate(0, 0, -0.9)
  opengl_rotate(180, 1, 0, 0)
  opengl_render(toggle.platemeshid);
  opengl_popmatrix()

  if attribute_get(id, "south") then opengl_color3(0, 1, 0) else opengl_color3(1, 0, 0) end
  opengl_pushmatrix()
  opengl_translate(0, 0, 0.9)
  opengl_render(toggle.platemeshid);
  opengl_popmatrix()

  opengl_popattrib()
  opengl_color3(1, 1, 1)
end

-------------------------------------------------------------------------------
toggle.final = {}

function toggle.final.update(id, tpf)
end

function toggle.final.collision(id, oid)
  block.initial.collision(id, oid)
end

function toggle.final.event(id, evt)
end

function toggle.final.render(id)
  toggle.initial.render(id)
end
