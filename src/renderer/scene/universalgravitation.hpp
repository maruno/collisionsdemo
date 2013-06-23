#ifndef UNIVERSALGRAVITATION_HPP
#define UNIVERSALGRAVITATION_HPP

#include <vector>
#include <algorithm>
#include <mutex>

#include "renderer/scene/gravitationalobject.hpp"

namespace scene {
	/**
	 * This class applies the law of universal gravitation from Sir Isaac Newton.
	 *
	 * @author Michel Bouwmans
	 */
	class UniversalGravitation {
	 private:
		static constexpr double G = 0.0000000000667384;

		std::vector<GravitationalObject*> gravityObjects;
		std::mutex gravObjectsMutex;
	 public:
		inline void addObject(GravitationalObject* gravObject);
		inline void delObject(GravitationalObject* gravObject);

		void update();
	};

	void UniversalGravitation::addObject(GravitationalObject* gravObject) {
		std::lock_guard<std::mutex> lock(gravObjectsMutex);
		gravityObjects.push_back(gravObject);
	}

	void UniversalGravitation::delObject(GravitationalObject* gravObject) {
		std::lock_guard<std::mutex> lock(gravObjectsMutex);

		auto it = std::find(gravityObjects.begin(), gravityObjects.end(), gravObject);
		gravityObjects.erase(it);
	}
}
#endif // UNIVERSALGRAVITATION_HPP
