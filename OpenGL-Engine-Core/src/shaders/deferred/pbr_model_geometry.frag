#version 450 core
layout(location = 0) out vec4 gb_Albedo;
layout(location = 1) out vec3 gb_Normal;
layout(location = 2) out vec4 gb_MaterialInfo;

// Does AMD support sampler2D in a struct?
struct Material {
	sampler2D texture_albedo;
	sampler2D texture_normal;
	sampler2D texture_metallic;
	sampler2D texture_roughness;
	sampler2D texture_ao;
};

in mat3 TBN;
in vec2 TexCoords;

uniform Material material;


//functions
vec3 UnpackNormal(vec3 textureNormal);


void main() {
	// Sample textures
	vec4 albedo = texture(material.texture_albedo, TexCoords);
	vec3 normal = texture(material.texture_normal, TexCoords).rgb;
	float metallic = texture(material.texture_metallic, TexCoords).r;
	float roughness = max(texture(material.texture_roughness, TexCoords).r, 0.04);
	float ao = texture(material.texture_ao, TexCoords).r;

	//normal mapping
	normal = normalize(TBN * UnpackNormal(normal));

	gb_Albedo = albedo;
	gb_Normal = normal;
	gb_MaterialInfo = vec4(metallic, roughness, ao, 1.0);
}



// Unpacks the normal from the texture and returns the normal in tangent space
vec3 UnpackNormal(vec3 textureNormal) {
	return normalize(textureNormal * 2.0 - 1.0);
}
