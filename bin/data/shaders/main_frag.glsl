/*
  main_frag.glsl

  This is the main fragment shader
  for this project.
*/
#version 330 core

// Struct
struct Light {
  vec3 diffuse;
  vec3 specular;
  vec3 position;
};

struct Material {
  vec3 albedo; // Color of the object, def <0, 0, 0>
  float fresnel; // Capted from [0, 1] def <0>
  float roughness; // Capted from [0, 1] <0>
  sampler2D albedoMap; // Base Color for object.
  sampler2D roughnessMap; // Rounghess of object
  sampler2D reflectiveMap; // ReflectiveMap
  sampler2D normalMap; // Normal Map
  sampler2D heightMap; // Height Map for POM
};

// Uniform

// Light
uniform Light light;
uniform Material material;

//in vec3 v_normals;
in mat3 v_tbn;
in vec3 v_lightDirection;
in vec3 v_view;
in vec3 v_tangentView;
in vec2 v_texCoords;

// Varying ( comming from vertex shader)
out vec4 out_color;

// Oren Nayar
// http://ruh.li/GraphicsOrenNayar.html
float diffuse(vec3 n, vec3 l, vec3 v, float r) {
  /*
    The "simple" version of Oren-Nayar
    from web page.

    I altered it somewhat because it had a couple
    of dot products when calculating gamma that
    wasn't needed because they were caculated
    already. I'll through the guy an email
    to the guy.
  */
  float ndotl = dot(n, l);
  float ndotv = dot(n, v);
  float angleLN = acos(ndotl);
  float angleVN = acos(ndotv);
  float alpha = max(angleVN, angleLN);
  float beta = min(angleVN, angleLN);
  // This has been altered, using ndotv and ndotl rather than
  // redoing it in the
  float gamma = dot(v - n * ndotv, l - n * ndotl);
  float r2 = r * r;
  float a = 1.0 - 0.5 * (r2 / (r2 + 0.57));
  float b = 0.45 * (r2 / (r2 + 0.09));
  float c = sin(alpha) * tan(beta);
  float L1 = max(0.0, ndotl) * (a + b * max(0.0, gamma) * c);
  return L1;
  /*
  // Lambertion BRDF
  float ndotl = dot(n, l);
  return ndotl;
  */
}

/*
  directSpecular -- This contains the direct
  RGE microfacet BRDF
*/
float specular(vec3 n, vec3 l, vec3 v, float r, float ref) {
  // Caculate alpha
  float alpha = r * r;
  // Caculate Half-Vector between l and v
  vec3 h = normalize(l + v);
  // n.l
  float ndotl = dot(n, l);
  float ndotl_clamp = clamp(ndotl, 0, 1);
  // n.v
  float ndotv = dot(n, v);
  float ndotv_clamp = clamp(ndotv, 0, 1);
  // n.h
  float ndoth = dot(n, h);
  float ndoth_clamp = clamp(ndoth, 0, 1);
  // v.h
  float vdoth = dot(v, h);
  float vdoth_clamp = clamp(vdoth, 0, 1);
  // l.h
  float ldoth = dot(l, h);
  float ldoth_clamp = clamp(ldoth, 0, 1);

  // Caculate F (Fresnel) using Schlicks Approximation
  float r1 = (1-ref)/(1+ref);
  // Caculate f0
  float f0 = r1 * r1;
  // Caculate inverse f0
  float invf0 = 1.0 - f0;
  // Caculate (v.h)^5
  float vdoth5_clamp = (1.0 - ldoth_clamp) * (1.0 - ldoth_clamp) * (1.0 - ldoth_clamp) * (1.0 - ldoth_clamp) * (1.0 - ldoth_clamp);
  // Caculate F
  float F = f0 + invf0 * vdoth5_clamp;
  // Caculate G (Geometry) using Cook-Torrence
  // Caculate A
  float A = (2 * ndoth * ndotv) / vdoth;
  // Caculate B
  float B = (2 * ndoth * ndotl) / vdoth;
  // Caculate C
  float C = min(A, B);
  float G = min(1, C);
  // Caculate D (Distrobution) using GGX(Trowbridge-Reitze)
  // Caculate Alpha2
  float alpha2 = alpha * alpha;
  // Caculate (n.h)^2
  float ndoth2 = ndoth * ndoth;
  // Caculate R
  float R = ndoth2 * (alpha2 - 1) + 1;
  // Caculate R2
  float R2 = R * R;
  // Caculate D
  float D = alpha2 / (3.14 * R2);
  // Caculate Microfacet using (F*G*D)/(4*(n.l)(n.v))
  // Caculate M
  float M = (F * G * D) / (3.14 * ndotl * ndotv);
  return M;
}

