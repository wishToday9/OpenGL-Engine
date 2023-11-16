#pragma once

#include "Pane.h"


namespace OpenGL_Engine {
	 
		class DebugPane : public Pane {
		public:
			DebugPane(glm::vec2& panePosition);
			virtual void setupPaneObjects() override;
			static inline void bindCameraPositionValue(glm::vec3* ptr) { s_CameraPosition = ptr; }
			static inline void bindGammaCorrectionValue(float* ptr) { s_GammaCorrectionValue = ptr; }
			static inline bool getWireframeMode() { return s_WireframeMode; }
			static inline void setWireframeMode(bool choice) { s_WireframeMode = choice; }
		private:
			static float* s_GammaCorrectionValue;
			static bool s_WireframeMode;
			static glm::vec3* s_CameraPosition;
 		};
}