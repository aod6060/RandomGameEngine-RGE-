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
in vec3 vertices;
in vec2 texCoords;
in vec3 normals;
// Uniforms
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 normalMatrix; // transpose(inverse(model))
uniform vec3 cameraPos;
// Light
uniform Light light;
// Varying
out vec3 v_normals;
out vec3 v_lightDirection;
out vec3 v_view;

void main() {
  gl_Position = projection * view * model * vec4(vertices, 1.0);
  // Caculating Lighting stuff I'll need
  vec3 posModel = (model * vec4(vertices, 1.0)).xyz;
  // Caculating Light
  v_lightDirection = normalize(-light.position);
  // Calculating normals
  v_normals = normalize((normalMatrix * vec4(normals, 0.0)).xyz);
  // Caculate View
  v_view = normalize(cameraPos - posModel);
}
