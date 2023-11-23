#version 450 core

out vec4 FragColour;

in vec2 TexCoords;


uniform float gamma_inverse;
uniform sampler2D color_texture;
uniform vec2 read_offset;
uniform bool blur_enabled;

void main() {
	vec3 hdrColour = texture(color_texture, TexCoords).rgb;
	
	// Apply a simple Reinhard tonemapper (HDR -> SDR)
	vec3 tonemappedColour = hdrColour / (hdrColour + vec3(1.0));

	// Apply gamma correction (Linear -> Non-Linear)
	FragColour = vec4(pow(tonemappedColour, vec3(gamma_inverse)), 1.0);
}