#version 450 core

// Does AMD support sampler2D in a struct?
struct Material {
	sampler2D texture_diffuse1; // background texture
	sampler2D texture_diffuse2; // r texture
	sampler2D texture_diffuse3; // g texture
	sampler2D texture_diffuse4; // b texture
	sampler2D texture_diffuse5; // blend map
	sampler2D texture_normal1; // background texture
	sampler2D texture_normal2; // r texture
	sampler2D texture_normal3; // g texture
	sampler2D texture_normal4; // b texture
	sampler2D blendmap;
	float shininess;
	float tilingAmount;
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

in mat3 TBN;
in vec2 TexCoords;
in vec3 FragPos;
in vec4 FragPosLightClipSpace;

out vec4 color;

uniform sampler2D shadowmap;
uniform ivec4 numDirPointSpotLights;

uniform DirLight dirLights[MAX_DIR_LIGHTS];
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform Material material;
uniform vec3 viewPos;

// Functions
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 fragToCam);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 fragToCam);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos);
float CalculateShadow(vec3 normal, vec3 fragToDirLight);
vec3 UnpackNormal(vec3 textureNormal);

void main() {
		vec4 blendMapColour = texture(material.blendmap, TexCoords);

	float backTextureWeight = 1 - (blendMapColour.r + blendMapColour.g + blendMapColour.b);

	vec2 tiledCoords = TexCoords * material.tilingAmount;

	vec3 backgroundTextureColour = texture(material.texture_diffuse1, tiledCoords).rgb * backTextureWeight;
	vec3 rTextureColour = texture(material.texture_diffuse2, tiledCoords).rgb * blendMapColour.r;
	vec3 gTextureColour = texture(material.texture_diffuse3, tiledCoords).rgb * blendMapColour.g;
	vec3 bTextureColour = texture(material.texture_diffuse4, tiledCoords).rgb * blendMapColour.b;
	vec3 blendedTextureColour = backgroundTextureColour + rTextureColour + gTextureColour + bTextureColour;

	vec3 backgroundTextureNormal = UnpackNormal(texture(material.texture_normal1, tiledCoords).rgb) * backTextureWeight;
	vec3 rTextureNormal = UnpackNormal(texture(material.texture_normal2, tiledCoords).rgb) * blendMapColour.r;
	vec3 gTextureNormal = UnpackNormal(texture(material.texture_normal3, tiledCoords).rgb) * blendMapColour.g;
	vec3 bTextureNormal = UnpackNormal(texture(material.texture_normal4, tiledCoords).rgb) * blendMapColour.b;
	vec3 blendedTextureNormal = normalize(backgroundTextureNormal + rTextureNormal + gTextureNormal + bTextureNormal);

	// Perform lighting on the terrain
	vec3 normal = normalize(TBN * blendedTextureNormal);
	vec3 fragToCam = normalize(viewPos - FragPos);
	vec3 terrainColour = vec3(0.0);

	for (int i = 0; i < numDirPointSpotLights.x; ++i) {
		terrainColour += CalcDirLight(dirLights[i], normal, fragToCam);
	}
	for (int i = 0; i < numDirPointSpotLights.y; ++i) {
		terrainColour += CalcPointLight(pointLights[i], normal, FragPos, fragToCam);
	}
	for (int i = 0; i < numDirPointSpotLights.z; ++i) {
		terrainColour += CalcSpotLight(spotLights[i], normal, FragPos);
	}

	terrainColour = terrainColour * blendedTextureColour;

	// Result
	color = vec4(terrainColour, 1.0);
}

// Unpacks the normal from the texture and returns the normal in tangent space
vec3 UnpackNormal(vec3 textureNormal) {
	return normalize(textureNormal * 2.0 - 1.0);
}

//add mutiple shadows
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 fragToCam) {
	vec3 fragToLight = normalize(-light.direction);

	float diff = max(dot(fragToLight, normal), 0.0);

	vec3 ambient = light.lightColor * vec3(0.05);
	vec3 diffuse = light.lightColor * diff;
	
	return ambient + (diffuse * (1.0 - CalculateShadow(normal, fragToLight)));
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 fragToCam) {
	vec3 fragToLight = normalize(light.position - fragPos);

	float diff = max(dot(fragToLight, normal), 0.0);

	// Attenuation calculation
	float fragToLightDistance = length(light.position - fragPos);
	float attenuation = 1.0 / (fragToLightDistance * fragToLightDistance);

	vec3 ambient = light.lightColor * vec3(0.05) * attenuation;
	vec3 diffuse = light.lightColor * diff * attenuation;

	return ambient + diffuse;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos) {
	vec3 fragToLight = normalize(light.position - fragPos);

	// Attenuation calculation
	float fragToLightDistance = length(light.position - fragPos);
	float attenuation = 1.0 / (fragToLightDistance * fragToLightDistance);

	// check if its in the spotlight's circle
	float theta = dot(-fragToLight, normalize(light.direction));
	float difference = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / difference, 0.0, 1.0);

	vec3 ambient = light.lightColor * vec3(0.05) * attenuation;
	vec3 diffuse = light.lightColor * intensity * attenuation;

	return ambient + diffuse;
}

float CalculateShadow(vec3 normal, vec3 fragToDirLight) {
	vec3 ndcCoords = FragPosLightClipSpace.xyz / FragPosLightClipSpace.w;
	vec3 depthmapCoords = ndcCoords * 0.5 + 0.5;

	float shadow = 0.0;
	float currentDepth = depthmapCoords.z;

	// Add shadow bias to avoid shadow acne, and more shadow bias is needed depending on the angle between the normal and light direction
	// However too much bias can cause peter panning
	float shadowBias = max(0.01, 0.1 * (1.0 - dot(normal, fragToDirLight)));

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
