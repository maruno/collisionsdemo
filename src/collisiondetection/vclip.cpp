#define VCLIP_TEMPLATES
#include "vclip.hpp"

#include <cassert>
#include <algorithm>
#include <cmath>
#include <set>

#include <glm/gtx/simd_vec4.hpp>
#include <glm/gtx/simd_mat4.hpp>
#include <glm/gtc/matrix_inverse.hpp>

using namespace collisiondetection;

VClip::VClip(scene::SceneItem* myA, scene::SceneItem* myB) : itemA(myA), itemB(myB), verticesA(myA->getVBuffer().getVertexData()), verticesB(myB->getVBuffer().getVertexData()), normalsA(myA->getVBuffer().getNormalData()), normalsB(myB->getVBuffer().getNormalData()), facesA(myA->getVBuffer().getIndicesData()), facesB(myB->getVBuffer().getIndicesData()), swapped(false) {
}

#pragma mark Utility methods: generic

void VClip::swap() {
	swapped = !swapped;

	std::swap(stateVarsA, stateVarsB);
	std::swap(itemA, itemB);
	std::swap(verticesA, verticesB);
	std::swap(normalsA, normalsB);
	std::swap(facesA, facesB);
}

float VClip::ds(const glm::vec3& v, const glm::vec3& planeNormal) {
	glm::vec3 referencePoint = transformVerticeB(stateVarsB.vertexIdx);

	return glm::dot(planeNormal, (v - referencePoint));
}

glm::vec3 VClip::transformVerticeA(unsigned int idx) {
	glm::vec3 vA = verticesA[idx];
	glm::vec4 v4A = itemA->getModelMatrix() * glm::vec4(vA, 1.0f);
	vA = glm::vec3(v4A) / v4A.w;

	return std::move(vA);
}

glm::vec3 VClip::transformVerticeB(unsigned int idx) {
	glm::vec3 vB = verticesB[idx];
	glm::vec4 v4B = itemB->getModelMatrix() * glm::vec4(vB, 1.0f);
	vB = glm::vec3(v4B) / v4B.w;

	return std::move(vB);
}

#pragma mark Neighours

std::vector<std::pair<unsigned int, unsigned int>> VClip::neighboursForVertex(unsigned int vertexIdx) {
	std::vector<std::pair<unsigned int, unsigned int>> neighbours;

	for(const unsigned int faceIdx : facesContainingVertex(vertexIdx)) {
		const glm::uvec3& face = facesA[faceIdx];

		//CCW, search for right indices-> The other ones
		if(face.x == vertexIdx) {
			neighbours.emplace_back(face.x, face.y);
			neighbours.emplace_back(face.z, face.x);
		} else if(face.y == vertexIdx) {
			neighbours.emplace_back(face.y, face.z);
			neighbours.emplace_back(face.x, face.y);
		} else {
			neighbours.emplace_back(face.z, face.x);
			neighbours.emplace_back(face.y, face.z);
		}
	}

	return std::move(neighbours);
}

std::vector<unsigned int> VClip::facesContainingVertex(unsigned int vertexIdx) {
	std::vector<unsigned int> containingFaces;

	for(auto it = facesA.cbegin(); it != facesA.cend(); ++it) {
		const glm::uvec3& face = *it;
		if(face.x == vertexIdx || face.y == vertexIdx || face.z == vertexIdx) {
			containingFaces.push_back(std::distance(facesA.cbegin(), it));
		}
	}

	return std::move(containingFaces);
}

std::vector<unsigned int> VClip::facesContainingEdge(unsigned int vertexIdx, unsigned int edgeToVertexIdx) {
	std::vector<unsigned int> containingFaces;

	for(auto it = facesA.cbegin(); it != facesA.cend(); ++it) {
		const glm::uvec3& face = *it;

		bool containsV1 = face.x == vertexIdx || face.y == vertexIdx || face.z == vertexIdx;
		bool containsV2 = face.x == edgeToVertexIdx || face.y == edgeToVertexIdx || face.z == edgeToVertexIdx;

		if(containsV1 && containsV2) {
			containingFaces.push_back(std::distance(facesA.cbegin(), it));
		}
	}

	return std::move(containingFaces);
}