// M = (F*G*D)/(PI * (n.l) * (n.v))
vec3 computeNormal(vec2 texCoord) {
  vec3 normal;
  normal = texture(material.normalMap, texCoord).rgb * 2.0 - 1.0;
  normal = normalize(v_tbn * normal);
  return normal;
}

/*
vec2 parallaxMapping(vec2 texCoord, vec3 view) {
  const float heightScale = 0.03;
  float height = texture(material.heightMap, texCoord).r;
  vec2 p = view.xy / view.z * (height * heightScale);
  return texCoord - p;
}

vec2 steepParallaxMapping(vec2 texCoord, vec3 view) {
  const float heightScale = 0.05;
  const float numLayers = 10;
  float layerHeight = 1.0 / numLayers;
  float currentLayerHeight = 0.0;
  vec2 P = view.xy * heightScale;
  vec2 deltaTexCoords = P / numLayers;
  // get Initialize Value
  vec2 currentTexCoords = texCoord;
  float defHeight = texture(material.heightMap, currentTexCoords).r;
  float height = defHeight;

  while(currentLayerHeight < height) {
    currentTexCoords -= deltaTexCoords;
    height = defHeight;
    currentLayerHeight += layerHeight;
  }

  return currentTexCoords;
}
vec2 parallaxOcclusionMapping(vec2 texCoord, vec3 view) {
  const float heightScale = 0.05;
  const float numLayers = 10;
  float layerHeight = 1.0 / numLayers;
  float currentLayerHeight = 0.0;
  vec2 P = view.xy * heightScale;
  vec2 deltaTexCoords = P / numLayers;
  // get Initialize Value
  vec2 currentTexCoords = texCoord;
  float defHeight = texture(material.heightMap, currentTexCoords).r;
  float height = defHeight;

  while(currentLayerHeight < height) {
    currentTexCoords -= deltaTexCoords;
    height = defHeight;
    currentLayerHeight += layerHeight;
  }

  // Get Texture Coordinates before collision (reverse operation)
  vec2 prevTexCoords = currentTexCoords + deltaTexCoords;
  // get depth after and before collision for linear iterp
  float afterHeight = height - currentLayerHeight;
  float beforeHeight = defHeight - currentLayerHeight + layerHeight;

  // Iterpolation of texture currentTexCoords
  float weight = afterHeight / (afterHeight - beforeHeight);
  vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

  return finalTexCoords;
}
*/

void main() {
  vec3 v = normalize(v_view);
  vec3 n = computeNormal(v_texCoords);
  vec3 l = normalize(v_lightDirection);

  float ref = 19.0 * (material.fresnel + texture(material.reflectiveMap, v_texCoords).r) + 1;
  float r = (material.roughness + texture(material.roughnessMap, v_texCoords).r);

  // Caculate Energy Conservation
  float r1 = (1 - ref)/(1 + ref);
  float f0 = r1 * r1;
  float invf0 = 1.0 - f0;

  if(r <= 0.05) {
    r = 1.0 - 0.95;
  }

  vec3 diffuse = light.diffuse * diffuse(n, l, v, r) * invf0;
  vec3 specular = light.specular * specular(n, l, v, r, ref);

  vec3 color = (diffuse + specular) * (material.albedo + texture(material.albedoMap, v_texCoords).rgb);

  //color = pow(color, vec3(1.0/2.2));

  out_color = vec4(color, 1.0);
  //out_color = texture(material.albedoMap, v_texCoords);
  //out_color = vec4(1.0);
}
