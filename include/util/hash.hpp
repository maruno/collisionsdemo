#pragma once

#include <functional>

#include <glm/glm.hpp>

namespace std {
	template<> struct hash<glm::vec3> {
		size_t operator()(const glm::vec3& v) const {
			std::hash<float> floathasher;
			
			return floathasher(v.x) ^ floathasher(v.y) ^ floathasher(v.z);
		}
	};
}