std::vector<std::pair<unsigned int, unsigned int>> VClip::neighboursForFace(unsigned int faceIdx) {
	const glm::uvec3& face = facesA[faceIdx];

	std::vector<std::pair<unsigned int, unsigned int>> neighbours {{face.x, face.y}, {face.y, face.z}, {face.z, face.x}};

	return std::move(neighbours);
}

#pragma mark Voronoi planes

bool VClip::violatesVP(const glm::vec3& planeNormal, const glm::vec3& vectorToPoint) {
	//VP-normal·pointVector < 0 -> point lies at the opposite side of the planeNormal
	return (glm::dot(vectorToPoint, planeNormal) < 0.0f);
}

std::vector<glm::vec3> VClip::voronoiPlanesForEdges(std::vector<std::pair<unsigned int, unsigned int>> edges) {
	std::vector<glm::vec3> planeNormals;

	for(std::pair<unsigned int, unsigned int>& edge : edges) {
		glm::vec3 t = transformVerticeA(edge.first);
		glm::vec3 h = transformVerticeA(edge.second);

		planeNormals.push_back(glm::normalize(t - h));
	}

	return std::move(planeNormals);
}

std::vector<glm::vec3> VClip::voronoiPlanesForEdgeFace(std::vector<std::pair<unsigned int, unsigned int>> edges, unsigned int faceIdx) {
	std::vector<glm::vec3> planeNormals;

	glm::mat3 normalMatrix(glm::inverseTranspose(itemA->getModelMatrix()));
	const glm::uvec3& face = facesA[faceIdx];
	glm::vec3 faceNormal = glm::normalize((normalsA[face.x] + normalsA[face.y] + normalsA[face.z]) / 3.0f);
	faceNormal = glm::normalize(faceNormal * normalMatrix);

	for(std::pair<unsigned int, unsigned int>& edge : edges) {
		glm::vec3 t = transformVerticeA(edge.first);
		glm::vec3 h = transformVerticeA(edge.second);

		planeNormals.push_back(glm::normalize(glm::cross(t - h, faceNormal)));
	}

	return std::move(planeNormals);
}

std::vector<glm::vec3> VClip::voronoiPlanesForEdgeFace(std::pair<unsigned int, unsigned int> edge, std::vector<unsigned int> faces) {
	std::vector<glm::vec3> planeNormals;

	glm::vec3 t = transformVerticeA(edge.first);
	glm::vec3 h = transformVerticeA(edge.second);

	glm::mat3 normalMatrix(glm::inverseTranspose(itemA->getModelMatrix()));

	for(unsigned int faceIdx : faces) {
		const glm::uvec3& face = facesA[faceIdx];
		glm::vec3 faceNormal = glm::normalize((normalsA[face.x] + normalsA[face.y] + normalsA[face.z]) / 3.0f);
		faceNormal = glm::normalize(faceNormal * normalMatrix);

		planeNormals.push_back(glm::normalize(glm::cross(faceNormal, t - h)));
	}

	return std::move(planeNormals);
}

std::vector<glm::vec3> VClip::voronoiPlanesForFaces(std::vector<unsigned int>& faces) {
	std::vector<glm::vec3> planeNormals;

	glm::mat3 normalMatrix(glm::inverseTranspose(itemA->getModelMatrix()));

	//Find voronoi planes
	for(auto it = faces.cbegin(); it != faces.cend(); ++it) {
		const glm::uvec3& face = facesA[*it];

		//Because we are dealing with triangles, each face delivers one voronoi plane
		glm::vec3 planeNormal = glm::normalize((normalsA[face.x] + normalsA[face.y] + normalsA[face.z]) / 3.0f);
		planeNormal = planeNormal * normalMatrix;

		planeNormals.push_back(glm::normalize(planeNormal));
	}

	return std::move(planeNormals);
}

