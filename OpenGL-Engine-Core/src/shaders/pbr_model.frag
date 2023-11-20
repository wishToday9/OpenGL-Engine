#version 450 core
/*
This IBL - BRDF Integration approach is based on the approach Epic made in their paper "Real Shading in Unreal Engine 4"
*/
struct Material {
	sampler2D texture_albedo;
	sampler2D texture_normal;
	sampler2D texture_metallic;
	sampler2D texture_roughness;
	sampler2D texture_ao;
};

struct DirLight {
	vec3 direction;
	vec3 lightColor; // radiant flux
};

struct PointLight {
	vec3 position;

	vec3 lightColor; // radiant flux
};

struct SpotLight {
	vec3 position;
	vec3 direction;

	float cutOff;
	float outerCutOff;

	vec3 lightColor; // radiant flux
};
#define MAX_DIR_LIGHTS 5
#define MAX_POINT_LIGHTS 5
#define MAX_SPOT_LIGHTS 5

const float PI = 3.14159265359;

in mat3 TBN;
in vec3 FragPos;
in vec4 FragPosLightClipSpace;
in vec2 TexCoords;

out vec4 color;

uniform vec3 viewPos;

// IBL
uniform int reflectionProbeMipCount;
uniform bool computeIBL;
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;

//lighting
uniform sampler2D shadowmap;
uniform ivec4 numDirPointSpotLights;

uniform DirLight dirLights[MAX_DIR_LIGHTS];
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform Material material;


// Light radiance calculations
vec3 CalculateDirectionalLightRadiance(vec3 albedo, vec3 normal, float metallic, float roughness, vec3 fragToView, vec3 baseReflectivity);
vec3 CalculatePointLightRadiance(vec3 albedo, vec3 normal, float metallic, float roughness, vec3 fragToView, vec3 baseReflectivity);
vec3 CalculateSpotLightRadiance(vec3 albedo, vec3 normal, float metallic, float roughness, vec3 fragToView, vec3 baseReflectivity);

// Cook-Torrance BRDF function prototypes
float NormalDistributionGGX(vec3 normal, vec3 halfway, float roughness);
float GeometrySmith(vec3 normal, vec3 viewDir, vec3 lightDir, float roughness);
float GeometrySchlickGGX(float cosTheta, float roughness);
vec3 FresnelSchlick(float cosTheta, vec3 baseReflectivity);

// Other function prototypes
float CalculateShadow(vec3 normal, vec3 fragToDirLight);


void main() {
	//smple textures
	vec3 albedo = texture(material.texture_albedo, TexCoords).rgb;
	float albedoAlpha = texture(material.texture_albedo, TexCoords).w;
	vec3 normal = texture(material.texture_normal, TexCoords).rgb;
	float metallic = texture(material.texture_metallic, TexCoords).r;
	float unclampedRoughness = texture(material.texture_roughness, TexCoords).r; // Used for indirect specular (reflections)
	float roughness = max(unclampedRoughness, 0.04); // Used for calculations since specular highlights will be too fine, and will cause flicker

	float ao = texture(material.texture_ao, TexCoords).r;

	//normal mapping 
	normal = normalize(normal * 2.0 - 1.0f);
	normal = normalize(TBN * normal);

	vec3 fragToView = normalize(viewPos - FragPos);
	vec3 reflectionVec = reflect(-fragToView, normal);

	vec3 baseReflectivity = vec3(0.04);
	baseReflectivity = mix(baseReflectivity, albedo, metallic);

	// Calculate per light radiance for all of the direct lighting
	vec3 directLightIrradiance = vec3(0.0);
	directLightIrradiance += CalculateDirectionalLightRadiance(albedo, normal, metallic, roughness, fragToView, baseReflectivity);
	directLightIrradiance += CalculatePointLightRadiance(albedo, normal, metallic, roughness, fragToView, baseReflectivity);
	directLightIrradiance += CalculateSpotLightRadiance(albedo, normal, metallic, roughness, fragToView, baseReflectivity);

	//IBL for both diffuse and specular
	// Calcualte ambient IBL for both diffuse and specular
	vec3 ambient = vec3(0.03) * albedo * ao;
	if (computeIBL) {
		vec3 specularRatio = FresnelSchlick(max(dot(normal, fragToView), 0.0), baseReflectivity);
		vec3 diffuseRatio = vec3(1.0) - specularRatio;
		diffuseRatio *= 1.0 - metallic;

		vec3 indirectDiffuse = texture(irradianceMap, normal).rgb * albedo;

		vec3 prefilterColour = textureLod(prefilterMap, reflectionVec, unclampedRoughness * (reflectionProbeMipCount - 1)).rgb;
		vec2 brdfIntegration = texture(brdfLUT, vec2(max(dot(normal, fragToView), 0.0), roughness)).rg;
		vec3 indirectSpecular = prefilterColour * (specularRatio * brdfIntegration.x + brdfIntegration.y);

		ambient = (diffuseRatio * indirectDiffuse + indirectSpecular) * ao;
	}

	color = vec4(ambient + directLightIrradiance, albedoAlpha);
}


