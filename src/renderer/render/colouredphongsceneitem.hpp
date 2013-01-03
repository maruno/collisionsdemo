#ifndef REDER_COLOUREDPHONGSCENEITEM_HPP
#define REDER_COLOUREDPHONGSCENEITEM_HPP

#include "renderer/scene/sceneitem.hpp"

#include <string>

#include "glload/gl_3_2.h"

#include "renderer/render/colourinformationuniform.hpp"

namespace render {
	class ColouredPhongSceneItem : public scene::SceneItem {
	 private:
		GLuint colourInfoUBO;

	 public:
		ColouredPhongSceneItem(glm::vec3 initialLocation, std::string objectName, ColourInformationUniform colour);

		virtual void render(glm::mat4& parentMatrix) const final override;

		virtual ~ColouredPhongSceneItem() = default;
	};
}

#endif // REDER_COLOUREDPHONGSCENEITEM_HPP
