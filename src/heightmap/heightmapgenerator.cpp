#include "heightmap/heightmapgenerator.hpp"

#include <ctime>

/**
 * Prepares the objects needed for generating terrain.
 *
 * Size must be a power of 2 plus 1 (i.e. 1+2^n).
 * It is recommended to use values no greater than 1025.
 *
 * Seed can be any value, but its recommended to use
 * values around 0.
 *
 * Variation can be any positive value, but to prevent
 * integer overflow the value must be smaller than
 * (2^31-seed)/2.5.
 */
HeightmapGenerator::HeightmapGenerator(int size, int seed, int variation) {
	HeightmapGenerator::variation = variation;

	//Set the size of the map
	for(int i = 0; i < size; ++i) {
		IntVectorPtr intVectorPtr(new std::vector<int>(size));
		map.push_back(intVectorPtr);
	}

	//Seed the map
	(*map[0])[0] = seed;
	(*map[0])[size-1] = seed;
	(*map[size-1])[0] = seed;
	(*map[size-1])[size-1] = seed;

	mapWidth = size;

	//Set the size of the vertex array. The array will contain duplicates so
	//triangle strips can be used easily.
	int pow = numberOfTrailingZeros(mapWidth-1);
	vertexCount = ((1 << (pow+1))-1)*mapWidth+2;
	vertices = new HMVertex[vertexCount];
}

HeightmapGenerator::~HeightmapGenerator() {
}

const Matrix& HeightmapGenerator::getMap() {
	return map;
}

HMVertex* HeightmapGenerator::getVertices() {
	return vertices;
}

void HeightmapGenerator::fillMap(){
	srand(std::time(0));//Initialize the random number generator with the current time
	int twoLog = numberOfTrailingZeros(map.size()-1);//Number of iterations needed to generate the map
	for(int i = 0; i < twoLog; i++) {
		fillMap(i);
	}
}

/**
 * Converts the 2D array of height values to a 1D array with vertices.
 * These verices are ordered in such a way that the entire map can be
 * drawn with triangle strips.
 */
void HeightmapGenerator::convertMap() {
	int numVertex = 0;
	int x = 0;
	int z = 0;
	int xPrev = -1;
	int zPrev = -1;

	for(int mod = 0; mod < mapWidth-1; mod++) {
		for(x = 0; x < mapWidth; x++) {
		   	if(x != xPrev || z != zPrev) {//Turnaround at the end of the row must be added only once
	    		z = mod;
	    		convertMap(x, z, numVertex);
	    		numVertex++;
	    	}
	        z = mod+1;
	        convertMap(x, z, numVertex);
    		numVertex++;
	        xPrev = x;
	        zPrev = z;
	    }
	    mod++;
	    for(x = mapWidth-1; x >= 0; x--) {
	    	if(x != xPrev || z != zPrev) {//Turnaround at the end of the row must be added only once
	    		z = mod;
	    		convertMap(x, z, numVertex);
	    		numVertex++;
	    	}
	        z = mod+1;
	        convertMap(x, z, numVertex);
    		numVertex++;
	        xPrev = x;
	        zPrev = z;
	    }
	}
}

void HeightmapGenerator::convertMap(int x, int z, int numVertex) {
	HMVertex nextVertex;
	nextVertex.x = x-0.5*mapWidth;//Substract half the width of the map to center the map on the x-axis
	nextVertex.z = z-0.5*mapWidth;//Substract half the width of the map to center the map on the z-axis
	nextVertex.y = (*map[x])[z];
	vertices[numVertex] = nextVertex;
}

/**
 * Generates the terrain.
 */
void HeightmapGenerator::fillMap(int iteration) {
	int length = (map.size()-1) >> iteration;
	int mapSize = map.size();

	//Determine the values needed for the diamond step
	//and execute the diamond step
	for (int xLow = 0; xLow < mapSize - 1; xLow += length) {
		int xHigh = xLow + length;
		for (int zLow = 0; zLow < mapSize - 1; zLow += length) {
			int zHigh = zLow + length;
			makeDiamond(iteration, xLow, xHigh, zLow, zHigh);
		}
	}

	//Determine the values needed for the square step
	//and execute the square step for the odd rows.
	for (int xLow = ((length>>1)^-1)+1; xLow < mapSize - length; xLow += length) {
		int xHigh = xLow + length;
		for (int zLow = 0; zLow < mapSize - 1; zLow += length) {
			int zHigh = zLow + length;
			makeSquare(iteration, xLow, xHigh, zLow, zHigh);
		}
	}

	//Determine the values needed for the square step
	//and execute the square step for the even rows.
	for (int zLow = ((length>>1)^-1)+1; zLow < mapSize - length; zLow += length) {
		int zHigh = zLow + length;
		for (int xLow = 0; xLow < mapSize - 1; xLow += length) {
			int xHigh = xLow + length;
			makeSquare(iteration, xLow, xHigh, zLow, zHigh);
		}
	}
}

void HeightmapGenerator::makeDiamond(int iteration, int xLow, int xHigh, int zLow, int zHigh) {
	int x = (xLow + xHigh) >> 1;//Average of xLow and xHigh
	int z = (zLow + zHigh) >> 1;//Average of zLow and zHigh

	int val1 = (*map[xLow])[zLow];
	int val2 = (*map[xLow])[zHigh];
	int val3 = (*map[xHigh])[zLow];
	int val4 = (*map[xHigh])[zHigh];

	int random = randomNumber(iteration);
	int newValue = ((val1 + val2 + val3 + val4) >> 2) + random;//Average of the values plus a random variation.

	(*map[x])[z] = newValue;
}

void HeightmapGenerator::makeSquare(int iteration, int xLow, int xHigh, int zLow, int zHigh) {
	int x = (xLow + xHigh) >> 1;//Average of xLow and xHigh
	int z = (zLow + zHigh) >> 1;//Average of yLow and yHigh
	int tempXLow = xLow < 0 ? map.size() + xLow - 1 : xLow;//Handle out of bounds
	int tempZLow = zLow < 0 ? map.size() + zLow - 1 : zLow;//Handle out of bounds

	int val1 = (*map[tempXLow])[z];
	int val2 = (*map[x])[tempZLow];
	int val3 = (*map[xHigh])[z];
	int val4 = (*map[x])[zHigh];

	int random = randomNumber(iteration);
	int newValue = ((val1 + val2 + val3 + val4) >> 2) + random;//Average of the values plus a random variation.

	(*map[x])[z] = newValue;
	if (x == 0) {
		(*map[map.size()-1])[z] = newValue;//Make the map wrappable
	}
	if (z == 0) {
		(*map[x])[map.size()-1] = newValue;//Make the map wrappable
	}
}

/**
 * Generates a random number between HeightmapGenerator.variation
 * and its negative halved by the amount of iterations the algorithm
 * has done so far.
 */
int HeightmapGenerator::randomNumber(int iteration){
	int random = (((rand()%variation)<<1)-variation)//Set the range to [-variation, variation)
									>>iteration;   //Halve the number base on the iteration
	return random;
}

/**
 * Returns the number of trailing zeros on the bit level.
 * For example: 4 is 100 in binary, so 4 has 2 trailing zeros.
 */
int HeightmapGenerator::numberOfTrailingZeros(int integer) {
	int trailingZeros = 32;
	while(integer != 0) {
		integer = integer << 1;
		trailingZeros--;
	}
	return trailingZeros;
}
