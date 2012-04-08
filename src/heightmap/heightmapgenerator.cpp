#include "heightmap/heightmapgenerator.hpp"

#include <ctime>

HeightmapGenerator::HeightmapGenerator(int size, int seed, int variation) {
	mapWidth = size;
	HeightmapGenerator::variation = variation;
	vertices = new HMVertex[size*size];

	for(int i = 0; i < size; ++i) {
		IntVectorPtr intVectorPtr(new std::vector<int>(size));
		map.push_back(intVectorPtr);
	}

	(*map[0])[0] = seed;
	(*map[0])[size-1] = seed;
	(*map[size-1])[0] = seed;
	(*map[size-1])[size-1] = seed;
}

HeightmapGenerator::~HeightmapGenerator() {
}

const Matrix& HeightmapGenerator::getMap() {
	return map;
}

HMVertex* HeightmapGenerator::getVertices() {
	return vertices;
}

void HeightmapGenerator::fillMap() {
	srand(std::time(0));
	int twoLog = numberOfTrailingZeros(map.size()-1);
	for(int i = 0; i < (twoLog - 1); i++) {
		fillMap(0);
	}
}

void HeightmapGenerator::convertMap() {
	int numVertex = 0;
	int x = 0;
	int z = 0;
	int xPrev = -1;
	int zPrev = -1;
	for(int mod = 0; mod < mapWidth-1; mod++) {
		for(x = 0; x < mapWidth; x++) {
			if(x != xPrev || z != zPrev) {
				z = mod;
				HMVertex nextVertex;
				nextVertex.x = x;
				nextVertex.z = z;
				nextVertex.y = (*map[x])[z];
				vertices[numVertex] = nextVertex;
				numVertex++;
			}
			z = mod+1;
			HMVertex nextVertex;
			nextVertex.x = x;
			nextVertex.z = z;
			nextVertex.y = (*map[x])[z];
			vertices[numVertex] = nextVertex;
			numVertex++;
			xPrev = x;
			zPrev = z;
		}
		mod++;
		for(x = mapWidth-1; x >= 0; x--) {
			if(x != xPrev || z != zPrev) {
				z = mod;
				HMVertex nextVertex;
				nextVertex.x = x;
				nextVertex.z = z;
				nextVertex.y = (*map[x])[z];
				vertices[numVertex] = nextVertex;
				numVertex++;
			}

			z = mod+1;
			HMVertex nextVertex;
			nextVertex.x = x;
			nextVertex.z = z;
			nextVertex.y = (*map[x])[z];
			vertices[numVertex] = nextVertex;
			numVertex++;
			xPrev = x;
			zPrev = z;
		}
	}
}

void HeightmapGenerator::fillMap(int iteration) {
	int length = (map.size()-1) >> iteration;
	int mapSize = map.size();
	for(int xLow = 0; xLow < mapSize - 1; xLow += length) {
		int xHigh = xLow + length;
		for(int yLow = 0; yLow < mapSize - 1; yLow += length) {
			int yHigh = yLow + length;
			makeDiamond(iteration, xLow, xHigh, yLow, yHigh);
		}
	}
	for(int xLow = ((length>>1)^-1)+1; xLow < mapSize - length; xLow += length) {
		int xHigh = xLow + length;
		for(int yLow = 0; yLow < mapSize - 1; yLow += length) {
			int yHigh = yLow + length;
			makeSquare(iteration, xLow, xHigh, yLow, yHigh);
		}
	}
	for(int yLow = ((length>>1)^-1)+1; yLow < mapSize - length; yLow += length) {
		int yHigh = yLow + length;
		for(int xLow = 0; xLow < mapSize - 1; xLow += length) {
			int xHigh = xLow + length;
			makeSquare(iteration, xLow, xHigh, yLow, yHigh);
		}
	}
}

void HeightmapGenerator::makeDiamond(int iteration, int xLow, int xHigh, int yLow, int yHigh) {
	int x = (xLow + xHigh) >> 1;
	int y = (yLow + yHigh) >> 1;
	int val1 = (*map[xLow])[yLow];
	int val2 = (*map[xLow])[yHigh];
	int val3 = (*map[xHigh])[yLow];
	int val4 = (*map[xHigh])[yHigh];
	int random = randomNumber(iteration);
	int newValue = ((val1 + val2 + val3 + val4) >> 2) + random;
	(*map[x])[y] = newValue;
}

void HeightmapGenerator::makeSquare(int iteration, int xLow, int xHigh, int yLow, int yHigh) {
	int x = (xLow + xHigh) >> 1;
	int y = (yLow + yHigh) >> 1;
	int tempXLow = xLow < 0 ? map.size() + xLow - 1 : xLow;
	int tempYLow = yLow < 0 ? map.size() + yLow - 1 : yLow;
	int val1 = (*map[tempXLow])[y];
	int val2 = (*map[x])[tempYLow];
	int val3 = (*map[xHigh])[y];
	int val4 = (*map[x])[yHigh];
	int random = randomNumber(iteration);
	int newValue = ((val1 + val2 + val3 + val4) >> 2) + random;
	(*map[x])[y] = newValue;
	if(x == 0) {
		(*map[map.size()-1])[y] = newValue;
	}
	if(y == 0) {
		(*map[x])[map.size()-1] = newValue;
	}
}

int HeightmapGenerator::randomNumber(int iteration) {
	int random = (((rand()%variation)<<1)-variation)>>iteration;
	return random;
}

int HeightmapGenerator::numberOfTrailingZeros(int integer) {
	if(integer == 0)
		return 32;
	int trailingZeros = 32;
	while(integer != 0) {
		integer = integer << 1;
		trailingZeros--;
	}
	return trailingZeros;
}