// Light radiance calculations
vec3 CalculateDirectionalLightRadiance(vec3 albedo, vec3 normal, float metallic,
	float roughness, vec3 fragToView, vec3 baseReflectivity)
{
	vec3 directLightIrradiance = vec3(0.0);

	for (int i = 0; i < numDirPointSpotLights.x; ++i) {
		vec3 lightDir = normalize(-dirLights[i].direction);
		vec3 halfway = normalize(lightDir + fragToView);
		vec3 radiance = dirLights[i].lightColor;

		// Cook-Torrance Specular BRDF calculations
		float normalDistribution = NormalDistributionGGX(normal, halfway, roughness);
		vec3 fresnel = FresnelSchlick(max(dot(halfway, fragToView), 0.0), baseReflectivity);
		float geometry = GeometrySmith(normal, fragToView, lightDir, roughness);

		// Calculate reflected and refracted light respectively, and since metals absorb all refracted light, we nullify the diffuse lighting based on the metallic parameter
		vec3 specularRatio = fresnel;
		vec3 diffuseRatio = vec3(1.0) - specularRatio;
		diffuseRatio *= 1.0 - metallic;

		// Finally calculate the specular part of the Cook-Torrance BRDF (max 0.1 stops any visual artifacts)
		vec3 numerator = specularRatio * normalDistribution * geometry;
		float denominator = 4 * max(dot(fragToView, normal), 0.1) * max(dot(lightDir, normal), 0.0) + 0.001;  // Prevents any division by zero
		vec3 specular = numerator / denominator;

		// Also calculate the diffuse, a lambertian calculation will be added onto the final radiance calculation
		vec3 diffuse = diffuseRatio * albedo / PI;

		// Add the light's radiance to the irradiance sum
		directLightIrradiance += (diffuse + specular) * radiance * max(dot(normal, lightDir), 0.0) * (1.0 - CalculateShadow(normal, lightDir));
	}

	return directLightIrradiance;
}

vec3 CalculatePointLightRadiance(vec3 albedo, vec3 normal, float metallic, float roughness, vec3 fragToView, vec3 baseReflectivity){
	vec3 pointLightIrradiance = vec3(0.0);
	for(int i = 0; i < numDirPointSpotLights.y; ++i){
		vec3 fragToLight = normalize(pointLights[i].position - FragPos);
		vec3 halfway = normalize(fragToView + fragToLight);
		float fragToLightDistance = length(pointLights[i].position - FragPos);
		float attenuation = 1.0 / (fragToLightDistance * fragToLightDistance);
		vec3 radiance = pointLights[i].lightColor * attenuation;

		//Cook-Torrance Specular BRDF calculations
		float normalDistribution = NormalDistributionGGX(normal, halfway, roughness);
		vec3 fresnel = FresnelSchlick(max(dot(halfway, fragToView), 0.0), baseReflectivity);
		float geometry = GeometrySmith(normal, fragToView, fragToLight, roughness);

		// Calculate reflected and refracted light respectively, and since metals absorb all refracted light, we nullify the diffuse lighting based on the metallic parameter
		vec3 specularRatio = fresnel;
		vec3 diffuseRatio = vec3(1.0) - specularRatio;
		diffuseRatio *= 1.0 - metallic;

		// Finally calculate the specular part of the Cook-Torrance BRDF
		vec3 numerator = specularRatio * normalDistribution * geometry;
		float denominator = 4 * max(dot(fragToView, normal), 0.1) * max(dot(fragToLight, normal), 0.0) + 0.001; // Prevents any division by zero
		vec3 specular = numerator / denominator;

		// Also calculate the diffuse, a lambertian calculation will be added onto the final radiance calculation
		vec3 diffuse = diffuseRatio * albedo / PI;

		// Add light radiance to the irradiance sum
		pointLightIrradiance += (diffuse + specular) * radiance * max(dot(normal, fragToLight), 0.0);
	}
	return pointLightIrradiance;	
}
vec3 CalculateSpotLightRadiance(vec3 albedo, vec3 normal, float metallic, float roughness, vec3 fragToView, vec3 baseReflectivity){
	vec3 spotLightIrradiance = vec3(0.0);

	for(int i = 0; i < numDirPointSpotLights.z; ++i){
		vec3 fragToLight = normalize(spotLights[i].position - FragPos);
		vec3 halfway = normalize(fragToView + fragToLight);
		float fragToLightDistance = length(spotLights[i].position - FragPos);

		// Check if it is in the spotlight's circle
		float theta = dot(normalize(spotLights[i].direction), -fragToLight);
		float difference = spotLights[i].cutOff - spotLights[i].outerCutOff;
		float intensity = clamp((theta - spotLights[i].outerCutOff) / difference, 0.0, 1.0);
		float attenuation = intensity * (1.0 / (fragToLightDistance * fragToLightDistance));
		vec3 radiance = spotLights[i].lightColor * attenuation;

		// Cook-Torrance Specular BRDF calculations
		float normalDistribution = NormalDistributionGGX(normal, halfway, roughness);
		vec3 fresnel = FresnelSchlick(max(dot(halfway, fragToView), 0.0), baseReflectivity);
		float geometry = GeometrySmith(normal, fragToView, fragToLight, roughness);

		// Calculate reflected and refracted light respectively, and since metals absorb all refracted light, we nullify the diffuse lighting based on the metallic parameter
		vec3 specularRatio = fresnel;
		vec3 diffuseRatio = vec3(1.0) - specularRatio;
		diffuseRatio *= 1.0 - metallic;

		// Finally calculate the specular part of the Cook-Torrance BRDF
		vec3 numerator = specularRatio * normalDistribution * geometry;
		float denominator = 4 * max(dot(fragToView, normal), 0.1) * max(dot(fragToLight, normal), 0.0) + 0.001; // Prevents any division by zero
		vec3 specular = numerator / denominator;

		// Also calculate the diffuse, a lambertian calculation will be added onto the final radiance calculation
		vec3 diffuse = diffuseRatio * albedo / PI;

		//add the light's radiance to the irradiance calculation
		 spotLightIrradiance += (diffuse + specular) * radiance * max(dot(normal, fragToLight), 0.0);
	}
	return spotLightIrradiance;
}


