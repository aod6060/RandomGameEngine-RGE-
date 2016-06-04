/*
  main_vert.glsl

  This is the project main vertex shader
*/
#version 330 core

// Vertices
in vec3 vertices;
in vec2 texCoords;
in vec3 normals;
// Uniforms
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
// Varying

void main() {
  gl_Position = projection * view * model * vec4(vertices, 1.0);
}
