/*
 * BoundingVolume.hpp
 *
 *      Author: Marcel Veltman
 */

#ifndef BOUNDINGVOLUME_HPP_
#define BOUNDINGVOLUME_HPP_

namespace scene{
	namespace collisiondetection {
		class BoundingVolume {
		 public:
			virtual bool intersects(const BoundingVolume& other) const = 0;

			virtual ~BoundingVolume() = default;
		};
	}
}

#endif /* BOUNDINGVOLUME_HPP_ */