// Approximates the amount of microfacets that are properly aligned with the halfway vector, thus determines the strength and area for specular light
float NormalDistributionGGX(vec3 normal, vec3 halfway, float roughness) {
	float a = roughness * roughness;
	float a2 = a * a;
	float normDotHalf = dot(normal, halfway);
	float normDotHalf2 = normDotHalf * normDotHalf;

	float numerator = a2;
	float denominator = normDotHalf2 * (a2 - 1) + 1;
	denominator = PI * denominator * denominator;

	return numerator / denominator;
}

// Approximates the geometry obstruction and geometry shadowing respectively, on the microfacet level
float GeometrySmith(vec3 normal, vec3 viewDir, vec3 lightDir, float roughness) {
	return GeometrySchlickGGX(max(dot(normal, viewDir), 0.0), roughness) * GeometrySchlickGGX(max(dot(normal, lightDir), 0.0), roughness);
}
float GeometrySchlickGGX(float cosTheta, float roughness) {
	float r = (roughness + 1.0);
	float k = (roughness * roughness) / 8.0;

	float numerator = cosTheta;
	float denominator = cosTheta * (1.0 - k) + k;

	return numerator / max(denominator, 0.001);
}

// Calculates the amount of specular light. Since diffuse(refraction) 
// and specular(reflection) are mutually exclusive, 
// we can also use this to determine the amount of diffuse light
// Taken from UE4's implementation which is faster and basically identical to the usual Fresnel calculations: https://blog.selfshadow.com/publications/s2013-shading-course/karis/s2013_pbs_epic_notes_v2.pdf
vec3 FresnelSchlick(float cosTheta, vec3 baseReflectivity) {
	return max(baseReflectivity + (1.0 - baseReflectivity) * pow(2, (-5.55473 * cosTheta - 6.98316) * cosTheta), 0.0);
}

float CalculateShadow(vec3 normal, vec3 fragToLight) {
	vec3 ndcCoords = FragPosLightClipSpace.xyz / FragPosLightClipSpace.w;
	vec3 depthmapCoords = ndcCoords * 0.5 + 0.5;

	float shadow = 0.0;
	float currentDepth = depthmapCoords.z;

	// Add shadow bias to avoid shadow acne, and more shadow bias is needed depending on the angle between the normal and light direction
	// However too much bias can cause peter panning
	float shadowBias = max(0.01, 0.1 * (1.0 - dot(normal, fragToLight)));

	// Perform Percentage Closer Filtering (PCF) in order to produce soft shadows
	vec2 texelSize = 1.0 / textureSize(shadowmap, 0);
	for (int y = -1; y <= 1; ++y) {
		for (int x = -1; x <= 1; ++x) {
			float sampledDepthPCF = texture(shadowmap, depthmapCoords.xy + (texelSize * vec2(x, y))).r;
			shadow += currentDepth > sampledDepthPCF + shadowBias ? 1.0 : 0.0;
		}
	}
	shadow /= 9.0;

	if (currentDepth > 1.0)
		shadow = 0.0;
	return shadow;
}

