
#ifndef HEIGHTMAPGENERATOR_H_
#define HEIGHTMAPGENERATOR_H_

#include <cstdlib>
#include <cmath>
#include <ctime>
#include <vector>
#include <boost/tr1/memory.hpp>
#include <cassert>

typedef std::tr1::shared_ptr<std::vector<int> > IntVectorPtr;
typedef std::vector<IntVectorPtr > Matrix;

struct HMVertex {
	float x, y, z;
};

/**
 * This class generates a height values representing terrain. The terrain
 * is generated using the Diamond-Square algorithm as explained here:
 *
 * http://www.gameprogrammer.com/fractal.html
 */
class HeightmapGenerator {
	public:
		HeightmapGenerator(int size, int seed, int variation);
		virtual ~HeightmapGenerator();

		void fillMap();
		const Matrix& getMap();
		void convertMap();
		HMVertex* getVertices();

		unsigned int getVertexCount();
	private:
		Matrix map;
		int mapWidth;
		int variation;
		unsigned int vertexCount;
		HMVertex* vertices;

		void fillMap(int iteration);
		void convertMap(int x, int z, int numVertex);
		void makeDiamond(int, int, int, int, int);
		void makeSquare(int, int, int, int, int);
		int randomNumber(int iteration);
		int numberOfTrailingZeros(int);
};

inline unsigned int HeightmapGenerator::getVertexCount() {
	assert(vertexCount);

	return vertexCount;
}

#endif
