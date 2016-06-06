/*
  main_frag.glsl

  This is the main fragment shader
  for this project.
*/
#version 330 core

// Struct
struct Light {
  vec3 ambient;
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
vec3 diffuse(vec3 n, vec3 l, vec3 v, float r) {
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
  return light.diffuse * L1;
  // Lambertion BRDF
  /*
  float ndotl = dot(n, l);
  return light.diffuse * ndotl;
  */
}

float approximateReflectince(float ref) {
    /*
      Reflectince Approximation Equation
      // Schlick
      f0 = ((1-ref)/(1+ref))^2
    */
    float a = 1 - ref;
    float b = 1 + ref;
    float c = a / b;
    return c * c;
}

float fresnel(vec3 v, vec3 h, float ref) {
  /*
    Fresnel Approximation Equation
    F(f0) = f0 + (1 - f0)*(1 - l.h)^5
  */
  //
  float f0 = approximateReflectince(ref);
  // none
  //return f0;
  //Schlick Fresnel
  // dot product l and h
  float vdoth = dot(v, h);
  // pow ldoth 5
  float vdoth5 = vdoth * vdoth * vdoth * vdoth * vdoth;
  // Inverse f0^-1
  float invf0 = 1 - f0;
  // caculate fresnel and returning it.
  return f0 + invf0 * vdoth5;
  // Cook-Trrance Fresnel
  /*
  float n = (1+sqrt(f0))/(1-sqrt(f0));
  float n2 = n * n;
  float c = dot(v, h);
  float c2 = c * c;
  float g = sqrt(n2 + c2 - 1);
  float a = (g-c)/(g+c);
  float a2 = a * a;
  float b = ((g+c)*c-1)/((g-c)*c+1);
  float b2 = b * b;
  return 0.5 * a2 * (1 + b2);
  /
  // SphericalGaussian
  /*
  float vdoth = dot(v, h);
  float invf0 = 1 - f0;
  float ex = (-5.55473*vdoth-6.98316)*vdoth;
  return f0 + invf0 * pow(2, ex);
  */
}

float distrobution(vec3 n, vec3 h, float r) {
  // Blinn-Phong
  // compute alpha for r
  /*
  float r2 = r * r;
  float nf = 1 / (3.14 * r2);
  float alpha = 2 / r2 - 2;
  // compute n.h
  float ndoth = dot(n, h);
  return nf * pow(ndoth, alpha);
  */
  // Beckman
  // dot product between n and h
  /*
  float ndoth = dot(n, h);
  // (n.h)^2
  float ndoth2 = ndoth * ndoth;
  // (n.h)^4
  float ndoth4 = ndoth * ndoth * ndoth * ndoth;
  // r^2
  float r2 = r * r;
  // a = exp((n.h^2 - 1)/(r^2 * (n.h)^2))
  float a = exp((ndoth2 - 1) / (r2 * ndoth2));
  // b = PI * r^2 * n.h^4
  float b = 3.14 * r2 * ndoth4;
  // D = a / b
  return a / b;
  */
  // GGX (Trowbridge-Reitz)
  // Square r
  float r2 = r * r;
  float ndoth = dot(n, h);
  float ndoth2 = ndoth * ndoth;
  float a = ndoth2 * (r2 - 1) + 1;
  float a2 = a * a;
  return r2 / (3.14 * a2);
  // GGX Anisotropic
  // Can't do because I don't know what x and y are
}

float geometry(vec3 n, vec3 h, vec3 v, vec3 l, float r) {
  /*
    Implicit Geometry Equation
  */
  /*
  float ndotl = dot(n, l);
  float ndotv = dot(n, v);
  return ndotl = ndotl * ndotv;
  */
  // Caculating Cook Torrence Geometry Term
  float ndoth = dot(n, h);
  float ndotv = dot(n, v);
  float ndotl = dot(n, l);
  float vdoth = dot(v, h);
  float ldoth = dot(l, h);
  // A = (2 * n.h * n.v) / v.h
  float A = (2 * ndoth * ndotv) / vdoth;
  // B = (2 * n.h * n.l) / l.h
  float B = (2 * ndoth * ndotl) / ldoth;
  // C = min(A, B)
  float C = min(A, B);
  // G = min(1, C)
  return min(1, C);
  /*
  // Neumann
  float ndotl = dot(n, l);
  float ndotv = dot(n, v);

  float a = ndotl * ndotv;
  float b = max(ndotl, ndotv);

  return a / b;
  */
  // Kelemen
  // n.l
  /*
  float ndotl = dot(n, l);
  // n.v
  float ndotv = dot(n, v);
  // v.h
  float vdoth = dot(v, h);
  // a = (n.l) * (n.v)
  float a = ndotl * ndotv;
  // b = (v.h)^2
  float b = vdoth * vdoth;
  // c = a / b
  return a / b;
  */
  // Smith G
  //return G1(n, l, h, r) * G1(n, v, h, r);
  /*
  float r2 = r * r;
  float k = sqrt((2*r2)/3.14);
  float ndotv = dot(n, v);
  float a = ndotv * (1 - k) + k;
  return ndotv / a;
  */
  /*
  float r2 = r * r;
  float k = r2 / 2.0;
  float k2 = k * k;
  float invK2 = 1.0 - k2;
  float ndotl = dot(n, l);
  return 1.0 / (ndotl * ndotl * invK2 + k2);
  */

  //return 1.0 / g1(n, l, r) * g1(n, v, r);
  /*
  float alpha = r * r;
  float ndotl = dot(n, l);
  float ndotv = dot(n, v);
  float ndotl2 = ndotl * ndotl;
  float ndotv2 = ndotv * ndotv;

  float A = ndotl / (alpha * sqrt(1 - ndotl2));
  float B = ndotv / (alpha * sqrt(1 - ndotv2));

  return (A * B);
  */
}

vec3 specular(vec3 n, vec3 l, vec3 v, float r, float ref) {
  /*
    microfacet equation

    f = (F(l, h) * G(l, v, h) * D(h)) / (4*(n.l)*(n.v))

    l = light direction
    v = viewer direction
    n = surface normal direction
    h = half vector between l and v
    F = fresnel term
    G = geometry term
    D = distrobution term
  */

  // microfacet brdf
  // Half Vector
  vec3 h = normalize(l + v);
  // Caculate dot product between n and l
  float ndotl = dot(n, l);
  // Caculate dot product between n and v
  float ndotv = dot(n, v);
  // Caculate F
  float F = fresnel(v, h, ref);
  // Caculate D
  float D = distrobution(n, h, r);
  // Caculate G
  float G = geometry(n, h, v, l, r);
  // Caculate final product
  float f = (F * D * G) / (4 * ndotl * ndotv);

  return light.specular * f;
  /*
  vec3 h = normalize(l + v);
  float ndoth = dot(n, h);
  float r2 = r * r;
  float alpha = 2 / r2 - 2;
  return light.specular * pow(ndoth, alpha);
  */
}

void main() {
  vec3 n = normalize(v_normals);
  vec3 l = normalize(v_lightDirection);
  vec3 v = normalize(v_view);

  float ref = 2;
  float r = 1.0 - 0.1;

  if(r <= 0.05) {
    r = 1.0 - 0.95;
  }

  vec3 ambient = light.ambient;
  vec3 diffuse = diffuse(n, l, v, r);
  vec3 specular = specular(n, l, v, r, ref);

  out_color = vec4(1.0) * vec4(diffuse + specular, 1.0);
}