#pragma mark Edge clipping

template<typename T>
VClip::ClipResult<T> VClip::clipEdge(const glm::vec3& t, const glm::vec3& h, T firstNeighbour, T pastLastNeightbour, const std::vector<glm::vec3>& planeNormals) {
	ClipResult<T> result;
	result.lebesgueBegin = 0.0f;
	result.lebesgueEnd = 1.0f;
	result.intersection = false;
	result.N1 = pastLastNeightbour;
	result.N2 = pastLastNeightbour;

	for(auto it = planeNormals.cbegin(); it != planeNormals.cend(); ++it) {
		T N = firstNeighbour + std::distance(planeNormals.cbegin(), it); //<- Neighbour feature of X corresponding to plane

		float dt = ds(t, *it);
		float dh = ds(h, *it);

		if(dt < 0.0f && dh < 0.0f) {
			result.N1 = result.N2 = N;
			return result;
		} else if(dt < 0.0f) {
			float lebesgue = dt/(dt-dh);
			if (lebesgue > result.lebesgueEnd) {
				result.lebesgueBegin = lebesgue;
				result.N1 = N;

				if(result.lebesgueBegin > result.lebesgueEnd) {
					return result;
				}
			}
		} else if(dh < 0.0f) {
			float lebesgue = dt/(dt-dh);
			if(lebesgue < result.lebesgueEnd) {
				result.lebesgueEnd = lebesgue;
				result.N2 = N;

				if(result.lebesgueBegin > result.lebesgueEnd) {
					return result;
				}
			}
		}
	}

	result.intersection = true;
	return result;
}

bool VClip::postClipDerivativeCheckVertex(ClipResult<std::vector<std::pair<unsigned int, unsigned int>>::iterator>& clipResult, std::vector<std::pair<unsigned int, unsigned int>>::iterator end, const glm::vec3& v,
					  const glm::vec3& t, const glm::vec3& h) {
	glm::vec3 u = (h - t);

	if(clipResult.N1 != end) {
		glm::vec3 e = (1.0f - clipResult.lebesgueBegin) * t + clipResult.lebesgueBegin * h;

		if(std::signbit(glm::dot(u, (e - v)))) {
			//Update V <- N1
			stateVarsA.vertexIdx = clipResult.N1->first;
			stateVarsA.edgeToVertexIdx = clipResult.N1->second;

			return true;
		}
	}

	if(clipResult.N2 != end) {
		glm::vec3 e = (1.0f - clipResult.lebesgueEnd) * t + clipResult.lebesgueEnd * h;

		if(std::signbit(glm::dot(u, (e - v)))) {
			//Update V <- N2
			stateVarsA.vertexIdx = clipResult.N2->first;
			stateVarsA.edgeToVertexIdx = clipResult.N2->second;

			return true;
		}
	}
	
	return false;
}

bool VClip::postClipDerivativeCheckVertexEdge(ClipResult<std::vector<unsigned int>::iterator> &clipResult, std::vector<unsigned int>::iterator end, const glm::vec3 &t, const glm::vec3 &h) {
	glm::vec3 u = (h - t);

	if(clipResult.N1 != end) {
		glm::vec3 e = (1.0f - clipResult.lebesgueBegin) * t + clipResult.lebesgueBegin * h;
		glm::vec3 v = transformVerticeA(*clipResult.N1);

		if(std::signbit(glm::dot(u, (e - v)))) {
			//Update E <- N1
			stateVarsA.vertexIdx = *clipResult.N1;

			return true;
		}
	}

	if(clipResult.N2 != end) {
		glm::vec3 e = (1.0f - clipResult.lebesgueEnd) * t + clipResult.lebesgueEnd * h;
		glm::vec3 v = transformVerticeA(*clipResult.N2);

		if(std::signbit(glm::dot(u, (e - v)))) {
			//Update E <- N2
			stateVarsA.vertexIdx = *clipResult.N2;

			return true;
		}
	}

	return false;
}

