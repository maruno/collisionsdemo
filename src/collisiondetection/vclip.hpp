#pragma once

#include <utility>
#include <type_traits>
#include <vector>

#include "modelloader/vertexbuffer.hpp"
#include "renderer/scene/sceneitem.hpp"

namespace collisiondetection {
	class VClip {
	 private:
		enum class State {done, penetrating, VV, VE, VF, EE, EF};
		struct StateVariables {
			unsigned int faceIdx, vertexIdx, edgeToVertexIdx;

			bool operator==(const StateVariables& other) {
				return (other.faceIdx == faceIdx) && (other.vertexIdx == vertexIdx) && (other.edgeToVertexIdx == edgeToVertexIdx);
			}
		};

		template<typename T, typename = std::enable_if<!std::is_same<typename std::iterator_traits<T>::value_type, void>::value>>
		struct ClipResult {
			float lebesgueBegin = 0.0f;
			float lebesgueEnd = 1.0f;
			T N1;
			T N2;
			bool intersection;
		};

		StateVariables stateVarsA, stateVarsB;
		State lastState;

		std::pair<unsigned int, unsigned int> previousEdgeEdgeA, previousEdgeEdgeB;

		scene::SceneItem* itemA;
		scene::SceneItem* itemB;

		std::vector<glm::vec3> verticesA;
		std::vector<glm::vec3> normalsA;
		std::vector<glm::uvec3> facesA;

		std::vector<glm::vec3> verticesB;
		std::vector<glm::vec3> normalsB;
		std::vector<glm::uvec3> facesB;

		State VV();
		State VE();
		State VF();
		State EE();
		State EF();

		State EELocalMinEscape();

		template<typename T>
		float ds(const glm::vec3& v, const glm::vec3& planeNormal, T reference);

		template<typename T>
		ClipResult<T> clipEdge(const glm::vec3& t, const glm::vec3& h, T firstNeighbour, T pastLastNeightbour, const std::vector<glm::vec3>& planeNormals, float lebesgueContinueBegin = 0.0f, float lebesgueContinueEnd = 1.0f, unsigned int* reference = nullptr);
		State postClipDerivativeCheckVertex(ClipResult<std::vector<std::pair<unsigned int, unsigned int>>::iterator>& clipResult, std::vector<std::pair<unsigned int, unsigned int>>::iterator end, const glm::vec3& v,
						   const glm::vec3& t, const glm::vec3& h);
		State postClipDerivativeCheckVertexEdge(ClipResult<std::vector<unsigned int>::iterator>& clipResult, std::vector<unsigned int>::iterator end, const glm::vec3& t, const glm::vec3& h);
		State postClipDerivativeCheckFaceEdge(ClipResult<std::vector<unsigned long>::iterator>& clipResult, std::vector<unsigned long>::iterator end, const glm::vec3& t, const glm::vec3& h);

		bool swapped;
		inline void swap();

		glm::vec3 transformVerticeA(unsigned int idx);
		glm::vec3 transformVerticeB(unsigned int idx);

		inline std::vector<glm::vec3> voronoiPlanesForEdges(std::vector<std::pair<unsigned int, unsigned int>> edges, unsigned int reference);
		inline std::vector<glm::vec3> voronoiPlanesForEdgeFace(std::vector<std::pair<unsigned int, unsigned int>> edges, unsigned int faceIdx);
		inline std::vector<glm::vec3> voronoiPlanesForEdgeFace(std::pair<unsigned int, unsigned int> edge, std::vector<unsigned long> faces);
		inline std::vector<glm::vec3> voronoiPlanesForFaces(std::vector<unsigned int>& faces);
		inline bool violatesVP(const glm::vec3& planeNormal, const glm::vec3& vectorToPoint);
		
		inline std::vector<unsigned int> facesContainingVertex(unsigned int vertexIdx);
		inline std::vector<unsigned long> facesContainingEdge(unsigned int vertexIdx, unsigned int edgeToVertexIdx);

		inline std::vector<std::pair<unsigned int, unsigned int>> neighboursForVertex(unsigned int vertexIdx);
		inline std::vector<std::pair<unsigned int, unsigned int>> neighboursForVertex(unsigned int vertexIdx, const glm::uvec3& face);
		inline std::vector<std::pair<unsigned int, unsigned int>> neighboursForFace(unsigned int faceIdx);
	public:
		VClip(scene::SceneItem* myA, scene::SceneItem* myB);
		
		bool run();

		glm::vec3 getPenetratingLocation();
	};
}

#ifndef	VCLIP_TEMPLATES
#define VCLIP_TEMPLATES
#include "vclip.cpp"
#endif