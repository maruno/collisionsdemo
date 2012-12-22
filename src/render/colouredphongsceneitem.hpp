#ifndef REDER_COLOUREDPHONGSCENEITEM_HPP
#define REDER_COLOUREDPHONGSCENEITEM_HPP

#include "scene/sceneitem.hpp"

#include <string>

#include "glload/gl_3_2.h"

#include "render/colourinformationuniform.hpp"

namespace render {
	class ColouredPhongSceneItem : public scene::SceneItem {
		private:
			GLuint colourInfoUBO;

		public:
			ColouredPhongSceneItem(glm::vec3 initialLocation, std::string objectName,
				       scene::collisiondetection::BoundingVolume& myBounds,
		                       ColourInformationUniform colour);

			virtual void render(glm::mat4& parentMatrix) const final override;
	};
}

#endif // REDER_COLOUREDPHONGSCENEITEM_HPP
