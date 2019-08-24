﻿#ifndef BABYLON_SHADERS_SHADERS_INCLUDE_PBR_HELPER_FUNCTIONS_FX_H
#define BABYLON_SHADERS_SHADERS_INCLUDE_PBR_HELPER_FUNCTIONS_FX_H

namespace BABYLON {

extern const char* pbrHelperFunctions;

const char* pbrHelperFunctions
  = "// Constants\n"
    "#define RECIPROCAL_PI2 0.15915494\n"
    "#define RECIPROCAL_PI 0.31830988618\n"
    "\n"
    "// AlphaG epsilon to avoid numerical issues\n"
    "#define MINIMUMVARIANCE 0.0005\n"
    "\n"
    "float convertRoughnessToAverageSlope(float roughness)\n"
    "{\n"
    "  // Calculate AlphaG as square of roughness; add epsilon to avoid numerical issues\n"
    "  return square(roughness) + MINIMUMVARIANCE;\n"
    "}\n"
    "\n"
    "float fresnelGrazingReflectance(float reflectance0) {\n"
    "  // For typical incident reflectance range (between 4% to 100%) set the grazing reflectance to 100% for typical fresnel effect.\n"
    "  // For very low reflectance range on highly diffuse objects (below 4%), incrementally reduce grazing reflecance to 0%.\n"
    "  float reflectance90 = saturate(reflectance0 * 25.0);\n"
    "  return reflectance90;\n"
    "}\n"
    "\n"
    "vec2 getAARoughnessFactors(vec3 normalVector) {\n"
    "  #ifdef SPECULARAA\n"
    "  vec3 nDfdx = dFdx(normalVector.xyz);\n"
    "  vec3 nDfdy = dFdy(normalVector.xyz);\n"
    "  float slopeSquare = max(dot(nDfdx, nDfdx), dot(nDfdy, nDfdy));\n"
    "\n"
    "  // Vive analytical lights roughness factor.\n"
    "  float geometricRoughnessFactor = pow(saturate(slopeSquare), 0.333);\n"
    "\n"
    "  // Adapt linear roughness (alphaG) to geometric curvature of the current pixel.\n"
    "  float geometricAlphaGFactor = sqrt(slopeSquare);\n"
    "  // BJS factor.\n"
    "  geometricAlphaGFactor *= 0.75;\n"
    "\n"
    "  return vec2(geometricRoughnessFactor, geometricAlphaGFactor);\n"
    "  #else\n"
    "  return vec2(0.);\n"
    "  #endif\n"
    "}\n"
    "\n"
    "#ifdef ANISOTROPIC\n"
    "  // Aniso parameter remapping\n"
    "  // https://blog.selfshadow.com/publications/s2017-shading-course/imageworks/s2017_pbs_imageworks_slides_v2.pdf page 24\n"
    "  vec2 getAnisotropicRoughness(float alphaG, float anisotropy) {\n"
    "  float alphaT = max(alphaG * (1.0 + anisotropy), MINIMUMVARIANCE);\n"
    "  float alphaB = max(alphaG * (1.0 - anisotropy), MINIMUMVARIANCE);\n"
    "  return vec2(alphaT, alphaB);\n"
    "  }\n"
    "\n"
    "  // Aniso Bent Normals\n"
    "  // Mc Alley https://www.gdcvault.com/play/1022235/Rendering-the-World-of-Far \n"
    "  vec3 getAnisotropicBentNormals(const vec3 T, const vec3 B, const vec3 N, const vec3 V, float anisotropy) {\n"
    "  vec3 anisotropicFrameDirection = anisotropy >= 0.0 ? B : T;\n"
    "  vec3 anisotropicFrameTangent = cross(normalize(anisotropicFrameDirection), V);\n"
    "  vec3 anisotropicFrameNormal = cross(anisotropicFrameTangent, anisotropicFrameDirection);\n"
    "  vec3 anisotropicNormal = normalize(mix(N, anisotropicFrameNormal, abs(anisotropy)));\n"
    "  return anisotropicNormal;\n"
    "\n"
    "  // should we also do http://advances.realtimerendering.com/s2018/Siggraph%202018%20HDRP%20talk_with%20notes.pdf page 80 ?\n"
    "  }\n"
    "#endif\n"
    "\n"
    "#if defined(CLEARCOAT) || defined(SS_REFRACTION)\n"
    "  // From beer lambert law I1/I0 = e −α′lc\n"
    "  // c is considered included in alpha\n"
    "  // https://blog.selfshadow.com/publications/s2017-shading-course/drobot/s2017_pbs_multilayered.pdf page 47\n"
    "  vec3 cocaLambert(vec3 alpha, float distance) {\n"
    "  return exp(-alpha * distance);\n"
    "  }\n"
    "\n"
    "  // where L on a thin constant size layer can be (d * ((NdotLRefract + NdotVRefract) / (NdotLRefract * NdotVRefract))\n"
    "  vec3 cocaLambert(float NdotVRefract, float NdotLRefract, vec3 alpha, float thickness) {\n"
    "  return cocaLambert(alpha, (thickness * ((NdotLRefract + NdotVRefract) / (NdotLRefract * NdotVRefract))));\n"
    "  }\n"
    "\n"
    "  // From beerLambert Solves what alpha should be for a given result at a known distance.\n"
    "  vec3 computeColorAtDistanceInMedia(vec3 color, float distance) {\n"
    "  return -log(color) / distance;\n"
    "  }\n"
    "\n"
    "  vec3 computeClearCoatAbsorption(float NdotVRefract, float NdotLRefract, vec3 clearCoatColor, float clearCoatThickness, float clearCoatIntensity) {\n"
    "  vec3 clearCoatAbsorption = mix(vec3(1.0),\n"
    "  cocaLambert(NdotVRefract, NdotLRefract, clearCoatColor, clearCoatThickness),\n"
    "  clearCoatIntensity);\n"
    "  return clearCoatAbsorption;\n"
    "  }\n"
    "#endif\n"
    "\n"
    "// ___________________________________________________________________________________\n"
    "//\n"
    "// LEGACY\n"
    "// ___________________________________________________________________________________\n"
    "\n"
    "#ifdef MICROSURFACEAUTOMATIC\n"
    "  float computeDefaultMicroSurface(float microSurface, vec3 reflectivityColor)\n"
    "  {\n"
    "  const float kReflectivityNoAlphaWorkflow_SmoothnessMax = 0.95;\n"
    "\n"
    "  float reflectivityLuminance = getLuminance(reflectivityColor);\n"
    "  float reflectivityLuma = sqrt(reflectivityLuminance);\n"
    "  microSurface = reflectivityLuma * kReflectivityNoAlphaWorkflow_SmoothnessMax;\n"
    "\n"
    "  return microSurface;\n"
    "  }\n"
    "#endif\n";

} // end of namespace BABYLON

#endif // end of BABYLON_SHADERS_SHADERS_INCLUDE_PBR_HELPER_FUNCTIONS_FX_H
