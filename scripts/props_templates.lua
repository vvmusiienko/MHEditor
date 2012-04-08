-- property templates

common_tpl = {
  {type=OP_LABEL, name="Other"},
  {type=OP_LIST, name="Collision Shape Type", setter="setCollisionShapeType", getter="getCollisionShapeType", names={"AUTOMATIC", "AUTOMATIC_GRAPHIC_MESH", "AUTOMATIC_BOX",
  "AUTOMATIC_SPHERE", "AUTOMATIC_CYLINDER", "CUSTOM"}, values={"0", "1", "2", "3", "4", "5", "6"}},
  {type=OP_BOOL,  name="Enable Deactivation", setter="setEnableDeactivation", getter="isEnableDeactivation"},
  {type=OP_VECTOR, name="Angular Factor", setter="setAngularFactor", getter="getAngularFactor"},
  {type=OP_VECTOR, name="Linear Factor", setter="setLinearFactor", getter="getLinearFactor"},
  {type=OP_FLOAT,  name="Linear Dumping", setter="setLinearDumping", getter="getLinearDumping"},
  {type=OP_FLOAT,  name="Angular Dumping", setter="setAngularDumping", getter="getAngularDumping"},
  {type=OP_FLOAT,  name="Friction", setter="setFriction", getter="getFriction"},
  {type=OP_FLOAT,  name="Restitution", setter="setRestitution", getter="getRestitution"},
  {type=OP_FLOAT,  name="Mass", setter="setMass", getter="getMass"},
  {type=OP_BOOL,  name="Trigger", setter="setTrigger", getter="isTrigger"},
  {type=OP_BOOL,  name="Enable Physics", setter="enablePhysics", getter="isEnabledPhysics"},

  
  {type=OP_LABEL, name="Physic Properties"},
  
  {type=OP_BOOL,  name="Opaque", setter="s_opaque", getter="g_opaque"},
  {type=OP_FLOAT,  name="Alpha", setter="setAlpha", getter="getAlpha"},
  {type=OP_BOOL,  name="Hidden", setter="setHidden", getter="getHidden"},
  {type=OP_BOOL,  name="Enabled", setter="setEnabled", getter="getEnabled"},
  {type=OP_BOOL,  name="Lighting", setter="enableLighting", getter="isLightingEnabled"},
  {type=OP_COLOR,  name="Color", setter="s_color", getter="g_color"},
  {type=OP_VECTOR, name="Scale", setter="setScale", getter="getScale"},
  {type=OP_VECTOR, name="Orientation", setter="setOrientation", getter="getOrientation"},
  {type=OP_VECTOR, name="Position", setter="setPosition", getter="getPosition"},

  
  {type=OP_EVENT, name="onIterate", params="(this)"},
  {type=OP_EVENT, name="onPhysicTick", params="(this, dt)"},
  {type=OP_EVENT, name="onContactOccured", params="(this, target, contactInfo)"},
  {type=OP_EVENT, name="onContactLost", params="(this, target, contactInfo)"},
  {type=OP_EVENT, name="onAddedToWorld", params="(this)"},
  {type=OP_EVENT, name="onWillRemoveFromWorld", params="(this)"},
  
  {type=OP_EVENT, name="onCreate", params="(this)"},
  
}

p_tpl = {}
p_tpl[GO_CUBE] = {
  {type=OP_VECTOR, name="Size", setter="setSize", getter="getSize"},
}
jt(p_tpl[GO_CUBE], common_tpl);

p_tpl[GO_GROUND] = {
  {type=OP_VECTOR, name="Size", setter="setSize", getter="getSize"},
  {type=OP_STRING, name="Model Pref", setter="setModelPrefix", getter="g_modelPrefix"},
  {type=OP_STRING, name="Tex Pref", setter="setTexturePrefix", getter="g_texturePrefix"},
}
jt(p_tpl[GO_GROUND], common_tpl);

p_tpl[GO_PLATE] = {
  {type=OP_FLOAT, name="Density", setter="setDensity", getter="g_density"},
  {type=OP_VECTOR, name="Size", setter="setSize", getter="g_size"},
  {type=OP_STRING, name="Texture", setter="setTexture", getter="g_textureName"},
}
jt(p_tpl[GO_PLATE], common_tpl);

p_tpl[GO_MODEL] = {
  {type=OP_VECTOR, name="Texture Scale", setter="setTexScale", getter="g_texScale"},
  {type=OP_FLOAT, name="Frame Rate", setter="setFrameRate", getter="g_frameRate"},
  {type=OP_STRING, name="Model", setter="setModel", getter="g_model"},
  {type=OP_STRING, name="Texture", setter="setTexture", getter="g_texture"},
  {type=OP_LIST, name="Animation", setter="setAnim", getter="getCurrentAnim", names="getAnimNames", values="getAnimNames"},
}
jt(p_tpl[GO_MODEL], common_tpl);


p_tpl[GO_CREATURE] = {
  {type=OP_STRING, name="Model", setter="setModel", getter="getModelName"},
  {type=OP_STRING, name="Texture", setter="setTexture", getter="getTextureName"},
}
jt(p_tpl[GO_CREATURE], common_tpl);


p_tpl[GO_PLAYER] = {
  {type=OP_FLOAT, name="SPEED_1", setter="s_SPEED_1", getter="g_SPEED_1"},
  {type=OP_FLOAT, name="SPEED_2", setter="s_SPEED_2", getter="g_SPEED_2"},

}
jt(p_tpl[GO_PLAYER], common_tpl);