bool VClip::postClipDerivativeCheckFaceEdge(ClipResult<std::vector<unsigned int>::iterator> &clipResult, std::vector<unsigned int>::iterator end, const glm::vec3 &t, const glm::vec3 &h) {
	glm::vec3 u = (h - t);
	glm::mat3 normalMatrix(glm::inverseTranspose(itemA->getModelMatrix()));

	if(clipResult.N1 != end) {
		glm::vec3 e = (1.0f - clipResult.lebesgueBegin) * t + clipResult.lebesgueBegin * h;
		const glm::uvec3& face = facesA[*clipResult.N1];

		glm::vec3 planeNormal = glm::normalize((normalsA[face.x] + normalsA[face.y] + normalsA[face.z]) / 3.0f);
		planeNormal = glm::normalize(planeNormal * normalMatrix);

		bool update;
		if(std::signbit(ds(e, planeNormal))) {
			update = std::signbit(glm::dot(u, planeNormal));
		} else {
			update = !std::signbit(glm::dot(u, planeNormal));
		}

		if(update) {
			//Update E <- N1
			stateVarsA.faceIdx = *clipResult.N1;

			return true;
		}
	}

	if(clipResult.N2 != end) {
		glm::vec3 e = (1.0f - clipResult.lebesgueEnd) * t + clipResult.lebesgueEnd * h;
		const glm::uvec3& face = facesA[*clipResult.N2];

		glm::vec3 planeNormal = glm::normalize((normalsA[face.x] + normalsA[face.y] + normalsA[face.z]) / 3.0f);
		planeNormal = glm::normalize(planeNormal * normalMatrix);

		bool update;
		if(std::signbit(ds(e, planeNormal))) {
			update = std::signbit(glm::dot(u, planeNormal));
		} else {
			update = !std::signbit(glm::dot(u, planeNormal));
		}

		if(update) {
			//Update E <- N2
			stateVarsA.faceIdx = *clipResult.N2;

			return true;
		}
	}

	return false;
}

#pragma mark State handlers

VClip::State VClip::VV() {
	glm::vec3 vA = transformVerticeA(stateVarsA.vertexIdx);
	glm::vec3 vB = transformVerticeB(stateVarsB.vertexIdx);

	auto voronoiCheck = [this](const unsigned int activeIndex, const glm::vec3 vectorToV2){
		//Find voronoi planes
		std::vector<std::pair<unsigned int, unsigned int>> neighbourEdges = neighboursForVertex(activeIndex);
		std::vector<glm::vec3> planeNormals = voronoiPlanesForEdges(neighbourEdges);

		for(auto it = planeNormals.cbegin(); it != planeNormals.cend(); ++it) {
			if (violatesVP(*it, vectorToV2)) {
				//Update V <- E
				const std::pair<unsigned int, unsigned int>& violatedEdge = neighbourEdges[std::distance(planeNormals.cbegin(), it)];
				stateVarsA.vertexIdx = violatedEdge.first;
				stateVarsA.edgeToVertexIdx = violatedEdge.second;

				swap();
				return State::VE;
			}
		}

		return State::done;
	};

	State newState = voronoiCheck(stateVarsA.vertexIdx, glm::normalize(vB - vA));

	swap(); //Beware: Variables vA & vB are not swapped
	if(newState == State::done) {
		newState = voronoiCheck(stateVarsA.vertexIdx, glm::normalize(vA - vB));
	}

	return newState;
}

