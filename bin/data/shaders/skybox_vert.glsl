/*
  skybox_vert.glsl

  This is the skybox renderer vertex shader
*/
#version 330 core
// attributes
layout(location = 0) in vec3 vertices;
// Uniforms
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

// outputs
out vec3 v_pos;

void main() {
  mat4 v = view;
  v[3][0] = 0;
  v[3][1] = 0;
  v[3][2] = 0;
  gl_Position = projection * v * vec4(vertices, 1.0);
  v_pos = vertices;
}