p_tpl[GO_MAP_PROPS] = {
  {type=OP_COLOR, name="Ambient Light", setter="s_ambientLight", getter="g_ambientLight"},
  {type=OP_COLOR, name="Clear Color", setter="s_clearColor", getter="g_clearColor"},
  {type=OP_STRING, name="SkyBox Texture", setter="s_skyBox", getter="g_skyBox"},
  {type=OP_VECTOR, name="Cam Pos", setter="s_startCameraPosition", getter="g_startCameraPosition"},
  {type=OP_VECTOR, name="Cam Orient", setter="s_startCameraOrientation", getter="g_startCameraOrientation"},

  {type=OP_EVENT, name="onMapLoaded", params="()"},
  {type=OP_EVENT, name="onIterate", params="()"},
  {type=OP_EVENT, name="onDraw2D", params="()"},
  {type=OP_EVENT, name="onMouseUp", params="(mouseButton)"},
  {type=OP_EVENT, name="onMouseDown", params="(mouseButton)"},
  {type=OP_EVENT, name="onMouseMoved", params="(x, y)"},
  
}


p_tpl[GO_LIGHT] = {
  {type=OP_LIST, name="Light Type", setter="s_lightType", getter="g_lightType", names={"DIRECTIONAL", "POSITIONAL", "SPOT"}, values={"0", "1", "2"}},
  {type=OP_VECTOR, name="Position", setter="setPosition", getter="getPosition"},
  {type=OP_VECTOR, name="Orientation", setter="setOrientation", getter="getOrientation"},
  {type=OP_COLOR, name="Diffuse", setter="s_diffuse", getter="g_diffuse"},
  {type=OP_COLOR, name="Specular", setter="s_specular", getter="g_specular"},
  {type=OP_VECTOR, name="Attenuation", setter="s_attenuation", getter="g_attenuation"},
  {type=OP_FLOAT, name="Spot Exponent", setter="s_spotExponent", getter="g_spotExponent"},
  {type=OP_FLOAT, name="Spot Cutoff", setter="s_spotCutoff", getter="g_spotCutoff"},
  
  
  {type=OP_EVENT, name="onIterate", params="(this)"},
}

p_tpl[GO_UNDEFINED] = {
}
jt(p_tpl[GO_UNDEFINED], common_tpl);

-- Collosion Bodyes
common_collision_tpl = {
  {type=OP_VECTOR, name="Orientation", setter="setOrientation", getter="getOrientation"},
  {type=OP_VECTOR, name="Offset", setter="setOffset", getter="getOffset"},
}

p_tpl[GO_COMPOUND_CS] = {
}
jt(p_tpl[GO_COMPOUND_CS], common_collision_tpl);

p_tpl[GO_BOX_CS] = {
  {type=OP_VECTOR, name="Size", setter="setSize", getter="getSize"},
}
jt(p_tpl[GO_BOX_CS], common_collision_tpl);

p_tpl[GO_SPHERE_CS] = {
  {type=OP_FLOAT, name="Diameter", setter="setDiameter", getter="getDiameter"},
}
jt(p_tpl[GO_SPHERE_CS], common_collision_tpl);

p_tpl[GO_CYLINDER_CS] = {
  {type=OP_VECTOR, name="Size", setter="setSize", getter="getSize"},
}
jt(p_tpl[GO_CYLINDER_CS], common_collision_tpl);

p_tpl[GO_CAPSULE_CS] = {
  {type=OP_FLOAT, name="Diameter", setter="setDiameter", getter="getDiameter"},
  {type=OP_FLOAT, name="Height", setter="setHeight", getter="getHeight"},
}
p_tpl[GO_MESH_CS] = {
  {type=OP_STRING, name="From Model", setter="createFromModel", getter="getModelName"},
}


jt(p_tpl[GO_CAPSULE_CS], common_collision_tpl);

p_tpl[GO_CONE_CS] = {
  {type=OP_FLOAT, name="Diameter", setter="setDiameter", getter="getDiameter"},
  {type=OP_FLOAT, name="Height", setter="setHeight", getter="getHeight"},
}
jt(p_tpl[GO_CONE_CS], common_collision_tpl);


p_tpl[GO_UNDEFINED_CS] = {
}


-- Constraints

common_constraint_tpl = {
  {type=OP_VECTOR, name="Second Pivot", setter="setSecondPivot", getter="getSecondPivot"},
  {type=OP_STRING, name="Second Object", setter="setSecondObjectID", getter="getSecondObjectID"},
  {type=OP_VECTOR, name="Pivot", setter="setPivot", getter="getPivot"},
}

p_tpl[GO_P2P_CONSTRAINT] = {
}
jt(p_tpl[GO_P2P_CONSTRAINT], common_constraint_tpl);

p_tpl[GO_HINGE_CONSTRAINT] = {
  {type=OP_VECTOR, name="Second Axis", setter="setSecondAxis", getter="getSecondAxis"},
  {type=OP_VECTOR, name="Axis", setter="setAxis", getter="getAxis"},
}
jt(p_tpl[GO_HINGE_CONSTRAINT], common_constraint_tpl);

p_tpl[GO_SLIDER_CONSTRAINT] = {
  {type=OP_VECTOR, name="Second Axis", setter="setSecondAxis", getter="getSecondAxis"},
  {type=OP_VECTOR, name="Axis", setter="setAxis", getter="getAxis"},
}
jt(p_tpl[GO_SLIDER_CONSTRAINT], common_constraint_tpl);
