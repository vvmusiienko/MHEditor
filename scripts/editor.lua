OP_VECTOR    =   0
OP_INTEGER   =   1
OP_FLOAT     =   2
OP_STRING    =   3
OP_COLOR     =   4
OP_BOOL      =   5
OP_EVENT     =   6
OP_LIST      =   7
OP_LABEL     =   8

-- property templates

dofile("./scripts/props_templates.lua")


editor = {}

s_editorMode(true)
moveMode = true
selectionMode = true   -- If true then on on mouse click caling in lua game.onObjectSelected(id) with id of object below cursor
appendSelect = false    -- If true then appends each new clicked object to current selection
ctrlIsDown = false
isDrag = false          -- true if click and drag; false if mousedown and up
lockAxle = false 
needRefreshTree = false
needSelectTree = true
-- for constraints
current_const = {}
current_const2 = {}

-- eval all propetios for passed obj in editor
game.setProperties = function(obj) 
  editor_clearProperties()
  
  if obj == nil then return end
  
  for i=1,#p_tpl[obj.g_type()],1 do
    local tpl = p_tpl[obj.g_type()][i]
    if tpl.type == OP_VECTOR then
      editor_addProperty(OP_VECTOR, tpl.name, obj.getFullID(), tpl.setter, obj[tpl.getter]().x, obj[tpl.getter]().y, obj[tpl.getter]().z)
    end
    if tpl.type == OP_COLOR then
      editor_addProperty(OP_COLOR, tpl.name, obj.getFullID(), tpl.setter, obj[tpl.getter]().r, obj[tpl.getter]().g, obj[tpl.getter]().b, obj[tpl.getter]().a)
    end    
    if tpl.type == OP_BOOL then
      editor_addProperty(OP_BOOL, tpl.name, obj.getFullID(), tpl.setter, obj[tpl.getter]())
    end    
    if tpl.type == OP_FLOAT then
      editor_addProperty(OP_FLOAT, tpl.name, obj.getFullID(), tpl.setter, obj[tpl.getter]())
    end    
    if tpl.type == OP_EVENT then
      editor_addProperty(OP_EVENT, tpl.name, obj.getFullID(), tpl.params)
    end    
    if tpl.type == OP_STRING then
      editor_addProperty(OP_STRING, tpl.name, obj.getFullID(), tpl.setter, obj[tpl.getter]())
    end    
    if tpl.type == OP_LABEL then
      editor_addProperty(OP_LABEL, tpl.name, obj.getFullID())
    end    
    if tpl.type == OP_LIST then
      -- dynamic list or static?
      if type(tpl.names) == "string" then
        editor_addProperty(OP_LIST, tpl.name, obj.getFullID(), tpl.setter, #obj[tpl.names](), obj[tpl.names](), obj[tpl.values](), obj[tpl.getter]())
      end
      if type(tpl.names) == "table" then
        editor_addProperty(OP_LIST, tpl.name, obj.getFullID(), tpl.setter, #tpl.names, tpl.names, tpl.values, obj[tpl.getter]())
      end
    end    
  end
end



selectedObjs = {}
game.onObjectSelected = function(_obj, append)
  -- restore color
  for i=1,#selectedObjs,1 do
    if selectedObjs[i].g_color ~= nil then
      local c = selectedObjs[i].g_color()
      if (c.r==1 and c.g==0 and c.b==0 and c.a==1) then
        selectedObjs[i].s_color(selectedObjs[i].oldColor.r, selectedObjs[i].oldColor.g, selectedObjs[i].oldColor.b, selectedObjs[i].oldColor.a) end
    end
  end

  -- if already selected then exit
  local alreadySelected = false;
  for i=1,#selectedObjs,1 do
    if selectedObjs[i] == _obj and append then alreadySelected = true;  end
  end
  
  if alreadySelected then
    table.remove(selectedObjs, i)
  else 
    if append then
      table.insert(selectedObjs, _obj)
    else
      selectedObjs = {_obj}
    end
  end

  if needSelectTree then
    for i=1,#selectedObjs,1 do
      if selectedObjs[i].treeNodeIndex then
        editor_selectTreeNode(selectedObjs[i].treeNodeIndex, append)
      end
    end
  end
  
  if #selectedObjs == 1 then
      -- get id to editor
      editor_setCurrentObjectID(selectedObjs[1].getFullID())
      game.setProperties(selectedObjs[1])
  else
      editor_setCurrentObjectID("")
      game.setProperties(nil)
  end
  
  -- mark selected and preserve color to futher restoring
  for i=1,#selectedObjs,1 do
    if selectedObjs[i].g_color ~= nil then
      selectedObjs[i].oldColor = selectedObjs[i].g_color()
      selectedObjs[i].s_color(1, 0, 0, 1)
    end
  end
  
  needSelectTree = true
  
end


-- struct for preserve mouse state
mouseState = {}
mouseState.dx = 0
mouseState.dy = 0
mouseState.lx = 0
mouseState.ly = 0

local oldMD = _onMouseDown
_onMouseDown = function(button)
  oldMD(button)
  if button == L_B then mouseState.lb = true end
  if button == M_B then mouseState.mb = true end
  if button == R_B then mouseState.rb = true end
  
  isDrag = false
end


local oldMU = _onMouseUp
_onMouseUp = function(button)
  oldMU(button)
  if button == L_B then mouseState.lb = false end
  if button == M_B then mouseState.mb = false end
  if button == R_B then mouseState.rb = false end
  
  -- selection handling
  if button == L_B and selectionMode and (not isDrag) then
    go = game.getObjectAtPoint(mouseState.x, game.getScreenHeight() - mouseState.y);
    if go then
      se(game.onObjectSelected(go, appendSelect));
    end

    if #selectedObjs == 1 then
      refreshCollisionShape()
      refreshConstraints()
    end

  end
  
  if #selectedObjs == 1 and isDrag and button == L_B then
    game.setProperties(selectedObjs[1])
  end
  
end


  -- for axle lock
lockX = 1
lockY = 1

local oldMM = _onMouseMoved
_onMouseMoved = function(x, y)
  oldMM(x,y)
  mouseState.x = x
  mouseState.y = y
  mouseState.dx = x - mouseState.lx
  mouseState.dy = y - mouseState.ly
  
  if not lockedAxleSetted and lockAxle then
    if math.abs(mouseState.dx) >= 4 then
      lockY = 0 
      lockedAxleSetted = true
    elseif math.abs(mouseState.dy) >= 4 then
      lockX = 0 
      lockedAxleSetted = true
    end
  end  
  
  if mouseState.lb and moveMode and (not lockAxle or (lockAxle and lockedAxleSetted)) then
    for i=1,#selectedObjs,1 do
      local o = selectedObjs[i]
      if o.getPosition ~= nil then
        local p = o.getPosition()
  
        o.setLinearVelocity(0,0,0)
        if g_camera_lookAt().x >= -0.75 and g_camera_lookAt().x <= 0.75 and g_camera_lookAt().z <= -0.65 then
          if ctrlIsDown then
            o.setPosition(p.x, p.y, p.z + mouseState.dy) -- 1
          else
            o.setPosition(p.x + mouseState.dx * lockX, p.y - mouseState.dy * lockY, p.z) -- 1
          end
        elseif g_camera_lookAt().z >= -0.75 and g_camera_lookAt().z <= 0.75 and g_camera_lookAt().x >= 0.65 then
          if ctrlIsDown then
            o.setPosition(p.x - mouseState.dy, p.y, p.z) -- 2
          else
            o.setPosition(p.x, p.y - mouseState.dy * lockY, p.z + mouseState.dx * lockX) -- 2
          end
        elseif g_camera_lookAt().x >= -0.75 and g_camera_lookAt().x <= 0.75 and g_camera_lookAt().z >= 0.65 then
          if ctrlIsDown then
            o.setPosition(p.x, p.y, p.z - mouseState.dy) -- 3
          else
            o.setPosition(p.x + -mouseState.dx * lockX, p.y - mouseState.dy * lockY, p.z) -- 3
          end
        elseif g_camera_lookAt().z >= -0.75 and g_camera_lookAt().z <= 0.75 and g_camera_lookAt().x <= -0.65 then
          if ctrlIsDown then
            o.setPosition(p.x + mouseState.dy, p.y, p.z) -- 2
          else
            o.setPosition(p.x, p.y - mouseState.dy * lockY, p.z + -mouseState.dx * lockX) -- 2
          end
        end
      end
    end
  end
  
  isDrag = true
  
  mouseState.lx = x
  mouseState.ly = y
end



-- Object creating

editor.createObject = function(obj_type, id)
  local cp = g_camera_position()
  cp.x = math.ceil(cp.x)
  cp.y = math.ceil(cp.y)
  
  local o = nil
  
  if obj_type == GO_CUBE then
    o = game.addChild(GO_CUBE, id)
    o.s_size( 20, 20, 20)
    o.setPosition(cp.x, cp.y, 0)
    o.objectCreated()
  end

  if obj_type == GO_GROUND then
    o = game.addChild(GO_GROUND, id)
    o.setModelPrefix("default")
    o.setTexturePrefix("grounds/default")
    o.setSize( 1, 1, 2)
    o.setPosition(cp.x, cp.y, 0)
    o.objectCreated()
  end
  
  if obj_type == GO_PLATE then
    o = game.addChild(GO_PLATE, id)
    o.s_size( 20, 20, 0)
    o.setPosition(cp.x, cp.y, 0)
    --s.setTexture("Tree_07")
    o.objectCreated()
  end

  if obj_type == GO_MODEL then
    o = game.addChild(GO_MODEL, id)
    o.setPosition(cp.x, cp.y, 0)
    o.setModel("box.md2")
    o.setTexture("box_stone.tga")
    o.objectCreated()
  end
  
  if obj_type == GO_LIGHT then
    o = game.addChild(GO_LIGHT, id)
    o.setPosition(cp.x, cp.y, 0)
    o.s_specular(1,1,1,1)
    o.s_diffuse(1,1,1,1)
    o.objectCreated()
  end  

  
  if obj_type == GO_CREATURE then
    o = game.addChild(GO_CREATURE, id)
    o.setPosition(cp.x, cp.y, 0)
    o.setModel("mario")
    o.objectCreated()
  end
  
  if obj_type == GO_UNDEFINED then
    o = game.addChild(obj_type, id)
    o.setPosition(cp.x, cp.y, 0)
    o.objectCreated()
  end

  o.s_needSaveToMap(true)
  o.s_needDeath(true)
  if #selectedObjs == 1 and selectedObjs[1].g_id() ~= "map" then
    selectedObjs[1].addChild(o);
  end
  
  refreshList()
end


deleteSelectedObjects = function ()
    for i=1,#selectedObjs,1 do
      selectedObjs[i].kill()
    end
  selectedObjs = {}
  game.setProperties(nil)
  refreshList()  
end


-- redefine draw function
local old2d = _onDraw2D;
_onDraw2D = function()
  old2d()
  if #selectedObjs == 1 then
    game.writeText(selectedObjs[1].g_id(), game.getScreenWidth() - 120, game.getScreenHeight() - 30)
    game.writeText(selectedObjs[1].g_type(), game.getScreenWidth() - 120, game.getScreenHeight() - 60)
  end
end


-- redefine iterate function
local oldi = _onIterate;
_onIterate = function(_dt)
  oldi(_dt)
  if needRefreshProps then
    if #selectedObjs == 1 then
      game.setProperties(selectedObjs[1])
    end  
    needRefreshProps = false
  end
  if needRefreshTree then
    editor_clearTree()
    editor_addNode("map", -1, -666)    
    showObjectsInTree(game.getRootObjects())
    editor_restoreLastSelection()
    needRefreshTree = false
  end


end


function checkDropPossibility(obj, newparent)
  if obj == newparent or obj.g_id() == "map" or newparent.hasParent(obj) then
    editor_setAccess(false)
    return
  end
  editor_setAccess(true)
end


function moveToParent(obj, newparent)
  obj.removeFromParent()
  if newparent.g_id() ~= "map" then
    newparent.addChild(obj)
  end
  needRefreshTree = true
end



-- OBJECT LIST

function refreshList()
    -- refresh tree
    needRefreshTree = true
end


function showObjectsInTree(objects)
  editor_pushNode()
    for i=1,#objects,1 do
      if objects[i] == nil then printf("nil in childs") end
      
      editor_addNode(objects[i].getFullID(), objects[i].g_type(), objects[i].cpointer)
      showObjectsInTree(objects[i].getChilds())
    end
  editor_popNode()
end



function checkOneObject()
  if #selectedObjs > 1 then
    showMessage("Select one object!")
    return nil;
  end
  if #selectedObjs <= 0 then
    showMessage("Select object!")
    return nil;
  end
  if selectedObjs[1] == nil then
    showMessage("Selected object is nil?!?!")
    return nil;
  end
  return selectedObjs[1];
end


function refreshCollisionShape()
    editor_cs_clearTree()

  local obj = checkOneObject()
  lastCHObject = obj
  if obj == nil then return end
  if obj.g_id() == "map" then return end
  
  shape = obj.getCollisionShape()
  if shape == nil then return end
  
  if shape.getCollisionShapeType() == GOCST_COMPOUND_SHAPE then
  -- add root element and chields
    editor_cs_addNode(shape.g_id(), shape.g_type(), -666)    
    local objects = shape.getChilds()
    editor_cs_pushNode()
      for i=1,#objects,1 do
        editor_cs_addNode(objects[i].g_id(), objects[i].g_type(), objects[i].cpointer)
      end
    editor_cs_popNode()
  
  else
  -- just add root element
    editor_cs_addNode(shape.g_id(), shape.g_type(), -666)    
  end
  
end


function deleteCollisionShape(shape)
  local rootShape = lastCHObject.getCollisionShape()
  
  if rootShape.getCollisionShapeType() == GOCST_COMPOUND_SHAPE then
    if shape ~= rootShape then -- delete child
      rootShape.removeChild(shape)
    else -- delete root shape
      lastCHObject.setCollisionShape(nil)
    end
  else -- just delete root shape
    lastCHObject.setCollisionShape(nil)
  end
  
  selectedObjs[1] = lastCHObject
  refreshCollisionShape()
  game.setProperties(nil)
end


function addCollisionShape(selected_shape, new_shape_type)
  local rootShape = lastCHObject.getCollisionShape()
  local new_shape = game.addChild(new_shape_type, "not matter")
  
  if rootShape.getCollisionShapeType() == GOCST_COMPOUND_SHAPE then
    rootShape.addChild(new_shape)
  else -- replace root with new shape
    lastCHObject.setCollisionShape(new_shape)
  end
  
  selectedObjs[1] = lastCHObject
  refreshCollisionShape()
  game.setProperties(new_shape)
end


function refreshConstraints()
  editor_clearConstraints()
  
  local obj = checkOneObject()
  if obj == nil then return end
  if obj.g_id() == "map" then return end
  lastCNObject = obj
  
  current_const = {}
  current_const2 = {}

  local objects = obj.getConstraints()
  for i=1,#objects,1 do
    editor_addConstraint(objects[i].g_id(), objects[i].g_type(), 0)
    current_const[i] = objects[i]
  end
  
  objects = obj.isSecondObjectForConstraints()
  for i=1,#objects,1 do
    editor_addConstraint(objects[i].g_id(), objects[i].g_type(), 1)
    current_const2[i] = objects[i]
  end
    
end

function selectConstraint(constraint_num, type)
  if type == 0 then
    game.onObjectSelected(current_const[constraint_num], false)
  else
    game.onObjectSelected(current_const2[constraint_num], false)
  end
end


function deleteConstraint(constraint_num)
  lastCNObject.removeConstraint(current_const[constraint_num])
  selectedObjs[1] = lastCNObject
  refreshConstraints()
  game.setProperties(nil)
end


function addConstraint(constraint_type, constraint_id, obrep)
  if lastCNObject == nil then return end
  local new_constraint = game.addChild(constraint_type, constraint_id)
  lastCNObject.addConstraint(new_constraint)
  selectedObjs[1] = lastCNObject
  refreshConstraints()
  game.setProperties(new_constraint)
end



onMapSaved = function()
  --editor_afterMapSaved()
end

new_game = function()
    game.clearMap()

    local p = game.addChild(GO_PLAYER, "player")
    p.setPosition(0, 0, 0)
    p.s_needSaveToMap(true)
    p.s_needDeath(true)
    p.objectCreated()
end

setCamPosToMap = function()
  map.s_startCameraPosition(g_camera_position().x, g_camera_position().y, g_camera_position().z)
  map.s_startCameraOrientation(g_camera_lookAt().x, g_camera_lookAt().y, g_camera_lookAt().z)
  needRefreshProps = true
end


-- INIT
new_game()
