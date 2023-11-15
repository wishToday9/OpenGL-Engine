#pragma once

#include <graphics/Shader.h>
#include <graphics/mesh/Mesh.h>
#include <graphics/mesh/Model.h>
#include <utils/loaders/TextureLoader.h>

#include <array>

#include <graphics/renderer/renderpass/RenderPassType.h>

namespace OpenGL_Engine { 
	class Terrain {
	public:
		Terrain(glm::vec3 &worldPosition);
		~Terrain();

		void Draw(Shader &shader, RenderPassType pass) const;

		inline const glm::vec3& getPosition() const { return m_Position; }
	private:
		glm::vec3 calculateNormal(unsigned x, unsigned z, unsigned char *heightMapData);
		float getVertexHeight(unsigned x, unsigned y, unsigned char *heightMapData);

		float m_TerrainSize;
		unsigned int m_VertexSideCount;
		GLushort m_HeightMapScale;

		glm::mat4 m_ModelMatrix;
		glm::vec3 m_Position;
		Mesh *m_Mesh;
		std::array<Texture*, 9> m_Textures; // Represents all the textures supported by the terrain's texure splatting (rgba and the default value)
	};
} 