VClip::State VClip::VE() {
	glm::vec3 t = transformVerticeB(stateVarsB.vertexIdx);
	glm::vec3 h = transformVerticeB(stateVarsB.edgeToVertexIdx);

	glm::vec3 v = transformVerticeA(stateVarsA.vertexIdx);

	//glm::mat3 normalMatrix(glm::inverseTranspose(itemA->getModelMatrix()));

	glm::vec3 vectorToV = glm::normalize(v - t);

	//Check voronoi planes for E,F
	swap();
	std::vector<unsigned int> containingFaces = facesContainingEdge(stateVarsA.vertexIdx, stateVarsA.edgeToVertexIdx);
	std::vector<glm::vec3> planeNormals = voronoiPlanesForEdgeFace({stateVarsA.vertexIdx, stateVarsA.edgeToVertexIdx}, containingFaces);
	for(auto it = planeNormals.cbegin(); it != planeNormals.cend(); ++it) {
		if (violatesVP(*it, vectorToV)) {
			//Update E <- F
			stateVarsA.faceIdx = containingFaces[std::distance(planeNormals.cbegin(), it)];

			swap();
			return State::VF;
		}
	}

	//Check voronoi planes for V,E
	if(violatesVP(glm::normalize(h - t), vectorToV)) {
		//Update E <- V
		return State::VV;
	}

	vectorToV = glm::normalize(v - h);
	if(violatesVP(glm::normalize(t - h), vectorToV)) {
		//Update E <- V
		stateVarsA.vertexIdx = stateVarsA.edgeToVertexIdx;

		return State::VV;
	}

	swap();

	std::vector<std::pair<unsigned int, unsigned int>> neighbourEdges = neighboursForVertex(stateVarsA.vertexIdx);
	planeNormals = voronoiPlanesForEdges(neighbourEdges);
	
	ClipResult<std::vector<std::pair<unsigned int, unsigned int>>::iterator> clipResult = clipEdge(t, h, neighbourEdges.begin(), neighbourEdges.end(), planeNormals);

	bool updatedV = false;

	if(clipResult.N1 != neighbourEdges.cend() && clipResult.N2 != neighbourEdges.cend()) {
		if(clipResult.N1 == clipResult.N2) {
			//Update V <- N
			stateVarsA.vertexIdx = clipResult.N1->first;
			stateVarsA.edgeToVertexIdx = clipResult.N1->second;

			updatedV = true;
		}
	} else {
		//Check derivates and possible update V
		updatedV = postClipDerivativeCheckVertex(clipResult, neighbourEdges.end(), v, t, h);
	}

	if(updatedV) {
		return State::EE;
	}

	return State::done;
}

