
#ifndef HEIGHTMAPGENERATOR_H_
#define HEIGHTMAPGENERATOR_H_

#include <cstdlib>
#include <cmath>
#include <ctime>
#include <vector>
#include <boost/tr1/memory.hpp>

typedef std::tr1::shared_ptr<std::vector<int> > IntVectorPtr;
typedef std::vector<IntVectorPtr > Matrix;

struct HMVertex {
	int x, y, z;
};

class HeightmapGenerator {
	public:
		HeightmapGenerator(int size, int seed, int variation);
		virtual ~HeightmapGenerator();

		void fillMap();
		const Matrix& getMap();
		void convertMap();
		HMVertex* getVertices();
	private:
		Matrix map;
		int mapWidth;
		int variation;
		HMVertex* vertices;

		void fillMap(int iteration);
		void makeDiamond(int, int, int, int, int);
		void makeSquare(int, int, int, int, int);
		int randomNumber(int iteration);
		int numberOfTrailingZeros(int);
};

#endif
