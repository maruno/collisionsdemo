#ifndef UNIVERSALGRAVITATION_HPP
#define UNIVERSALGRAVITATION_HPP

#include <vector>
#include <algorithm>

#include "scene/gravitationalobject.hpp"

namespace scene {
	class UniversalGravitation {
		private:
			static constexpr double G = 0.0000000000667384;
			
			std::vector<GravitationalObject*> gravityObjects;
		public:
			inline void addObject(GravitationalObject* gravObject);
			inline void delObject(GravitationalObject* gravObject);
			
			void update();
	};
	
	void UniversalGravitation::addObject(GravitationalObject* gravObject) {
		gravityObjects.push_back(gravObject);
	}

	void UniversalGravitation::delObject(GravitationalObject* gravObject) {
		std::remove(gravityObjects.begin(), gravityObjects.end(), gravObject);
	}
}
#endif // UNIVERSALGRAVITATION_HPP
