/*
  main_vert.glsl

  This is the project main vertex shader
*/
#version 330 core

// Struct
struct Light {
  vec3 diffuse;
  vec3 specular;
  vec3 position;
};
// Vertices
layout(location = 0) in vec3 vertices;
layout(location = 1) in vec2 texCoords;
layout(location = 2) in vec3 normals;
layout(location = 3) in vec3 tangents;
layout(location = 4) in vec3 bitangents;
// Uniforms
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 normalMatrix; // transpose(inverse(model))
uniform vec3 cameraPos;
// Light
uniform Light light;
// Varying
//out vec3 v_normals;
out mat3 v_tbn;
out vec3 v_lightDirection;
out vec3 v_view;
out vec3 v_tangentView;
out vec2 v_texCoords;

void main() {
  gl_Position = projection * view * model * vec4(vertices, 1.0);
  // Caculating Lighting stuff I'll need
  vec3 posModel = (model * vec4(vertices, 1.0)).xyz;
  // Caculating Light
  // Build Matrix
  vec3 t = normalize(vec3(normalMatrix * vec4(tangents, 0.0)));
  vec3 b = normalize(vec3(normalMatrix * vec4(bitangents, 0.0)));
  vec3 n = normalize(vec3(normalMatrix * vec4(normals, 0.0)));
  v_tbn = mat3(t, b, n);
  v_lightDirection = normalize(-light.position);
  // Calculating normals
  //v_normals = normalize((normalMatrix * vec4(normals, 0.0)).xyz);
  // Caculate View
  v_view = normalize(cameraPos - posModel);
  v_tangentView = v_tbn * v_view;
  // Grabbing Texture Coords
  v_texCoords = texCoords;
}
