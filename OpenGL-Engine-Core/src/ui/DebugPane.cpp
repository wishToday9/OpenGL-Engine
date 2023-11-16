#include "pch.h"
#include "DebugPane.h"


namespace OpenGL_Engine {
	 

		DebugPane::DebugPane(glm::vec2& panePosition)
			:Pane(std::string("Debug Controls"), panePosition)
		{

		}

		void DebugPane::setupPaneObjects()
		{

			if (s_CameraPosition != nullptr)
				ImGui::Text("Camera Pos x:%.1f y:%.1f z:%.1f", s_CameraPosition->x, s_CameraPosition->y, s_CameraPosition->z);
			if (s_GammaCorrectionValue != nullptr)
				ImGui::SliderFloat("Gamma", s_GammaCorrectionValue, 0.5f, 3.0f, "%.2f");
#if DEBUG_ENABLED
			ImGui::Text("Hit \"P\" to show/hide the cursor");
			ImGui::Checkbox("Wireframe mode", &s_WireframeMode);
#endif
		}

		bool DebugPane::s_WireframeMode = false;
		float* DebugPane::s_GammaCorrectionValue = nullptr;
		glm::vec3* DebugPane::s_CameraPosition = nullptr;
}