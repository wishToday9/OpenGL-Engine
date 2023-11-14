#pragma once

#include "Pane.h"


namespace OpenGL_Engine {
	namespace ui {
		class DebugPane : public Pane {
		public:
			DebugPane(glm::vec2& panePosition);
			virtual void setupPaneObjects() override;
			inline static void bindCameraPositionValue(glm::vec3* ptr) {
				s_CameraPosition = ptr;
			}
			
			inline static void bindGammaCorrectionValue(float* ptr) {
				s_GammaCorrectionValue = ptr;
			}

			static bool getWireframeMode() {
				return s_WireframeMode;
			}
			static bool setWireframeMode(bool choice) {
				s_WireframeMode = choice;
			}
		private:
			static float* s_GammaCorrectionValue;
			static bool s_WireframeMode;
			static glm::vec3* s_CameraPosition;
 		};
	}

}