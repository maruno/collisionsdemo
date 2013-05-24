#define VCLIP_TEMPLATES
#include "vclip.hpp"

#include <cassert>
#include <algorithm>
#include <cmath>
#include <set>
#include <limits>

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

template<>
float VClip::ds(const glm::vec3& v, const glm::vec3& planeNormal, glm::vec3 referencePoint) {
	return glm::dot(planeNormal, (v - referencePoint));
}

template<>
float VClip::ds(const glm::vec3& v, const glm::vec3& planeNormal, std::pair<unsigned int, unsigned int> reference) {
	glm::vec3 referencePoint = transformVerticeA(reference.first);

	return glm::dot(planeNormal, (v - referencePoint));
}

//Use long for faces to differentiate
template<>
float VClip::ds(const glm::vec3 &v, const glm::vec3 &planeNormal, unsigned long reference) {
	glm::vec3 referencePoint = transformVerticeA(facesA[reference].x);

	return glm::dot(planeNormal, (v - referencePoint));
}

template<>
float VClip::ds(const glm::vec3 &v, const glm::vec3 &planeNormal, unsigned int reference) {
	glm::vec3 referencePoint = transformVerticeA(reference);

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

std::vector<std::pair<unsigned int, unsigned int>> VClip::neighboursForVertex(unsigned int vertexIdx, const glm::uvec3& face) {
	std::vector<std::pair<unsigned int, unsigned int>> neighbours;

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

	return std::move(neighbours);
}

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

std::vector<unsigned long> VClip::facesContainingEdge(unsigned int vertexIdx, unsigned int edgeToVertexIdx) {
	std::vector<unsigned long> containingFaces;

	for(auto it = facesA.cbegin(); it != facesA.cend(); ++it) {
		const glm::uvec3& face = *it;

		bool containsV1 = face.x == vertexIdx || face.y == vertexIdx || face.z == vertexIdx;
		bool containsV2 = face.x == edgeToVertexIdx || face.y == edgeToVertexIdx || face.z == edgeToVertexIdx;

		if(containsV1 && containsV2) {
			containingFaces.push_back(std::distance(facesA.cbegin(), it));
		}
	}

	assert(containingFaces.size() == 2);

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
	//Because coplanar-cases are problematic we use a tolerance of 0.1
	return (glm::dot(vectorToPoint, planeNormal) < -0.1f);
}

std::vector<glm::vec3> VClip::voronoiPlanesForEdges(std::vector<std::pair<unsigned int, unsigned int>> edges, unsigned int reference) {
	std::vector<glm::vec3> planeNormals;

	for(std::pair<unsigned int, unsigned int>& edge : edges) {
		glm::vec3 t, h;

		//Correct for CCW
		if(reference == edge.first) {
			t = transformVerticeA(edge.first);
			h = transformVerticeA(edge.second);
		} else {
			t = transformVerticeA(edge.second);
			h = transformVerticeA(edge.first);
		}

		planeNormals.push_back(glm::normalize(t - h));
	}

	return std::move(planeNormals);
}

std::vector<glm::vec3> VClip::voronoiPlanesForEdgeFace(std::vector<std::pair<unsigned int, unsigned int>> edges, unsigned int faceIdx) {
	std::vector<glm::vec3> planeNormals;

	glm::mat3 normalMatrix(glm::inverseTranspose(itemA->getModelMatrix()));
	const glm::uvec3& face = facesA[faceIdx];
	glm::vec3 faceNormal = glm::normalize((normalsA[face.x] + normalsA[face.y] + normalsA[face.z]) / 3.0f);
	faceNormal = glm::normalize(normalMatrix * faceNormal);

	for(std::pair<unsigned int, unsigned int>& edge : edges) {
		glm::vec3 t = transformVerticeA(edge.first);
		glm::vec3 h = transformVerticeA(edge.second);

		planeNormals.push_back(glm::normalize(glm::cross(faceNormal, h - t)));
	}

	return std::move(planeNormals);
}

std::vector<glm::vec3> VClip::voronoiPlanesForEdgeFace(std::pair<unsigned int, unsigned int> edge, std::vector<unsigned long> faces) {
	std::vector<glm::vec3> planeNormals;

	glm::mat3 normalMatrix(glm::inverseTranspose(itemA->getModelMatrix()));

	for(unsigned int faceIdx : faces) {
		const glm::uvec3& face = facesA[faceIdx];
		glm::vec3 faceNormal = glm::normalize((normalsA[face.x] + normalsA[face.y] + normalsA[face.z]) / 3.0f);
		faceNormal = glm::normalize(normalMatrix * faceNormal);

		glm::vec3 t, h;

		//Correct for CCW
		if((face.x == edge.first && face.y == edge.second) || (face.y == edge.first && face.z == edge.second) || (face.z == edge.first && face.x == edge.second)) {
			t = transformVerticeA(edge.first);
			h = transformVerticeA(edge.second);
		} else {
			t = transformVerticeA(edge.second);
			h = transformVerticeA(edge.first);
		}

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
		planeNormal = normalMatrix * planeNormal;

		planeNormals.push_back(glm::normalize(planeNormal));
	}

	return std::move(planeNormals);
}

#pragma mark Edge clipping

template<typename T>
VClip::ClipResult<T> VClip::clipEdge(const glm::vec3& t, const glm::vec3& h, T firstNeighbour, T pastLastNeightbour, const std::vector<glm::vec3>& planeNormals, float lebesgueContinueBegin, float lebesgueContinueEnd, unsigned int* reference) {
	ClipResult<T> result;
	result.lebesgueBegin = lebesgueContinueBegin;
	result.lebesgueEnd = lebesgueContinueEnd;
	result.intersection = false;
	result.N1 = pastLastNeightbour;
	result.N2 = pastLastNeightbour;

	for(auto it = planeNormals.cbegin(); it != planeNormals.cend(); ++it) {
		T N = firstNeighbour + std::distance(planeNormals.cbegin(), it); //<- Neighbour feature of X corresponding to plane

		float dt, dh;
		if(reference) {
			dt = ds(t, *it, *reference);
			dh = ds(h, *it, *reference);
		} else {
			dt = ds(t, *it, *N);
			dh = ds(h, *it, *N);
		}

		if(dt < 0.0f && dh < 0.0f) {
			result.N1 = result.N2 = N;
			return result;
		} else if(dt < 0.0f) {
			float lebesgue = dt/(dt-dh);
			if (lebesgue > result.lebesgueBegin) {
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

VClip::State VClip::postClipDerivativeCheckVertex(ClipResult<std::vector<std::pair<unsigned int, unsigned int>>::iterator>& clipResult, std::vector<std::pair<unsigned int, unsigned int>>::iterator end, const glm::vec3& v,
					  const glm::vec3& t, const glm::vec3& h) {
	glm::vec3 u = (h - t);

	if(clipResult.N1 != end) {
		glm::vec3 e = (1.0f - clipResult.lebesgueBegin) * t + clipResult.lebesgueBegin * h;

		glm::vec3 eMinV = e - v;
		if(eMinV.x > -0.01f && eMinV.x < 0.01f && eMinV.y > -0.01f && eMinV.y < 0.01f && eMinV.z > -0.01f && eMinV.z < -0.01f) {
			return State::penetrating;
		}

		if(std::signbit(glm::dot(u, (e - v)))) {
			//Update V <- N1
			stateVarsA.vertexIdx = clipResult.N1->first;
			stateVarsA.edgeToVertexIdx = clipResult.N1->second;

			return State::EE;
		}

		return State::done;
	}

	if(clipResult.N2 != end) {
		glm::vec3 e = (1.0f - clipResult.lebesgueEnd) * t + clipResult.lebesgueEnd * h;

		glm::vec3 eMinV = e - v;
		if(eMinV.x > -0.01f && eMinV.x < 0.01f && eMinV.y > -0.01f && eMinV.y < 0.01f && eMinV.z > -0.01f && eMinV.z < -0.01f) {
			return State::penetrating;
		}

		if(!std::signbit(glm::dot(u, (e - v)))) {
			//Update V <- N2
			stateVarsA.vertexIdx = clipResult.N2->first;
			stateVarsA.edgeToVertexIdx = clipResult.N2->second;

			return State::EE;
		}
	}
	
	return State::done;
}

VClip::State VClip::postClipDerivativeCheckVertexEdge(ClipResult<std::vector<unsigned int>::iterator> &clipResult, std::vector<unsigned int>::iterator end, const glm::vec3 &t, const glm::vec3 &h) {
	glm::vec3 u = (h - t);

	if(clipResult.N1 != end) {
		glm::vec3 e = (1.0f - clipResult.lebesgueBegin) * t + clipResult.lebesgueBegin * h;
		glm::vec3 v = transformVerticeA(*clipResult.N1);

		glm::vec3 eMinV = e - v;
		if(eMinV.x > -0.01f && eMinV.x < 0.01f && eMinV.y > -0.01f && eMinV.y < 0.01f && eMinV.z > -0.01f && eMinV.z < -0.01f) {
			return State::penetrating;
		}

		if(std::signbit(glm::dot(u, (e - v)))) {
			//Update E <- N1
			stateVarsA.vertexIdx = *clipResult.N1;

			return State::VE;
		}

		return State::done;
	}

	if(clipResult.N2 != end) {
		glm::vec3 e = (1.0f - clipResult.lebesgueEnd) * t + clipResult.lebesgueEnd * h;
		glm::vec3 v = transformVerticeA(*clipResult.N2);

		glm::vec3 eMinV = e - v;
		if(eMinV.x > -0.01f && eMinV.x < 0.01f && eMinV.y > -0.01f && eMinV.y < 0.01f && eMinV.z > -0.01f && eMinV.z < -0.01f) {
			return State::penetrating;
		}

		if(!std::signbit(glm::dot(u, (e - v)))) {
			//Update E <- N2
			stateVarsA.vertexIdx = *clipResult.N2;

			return State::VE;
		}
	}

	return State::done;
}

VClip::State VClip::postClipDerivativeCheckFaceEdge(ClipResult<std::vector<unsigned long>::iterator> &clipResult, std::vector<unsigned long>::iterator end, const glm::vec3 &t, const glm::vec3 &h) {
	glm::vec3 u = (h - t);
	glm::mat3 normalMatrix(glm::inverseTranspose(itemA->getModelMatrix()));

	if(clipResult.N1 != end) {
		glm::vec3 e = (1.0f - clipResult.lebesgueBegin) * t + clipResult.lebesgueBegin * h;
		const glm::uvec3& face = facesA[*clipResult.N1];

		glm::vec3 planeNormal = glm::normalize((normalsA[face.x] + normalsA[face.y] + normalsA[face.z]) / 3.0f);
		planeNormal = glm::normalize(normalMatrix * planeNormal);

		float de = ds(e, planeNormal, transformVerticeA(face.x));

		if(de > -0.01 && de < 0.01) {
			return State::penetrating;
		}

		bool foo;
		if(std::signbit(de)) {
			foo = std::signbit(glm::dot(u, planeNormal));
		} else {
			foo = !std::signbit(glm::dot(u, planeNormal));
		}

		if(foo) {
			//Update E <- N1
			stateVarsA.faceIdx = *clipResult.N1;

			swap();
			return State::EF;
		}

		return State::done;
	}

	if(clipResult.N2 != end) {
		glm::vec3 e = (1.0f - clipResult.lebesgueEnd) * t + clipResult.lebesgueEnd * h;
		const glm::uvec3& face = facesA[*clipResult.N2];

		glm::vec3 planeNormal = glm::normalize((normalsA[face.x] + normalsA[face.y] + normalsA[face.z]) / 3.0f);
		planeNormal = glm::normalize(normalMatrix * planeNormal);

		float de = ds(e, planeNormal, transformVerticeA(face.x));

		if(de > -0.01 && de < 0.01) {
			return State::penetrating;
		}

		bool foo;
		if(std::signbit(de)) {
			foo = std::signbit(glm::dot(u, planeNormal));
		} else {
			foo = !std::signbit(glm::dot(u, planeNormal));
		}

		if(!foo) {
			//Update E <- N2
			stateVarsA.faceIdx = *clipResult.N2;

			swap();
			return State::EF;
		}
	}

	return State::done;
}

#pragma mark State handlers

VClip::State VClip::VV() {
	glm::vec3 vA = transformVerticeA(stateVarsA.vertexIdx);
	glm::vec3 vB = transformVerticeB(stateVarsB.vertexIdx);

	auto voronoiCheck = [this](const unsigned int activeIndex, const glm::vec3 vectorToV2){
		//Find voronoi planes
		std::vector<std::pair<unsigned int, unsigned int>> neighbourEdges = neighboursForVertex(activeIndex);
		std::vector<glm::vec3> planeNormals = voronoiPlanesForEdges(neighbourEdges, activeIndex);

		for(auto it = planeNormals.cbegin(); it != planeNormals.cend(); ++it) {
			if (violatesVP(*it, vectorToV2)) {
				const std::pair<unsigned int, unsigned int>& violatedEdge = neighbourEdges[std::distance(planeNormals.cbegin(), it)];

				//Update V <- E
				stateVarsA.vertexIdx = violatedEdge.first;
				stateVarsA.edgeToVertexIdx = violatedEdge.second;

				swap();
				return State::VE;
			}
		}

		return State::done;
	};

	State newState = voronoiCheck(stateVarsA.vertexIdx, glm::normalize(vB - vA));

	if(newState == State::done) {
		swap(); //Beware: Variables vA & vB are not swapped
		newState = voronoiCheck(stateVarsA.vertexIdx, glm::normalize(vA - vB));
	}

	return newState;
}


VClip::State VClip::VE() {
	glm::vec3 t = transformVerticeB(stateVarsB.vertexIdx);
	glm::vec3 h = transformVerticeB(stateVarsB.edgeToVertexIdx);

	glm::vec3 v = transformVerticeA(stateVarsA.vertexIdx);

	glm::vec3 vectorToV = glm::normalize(v - t);

	swap();

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

	//Check voronoi planes for E,F
	std::vector<unsigned long> containingFaces = facesContainingEdge(stateVarsA.vertexIdx, stateVarsA.edgeToVertexIdx);
	std::vector<glm::vec3> planeNormals = voronoiPlanesForEdgeFace({stateVarsA.vertexIdx, stateVarsA.edgeToVertexIdx}, containingFaces);
	for(auto it = planeNormals.cbegin(); it != planeNormals.cend(); ++it) {
		if (violatesVP(*it, vectorToV)) {
			//Update E <- F
			stateVarsA.faceIdx = containingFaces[std::distance(planeNormals.cbegin(), it)];

			swap();
			return State::VF;
		}
	}
	
	swap();

	std::vector<std::pair<unsigned int, unsigned int>> neighbourEdges = neighboursForVertex(stateVarsA.vertexIdx);
	planeNormals = voronoiPlanesForEdges(neighbourEdges, stateVarsA.vertexIdx);
	
	ClipResult<std::vector<std::pair<unsigned int, unsigned int>>::iterator> clipResult = clipEdge(t, h, neighbourEdges.begin(), neighbourEdges.end(), planeNormals, 0.0f, 1.0f, &stateVarsA.vertexIdx);

	if(clipResult.N1 != neighbourEdges.end() && clipResult.N2 != neighbourEdges.end()) {
		if(clipResult.N1 == clipResult.N2) {
			//Update V <- N
			stateVarsA.vertexIdx = clipResult.N1->first;
			stateVarsA.edgeToVertexIdx = clipResult.N1->second;

			return State::EE;
		}
	} else {
		//Check derivates and possible update V
		return postClipDerivativeCheckVertex(clipResult, neighbourEdges.end(), v, t, h);
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

	glm::vec3 referencePoint = transformVerticeA(face.x);
	glm::vec3 vectorToV = glm::normalize(v - referencePoint);
	if(violatesVP(planeNormals.at(0), vectorToV)) {
		vEA = face.x;
		vEB = face.y;

		maxViolationDistance = ds(v, planeNormals.at(0), referencePoint);
		foundViolation = true;
	}

	referencePoint = transformVerticeA(face.y);
	vectorToV = glm::normalize(v - referencePoint);
	if(violatesVP(planeNormals.at(1), vectorToV)) {
		float violationDistance = ds(v, planeNormals.at(1), referencePoint);

		if(violationDistance > maxViolationDistance) {
			vEA = face.y;
			vEB = face.z;

			maxViolationDistance = violationDistance;
			foundViolation = true;
		}
	}

	referencePoint = transformVerticeA(face.z);
	vectorToV = glm::normalize(v - referencePoint);
	if(violatesVP(planeNormals.at(2), vectorToV)) {
		float violationDistance = ds(v, planeNormals.at(2), referencePoint);

		if(violationDistance > maxViolationDistance) {
			vEA = face.z;
			vEB = face.x;

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

	referencePoint = transformVerticeB(face.x);

	glm::mat3 normalMatrix(glm::inverseTranspose(itemB->getModelMatrix()));
	glm::vec3 planeNormal = glm::normalize((normalsB[face.x] + normalsB[face.y] + normalsB[face.z]) / 3.0f);
	planeNormal = glm::normalize(normalMatrix * planeNormal);

	float absoluteDistanceFromV = glm::abs(ds(v, planeNormal, referencePoint));

	//Check for incident edges
	std::vector<unsigned int> containingFaces = facesContainingVertex(stateVarsA.vertexIdx);
	for(auto it = containingFaces.cbegin(); it != containingFaces.cend(); ++it) {
		const glm::uvec3& possibleFace = facesA[*it];
		if(face.x == stateVarsA.vertexIdx) {
			const glm::vec3 vY = transformVerticeA(possibleFace.y);
			if(absoluteDistanceFromV > glm::abs(ds(vY, planeNormal, referencePoint))) {
				//Update V <- E
				stateVarsA.edgeToVertexIdx = possibleFace.y;

				return State::EF;
			}
		} else if(face.y == stateVarsA.vertexIdx) {
			const glm::vec3 vZ = transformVerticeA(possibleFace.z);
			if(absoluteDistanceFromV > glm::abs(ds(vZ, planeNormal, referencePoint))) {
				//Update V <- E
				stateVarsA.edgeToVertexIdx = possibleFace.z;

				return State::EF;
			}
		} else {
			const glm::vec3 vX = transformVerticeA(possibleFace.x);
			if(absoluteDistanceFromV > glm::abs(ds(vX, planeNormal, referencePoint))) {
				//Update V <- E
				stateVarsA.edgeToVertexIdx = possibleFace.x;

				return State::EF;
			}
		}
	}

	if(ds(v, planeNormal, referencePoint) > 0.0f) {
		return State::done;
	}

	//Lodged in a local minimal. Implement MERL TR97-05 algorithm 3? Possibly just penetration
	return State::penetrating;
}

VClip::State VClip::EE() {
	std::pair<unsigned int, unsigned int> edgeA = {stateVarsA.vertexIdx, stateVarsA.edgeToVertexIdx};
	std::pair<unsigned int, unsigned int> edgeB = {stateVarsB.vertexIdx, stateVarsB.edgeToVertexIdx};

	//Check for edge-edge loop
	if(previousEdgeEdgeA == edgeA) {
		swap();
		return EELocalMinEscape();
	} else if(previousEdgeEdgeB == edgeB) {
		return EELocalMinEscape();
	}

	swap();
	std::swap(edgeA, edgeB);

	if(previousEdgeEdgeA == edgeA) {
		swap();
		return EELocalMinEscape();
	} else if(previousEdgeEdgeB == edgeB) {
		return EELocalMinEscape();
	}

	swap();
	std::swap(edgeA, edgeB);

	previousEdgeEdgeA = edgeA;
	previousEdgeEdgeB = edgeB;

	auto EERun = [this]() {
		const glm::vec3 t = transformVerticeB(stateVarsB.vertexIdx);
		const glm::vec3 h = transformVerticeB(stateVarsB.edgeToVertexIdx);
		
		//Clip E2 to VP(E1,V)
		const glm::vec3 tA = transformVerticeA(stateVarsA.vertexIdx);
		const glm::vec3 hA = transformVerticeA(stateVarsA.edgeToVertexIdx);

		std::vector<unsigned int> neighbourVertices = {stateVarsA.vertexIdx, stateVarsA.edgeToVertexIdx};
		std::vector<glm::vec3> planeNormals = {glm::normalize(hA - tA), glm::normalize(tA - hA)};
		
		ClipResult<std::vector<unsigned int>::iterator> clipResult = clipEdge(t, h, neighbourVertices.begin(), neighbourVertices.end(), planeNormals);

		if(!clipResult.intersection) {
			if(clipResult.N1 == clipResult.N2) {
				//Simply excluded by N, update E <- V
				stateVarsA.vertexIdx = *clipResult.N1;

				return State::VE;
			}
		}

		if(postClipDerivativeCheckVertexEdge(clipResult, neighbourVertices.end(), t, h) == State::VE) {
			return State::VE;
		}

		//Clip E2 to VP(E,F)
		std::vector<unsigned long> neighbourFaces = facesContainingEdge(stateVarsA.vertexIdx, stateVarsA.edgeToVertexIdx);
		planeNormals = voronoiPlanesForEdgeFace({stateVarsA.vertexIdx, stateVarsA.edgeToVertexIdx}, neighbourFaces);

		ClipResult<std::vector<unsigned long>::iterator> clipResultF = clipEdge(t, h, neighbourFaces.begin(), neighbourFaces.end(), planeNormals, clipResult.lebesgueBegin, clipResult.lebesgueEnd);

		if(!clipResultF.intersection) {
			if(clipResultF.N1 == clipResultF.N2) {
				//Simply excluded by N, update E <- F
				stateVarsA.faceIdx = *clipResultF.N1;

				swap();
				return State::EF;
			}
		}

		return postClipDerivativeCheckFaceEdge(clipResultF, neighbourFaces.end(), t, h);
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

	const glm::uvec3& face = facesA[stateVarsA.faceIdx];

	const glm::vec3 t = transformVerticeB(stateVarsB.vertexIdx);
	const glm::vec3 h = transformVerticeB(stateVarsB.edgeToVertexIdx);

	ClipResult<std::vector<std::pair<unsigned int, unsigned int>>::iterator> clipResult = clipEdge(t, h, neighbours.begin(), neighbours.end(), planeNormals);

	if(!clipResult.intersection) {
		//Update F <- Closest egde or vertex on F to E
		bool final = false;
		bool lastScanWasV = false;
		
		auto scanCheckE = [&, this](std::pair<unsigned int, unsigned int> edge) {
			lastScanWasV = false;

			const glm::vec3 tA = transformVerticeA(edge.first);
			const glm::vec3 hA = transformVerticeA(edge.second);

			std::vector<unsigned int> neighbourVertices = {edge.first, edge.second};
			std::vector<glm::vec3> planeNormals = {glm::normalize(hA - tA), glm::normalize(tA - hA)};

			ClipResult<std::vector<unsigned int>::iterator> clipResultV = clipEdge(t, h, neighbourVertices.begin(), neighbourVertices.end(), planeNormals);
			if(clipResultV.intersection) {
				if(postClipDerivativeCheckVertexEdge(clipResultV, neighbourVertices.end(), t, h) == State::VE) {
					//One of the endpoints is closer
					return State::VE;
				} else {
					//This edge is closest
					final = true;

					stateVarsA.vertexIdx = edge.first;
					stateVarsA.edgeToVertexIdx = edge.second;

					return State::EE;
				}
			}
			return State::done;
		};

		auto scanCheckV = [&, this](unsigned int vertexIdx) {
			lastScanWasV = true;

			std::vector<std::pair<unsigned int, unsigned int>> neighbours = neighboursForVertex(vertexIdx, face);
			std::vector<glm::vec3> planeNormals = voronoiPlanesForEdges(neighbours, vertexIdx);

			ClipResult<std::vector<std::pair<unsigned int, unsigned int>>::iterator> clipResultE = clipEdge(t, h, neighbours.begin(), neighbours.end(), planeNormals, 0.0f, 1.0f, &vertexIdx);
			if(clipResultE.intersection) {
				const glm::vec3 v = transformVerticeA(vertexIdx);
				if(postClipDerivativeCheckVertex(clipResultE, neighbours.end(), v, t, h) == State::EE) {
					//One of the neighbours is closer
					return State::EE;
				} else {
					//This vertice is closest
					final = true;

					stateVarsA.vertexIdx = vertexIdx;

					return State::VE;
				}
			}

			return State::done;
		};

		std::vector<std::pair<unsigned int, unsigned int>>::iterator it;

		if(clipResult.N1 != neighbours.end()) {
			it = clipResult.N1;
		} else if(clipResult.N2 != neighbours.end()) {
			it = clipResult.N2;
		} else {
			it = neighbours.begin();
		}

		State scanState = State::done;

		while(!final) {
			switch (scanState) {
				case State::VE:
				{
					unsigned int vertexIdx = stateVarsA.vertexIdx;

					State nextState = scanCheckE(*it);

					if(nextState == State::VE && vertexIdx == stateVarsA.vertexIdx) {
						final = true;
					} else {
						scanState = nextState;
					}
				}

					break;

				case State::EE:
				{
					if(stateVarsA.vertexIdx == it->second && stateVarsA.edgeToVertexIdx != it->first) {
						std::swap(stateVarsA.vertexIdx, stateVarsA.edgeToVertexIdx);
					}
				}
				default:
				{
					if(lastScanWasV) {
						scanState = scanCheckE(*it);
					} else {
						scanState = scanCheckV(it->first);
					}
				}
					break;
			}

			if (!lastScanWasV) {
				++it;
				if(it == neighbours.end()) {
					it = neighbours.begin();
				}
			}
		}

		return scanState;
	}

	glm::mat3 normalMatrix(glm::inverseTranspose(itemA->getModelMatrix()));

	glm::vec3 referencePoint = transformVerticeA(face.x);
	glm::vec3 faceNormal = glm::normalize((normalsA[face.x] + normalsA[face.y] + normalsA[face.z]) / 3.0f);
	faceNormal = glm::normalize(normalMatrix * faceNormal);

	glm::vec3 eBegin = (1.0f - clipResult.lebesgueBegin) * t + clipResult.lebesgueBegin * h;
	float dBegin = ds(eBegin, faceNormal, referencePoint);

	glm::vec3 eEnd = (1.0f - clipResult.lebesgueEnd) * t + clipResult.lebesgueEnd * h;
	float dEnd = ds(eEnd, faceNormal, referencePoint);

	if(dBegin * dEnd <= 0.0f) {
		return State::penetrating;
	}

	glm::vec3 u = (h - t);

	bool foo;
	if(std::signbit(ds(eBegin, faceNormal, referencePoint))) {
		foo = std::signbit(glm::dot(u, faceNormal));
	} else {
		foo = !std::signbit(glm::dot(u, faceNormal));
	}

	if(foo) {
		if(clipResult.N1 != neighbours.end()) {
			//Update F <- N1
			stateVarsA.vertexIdx = clipResult.N1->first;
			stateVarsA.edgeToVertexIdx = clipResult.N1->second;

			return State::EE;
		} else {
			//Update E <- tail(E)
			stateVarsB.vertexIdx = stateVarsB.vertexIdx;

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
			stateVarsB.vertexIdx = stateVarsB.edgeToVertexIdx;

			swap();
			return State::VF;
		}
	}
}

#pragma mark Loop escapers
VClip::State VClip::EELocalMinEscape() {
	float min = 0.0f;
	float max = 1.0f;
	float maxd = -std::numeric_limits<float>::infinity();

	swap();

	const glm::vec3 t = transformVerticeB(stateVarsB.vertexIdx);
	const glm::vec3 h = transformVerticeB(stateVarsB.edgeToVertexIdx);

	unsigned long maxFace = 0;

	for(auto it = facesA.begin(); it != facesA.end(); ++it) {
		const glm::uvec3& face = *it;

		glm::mat3 normalMatrix(glm::inverseTranspose(itemA->getModelMatrix()));
		glm::vec3 faceNormal = glm::normalize((normalsA[face.x] + normalsA[face.y] + normalsA[face.z]) / 3.0f);
		faceNormal = glm::normalize(normalMatrix * faceNormal);

		unsigned long faceIdx = std::distance(facesA.begin(), it);

		float dt = ds(t, faceNormal, faceIdx);
		float dh = ds(h, faceNormal, faceIdx);

		if (dt > maxd) {
			maxd = dt;
			maxFace = faceIdx;
		}

		if (dh > maxd) {
			maxd = dh;
			maxFace = faceIdx;
		}

		if (dh > 0.0f && dt > 0.0f) {
			max = 0.0f;
			min = 1.0f;
		}

		float lebesgue = dt/(dt-dh);
		if(dh > 0.0f) {
			if (lebesgue < max) {
				max = lebesgue;
			}
		} else {
			if(lebesgue > min) {
				min = lebesgue;
			}
		}
	}

	if(max < min) {
		//Disjoint
		return State::done;

		//Disjoint
		stateVarsA.faceIdx = maxFace;

		swap();
		return State::EF;
	} else {
		return State::penetrating;
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