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
// Uniform

// Light
uniform Light light;

in vec3 v_normals;
in vec3 v_lightDirection;
in vec3 v_view;
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
  // Lambertion BRDF
  /*
  float ndotl = dot(n, l);
  return light.diffuse * ndotl;
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
  float ndotl_clamp = max(ndotl, 0.0);
  // n.v
  float ndotv = dot(n, v);
  float ndotv_clamp = max(ndotv, 0.0);
  // n.h
  float ndoth = dot(n, h);
  float ndoth_clamp = max(ndoth, 0.0);
  // v.h
  float vdoth = dot(v, h);
  float vdoth_clamp = max(vdoth, 0.0);
  // l.h
  float ldoth = dot(l, h);
  float ldoth_clamp = max(ldoth, 0.0);
  // Caculate F (Fresnel) using Schlicks Approximation
  float r1 = (1-ref)/(1+ref);
  // Caculate f0
  float f0 = r1 * r1;
  // Caculate inverse f0
  float invf0 = 1.0 - f0;
  // Caculate (v.h)^5
  float vdoth5 = (1.0 - ldoth_clamp) * (1.0 - ldoth_clamp) * (1.0 - ldoth_clamp) * (1.0 - ldoth_clamp) * (1.0 - ldoth_clamp);
  // Caculate F
  float F = f0 + invf0 * vdoth5;
  // Caculate G (Geometry) using Cook-Torrence
  // Caculate A
  float A = (2 * ndoth * ndotv) / vdoth;
  // Caculate B
  float B = (2 * ndoth * ndotl) / ldoth;
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
  float M = (F * G * D) / (4*ndotl*ndotv);
  return M;
}

// M = (F*G*D)/(PI * (n.l) * (n.v))

void main() {
  vec3 n = normalize(v_normals);
  vec3 l = normalize(v_lightDirection);
  vec3 v = normalize(v_view);

  float s = 1.0;

  float g = 1.0;

  float ref = 5 * s;
  float r = 1.0 - g;

  // Caculate Energy Conservation
  float r1 = (1 - ref)/(1 + ref);
  float f0 = r1 * r1;
  float invf0 = 1.0 - f0;

  if(r <= 0.05) {
    r = 1.0 - 0.95;
  }

  vec3 diffuse = light.diffuse * diffuse(n, l, v, r) * invf0;
  vec3 specular = light.specular * specular(n, l, v, r, ref);

  out_color = vec4(1.0) * vec4(diffuse, 1.0);
}
