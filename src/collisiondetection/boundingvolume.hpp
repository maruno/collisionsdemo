#pragma once

namespace collisiondetection {
	class BoundingVolume {
	 public:
		virtual bool intersects(const BoundingVolume& other) const = 0;

		virtual ~BoundingVolume() = default;
	};
}