VClip::State VClip::VF() {
	float maxViolationDistance = 0.0f;
	bool foundViolation = false;
	unsigned int vEA, vEB;

	glm::vec3 v = transformVerticeA(stateVarsA.vertexIdx);
	glm::uvec3 face = facesB[stateVarsB.faceIdx];

	swap();

	//Check for maximally violating edge
	std::vector<std::pair<unsigned int, unsigned int>> faceEdges = neighboursForFace(stateVarsA.faceIdx);
	std::vector<glm::vec3> planeNormals = voronoiPlanesForEdgeFace(faceEdges, stateVarsA.faceIdx);
	if(violatesVP(planeNormals.at(0), (v - transformVerticeA(face.x)))) {
		vEA = face.x;
		vEB = face.y;

		maxViolationDistance = glm::abs(glm::dot(planeNormals.front(), v));
		foundViolation = true;
	}

	if(violatesVP(planeNormals.at(1), (v - transformVerticeA(face.y)))) {
		float violationDistance = glm::abs(glm::dot(planeNormals.front(), v));

		if(violationDistance > maxViolationDistance) {
			vEA = face.y;
			vEB = face.z;

			maxViolationDistance = violationDistance;
			foundViolation = true;
		}
	}

	if(violatesVP(planeNormals.at(2), (v - transformVerticeA(face.z)))) {
		float violationDistance = glm::abs(glm::dot(planeNormals.front(), v));

		if(violationDistance > maxViolationDistance) {
			vEA = face.z;
			vEB = face.x;

			maxViolationDistance = violationDistance;
			foundViolation = true;
		}
	}

	if(foundViolation) {
		//Update F <- E
		stateVarsA.vertexIdx = vEA;
		stateVarsA.edgeToVertexIdx = vEB;

		swap();
		return State::VE;
	}

	swap();

	glm::mat3 normalMatrix(glm::inverseTranspose(itemB->getModelMatrix()));
	glm::vec3 planeNormal = glm::normalize((normalsB[face.x] + normalsB[face.y] + normalsB[face.z]) / 3.0f);
	planeNormal = glm::normalize(planeNormal * normalMatrix);

	//Check for incident edges
	std::vector<unsigned int> containingFaces = facesContainingVertex(stateVarsA.vertexIdx);
	for(auto it = containingFaces.cbegin(); it != containingFaces.cend(); ++it) {
		const glm::uvec3& face = facesA[*it];

		if(face.x == stateVarsA.vertexIdx) {
			const glm::vec3 vY = transformVerticeA(face.y);
			if(ds(v, planeNormal) > ds(vY, planeNormal)) {
				//Update V <- E
				stateVarsA.edgeToVertexIdx = face.y;

				return State::EF;
			}
		} else if(face.y == stateVarsA.vertexIdx) {
			const glm::vec3 vZ = transformVerticeA(face.z);
			if(ds(v, planeNormal) > ds(vZ, planeNormal)) {
				//Update V <- E
				stateVarsA.edgeToVertexIdx = face.z;

				return State::EF;
			}
		} else {
			const glm::vec3 vX = transformVerticeA(face.x);
			if(ds(v, planeNormal) > ds(vX, planeNormal)) {
				//Update V <- E
				stateVarsA.edgeToVertexIdx = face.x;

				return State::EF;
			}
		}
	}

	if(ds(v, planeNormal) > 0.0f) {
		return State::done;
	}

	//Lodged in a local minimal. Implement MERL TR97-05 algorithm 3? Possibly just penetration
	assert(0);
}

VClip::State VClip::EE() {
	auto EERun = [this]() {
		const glm::vec3 t = transformVerticeB(stateVarsB.vertexIdx);
		const glm::vec3 h = transformVerticeB(stateVarsB.edgeToVertexIdx);
		
		//Clip EB to VP(E1,V)
		const glm::vec3 tA = transformVerticeA(stateVarsA.vertexIdx);
		const glm::vec3 hA = transformVerticeA(stateVarsA.edgeToVertexIdx);

		std::vector<unsigned int> neighbourVertices = {stateVarsA.vertexIdx, stateVarsA.edgeToVertexIdx};
		std::vector<glm::vec3> planeNormals = {glm::normalize(tA - hA), glm::normalize(hA - tA)};
		ClipResult<std::vector<unsigned int>::iterator> clipResult = clipEdge(t, h, neighbourVertices.begin(), neighbourVertices.end(), planeNormals);

		if(!clipResult.intersection) {
			if(clipResult.N1 == clipResult.N2) {
				//Simply excluded by N, update E <- V
				stateVarsA.vertexIdx = *clipResult.N1;

				return State::VE;
			}
		}

		if(postClipDerivativeCheckVertexEdge(clipResult, neighbourVertices.end(), t, h)) {
			return State::VE;
		}

		//Clip E2 to VP(E,F)
		std::vector<unsigned int> neighbourFaces = facesContainingEdge(stateVarsA.vertexIdx, stateVarsA.edgeToVertexIdx);
		planeNormals = voronoiPlanesForEdgeFace({stateVarsA.vertexIdx, stateVarsA.edgeToVertexIdx}, neighbourFaces);
		clipResult = clipEdge(t, h, neighbourFaces.begin(), neighbourFaces.end(), planeNormals);

		if(!clipResult.intersection) {
			if(clipResult.N1 == clipResult.N2) {
				//Simply excluded by N, update E <- F
				stateVarsA.faceIdx = *clipResult.N1;

				swap();
				return State::EF;
			}
		}

		if(postClipDerivativeCheckFaceEdge(clipResult, neighbourFaces.end(), t, h)) {
			swap();
			return State::EF;
		}

		return State::done;
	};

	State newState = EERun();

	if(newState == State::done) {
		swap();
		newState = EERun();
	}

	return newState;
}

