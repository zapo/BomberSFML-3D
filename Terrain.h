/*
 * Terrain.h
 *
 *  Created on: 2011-02-25
 *      Author: zapo
 */

#ifndef TERRAIN_H_
#define TERRAIN_H_
#include "Model.h"

#include "Vertex3D.h"
#include "VectorUtils.h"
#include <SFML/Graphics.hpp>

namespace Bomber {

struct vector_hash : std::unary_function<sf::Vector3f, std::size_t> {
    inline std::size_t operator()(sf::Vector3f const& p) const
    {
    	std::size_t seed = 0;
		boost::hash_combine(seed, p.x);
		boost::hash_combine(seed, p.y);
		boost::hash_combine(seed, p.z);
		return seed;
    }
};

class TerrainNode;
class Camera;
class VertexBuffer;

class Terrain : public Model {
public:

	Terrain(const std::string & texturePath, Camera &);
	virtual ~Terrain();

	bool LoadHeightMap(const std::string & filename);
	bool GenMap();
	void UnloadHeightMap();

	float GetHeightAt(unsigned int x, unsigned y) const;
	void Update();
	void Render();

	sf::Uint32 & GetNbTriangles() {
		return numbTriangles;
	}

	void SetScale(const sf::Vector3f & _scale) {
		scale = _scale;
	}

	sf::Uint32 & GetNbNodes() {
		return numbNodes;
	}

	sf::Uint32 GetVisibleNodes() const {
		return numbNodes - numbCulledNodes;
	}

	Camera & GetCamera() {
		return *camera;
	}

	const sf::Vector3f & GetScale() {
		return scale;
	}

	const sf::Image & GetMainTexture() const {
		return mainTexture;
	}

	void RefineNode(TerrainNode & node);
	bool Subdivide(TerrainNode & node);


	void SetMaxResolution(float res) {
		maxResolution = res;
	}

	void SetMinResolution(float res) {
		minResolution = res;
	}

	float GetMaxResolution() const {
		return maxResolution;
	}

	float GetMinResolution() const {
		return minResolution;
	}



	std::vector<TerrainNode *> nodes;
	std::vector<TerrainNode *>::iterator nodes_it;

	static const unsigned int direction_adjacent_vertices[4][3];

	std::vector<Vertex3D *> vertices;

	TerrainNode * root;

	boost::unordered_map<sf::Vector3f, Vertex3D *, vector_hash> position_vertice;
	boost::unordered_map<sf::Vector3f, Vertex3D *, vector_hash>::iterator pv_it;


private:


	float GetRelHeightAt(unsigned int x, unsigned y) const;


	sf::Vector3f scale;
	sf::Uint8 precision;

	sf::Image heightMap;

	float * heights;
	Camera * camera;

	unsigned int size;

	float framerate;

	float maxResolution;
	float minResolution;

	unsigned int numbNodes, numbCulledNodes, numbTriangles, numbSkirts;






};

}

#endif /* TERRAIN_H_ */
