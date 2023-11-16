#pragma once

#include <graphics/Shader.h>
#include <unordered_map>

namespace OpenGL_Engine {
	class ShaderLoader {
	public:
		static Shader* loadShader(const char* vertPath, const char* fragPath);
		static Shader* loadShader(const char* vertPath, const char* fragPath, const char* geoPath);

	private:
		static std::unordered_map<std::size_t, Shader*> s_ShaderCache;
		static std::hash<std::string> s_Hasher;
	};
}