VClip::State VClip::EF() {
	swap();
	std::vector<std::pair<unsigned int, unsigned int>> neighbours = neighboursForFace(stateVarsA.faceIdx);
	std::vector<glm::vec3> planeNormals = voronoiPlanesForEdgeFace(neighbours, stateVarsA.faceIdx);

	const glm::vec3 t = transformVerticeB(stateVarsB.vertexIdx);
	const glm::vec3 h = transformVerticeB(stateVarsB.edgeToVertexIdx);

	ClipResult<std::vector<std::pair<unsigned int, unsigned int>>::iterator> clipResult = clipEdge(t, h, neighbours.begin(), neighbours.end(), planeNormals);

	if(!clipResult.intersection) {
		//TODO Update F <- Closest egde or vertex on F to E
		assert(0);
	}

	const glm::uvec3& face = facesA[stateVarsA.faceIdx];
	glm::mat3 normalMatrix(glm::inverseTranspose(itemA->getModelMatrix()));

	glm::vec3 planeNormal = glm::normalize((normalsA[face.x] + normalsA[face.y] + normalsA[face.z]) / 3.0f);
	planeNormal = glm::normalize(planeNormal * normalMatrix);

	glm::vec3 eBegin = (1.0f - clipResult.lebesgueBegin) * t + clipResult.lebesgueBegin * h;
	float dBegin = ds(eBegin, planeNormal);

	glm::vec3 eEnd = (1.0f - clipResult.lebesgueEnd) * t + clipResult.lebesgueEnd * h;
	float dEnd = ds(eEnd, planeNormal);

	if(dBegin * dEnd <= 0.0f) {
		return State::penetrating;
	}

	glm::vec3 u = (h - t);

	bool foo;
	if(std::signbit(ds(eBegin, planeNormal))) {
		foo = std::signbit(glm::dot(u, planeNormal));
	} else {
		foo = !std::signbit(glm::dot(u, planeNormal));
	}

	if(foo) {
		if(clipResult.N1 != neighbours.end()) {
			//Update F <- N1
			stateVarsA.vertexIdx = clipResult.N1->first;
			stateVarsA.edgeToVertexIdx = clipResult.N1->second;

			return State::EE;
		} else {
			//Update E <- tail(E)
			stateVarsB.vertexIdx = stateVarsB.edgeToVertexIdx;

			swap();
			return State::VF;
		}
	} else {
		if(clipResult.N2 != neighbours.end()) {
			//Update F <- N2
			stateVarsA.vertexIdx = clipResult.N2->first;
			stateVarsA.edgeToVertexIdx = clipResult.N2->second;

			return State::EE;
		} else {
			//Update E <- head(E)
			swap();
			return State::VF;
		}
	}
}

#pragma mark -

bool VClip::run() {
	State state = State::VV;
	stateVarsA.vertexIdx = 0;
	stateVarsB.vertexIdx = 0;

	state = State::VV;

	while (true) {
		switch (state) {
			case State::VV:
				state = VV();
				break;
			case State::VE:
				state = VE();
				break;
			case State::VF:
				state = VF();
				break;
			case State::EE:
				state = EE();
				break;
			case State::EF:
				state = EF();
				break;
			case State::done:
				return false;
			case State::penetrating:
				return true;
		}
	}
}