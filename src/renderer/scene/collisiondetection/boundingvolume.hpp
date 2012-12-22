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
			protected:
				BoundingVolume() = default;
				virtual ~BoundingVolume() = default;
		};
	}
}

#endif /* BOUNDINGVOLUME_HPP_ */
