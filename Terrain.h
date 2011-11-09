/*
 * Terrain.h
 *
 *  Created on: 2011-02-25
 *      Author: zapo
 */

#ifndef TERRAIN_H_
#define TERRAIN_H_
#include "Vertex3D.h"
#include <SFML/Graphics.hpp>
#include <list>

namespace Bomber {

class TerrainNode;
class Camera;
class VertexBuffer;

class Terrain : public sf::Drawable {
public:

	Terrain(const std::string & texturePath, Camera &);
	virtual ~Terrain();

	bool LoadHeightMap(const std::string & filename);
	bool GenMap();
	void UnloadHeightMap();

	float GetHeightAt(unsigned int x, unsigned y) const;

	void Render(float framerate);
	void Update();

	sf::Uint32 GetNbTriangles() const {
		return numbTriangles;
	}

	void SetNbTriangles(sf::Uint32 nb) {
		numbTriangles = nb;
	}

	void SetScale(const sf::Vector3f & _scale) {
		scale = _scale;
	}

	sf::Uint32 GetNbNodes() const {
		return numbNodes;
	}

	sf::Uint32 GetVisibleNodes() const {
		return numbNodes - numbCulledNodes;
	}

	void SetNbNodes(sf::Uint32 nb) {
		numbNodes = nb;
	}

	void SetTextureRepeat(bool repeat) {
		textureRepeat = repeat;
	}

	void SetCamera(Camera & camera) {
		this->camera = &camera;
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
	void Render(sf::RenderTarget &) const {

	}

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

	std::vector<TerrainNode*> nodes;

private:

	std::vector<Vertex3D> vertices;
	std::vector<Vertex3D>::iterator v_it;

	std::vector<GLuint> indexes;
	std::vector<GLuint>::iterator i_it;

	static const unsigned int ordering[24];

	float GetRelHeightAt(unsigned int x, unsigned y) const;

	TerrainNode * root;

	bool textureRepeat;
	sf::Vector3f scale;
	sf::Uint8 precision;
	sf::Image mainTexture;
	sf::Image heightMap;
	float * heights;
	Camera * camera;



	unsigned int size;

	float framerate;

	float maxResolution;
	float minResolution;

	unsigned int numbNodes, numbCulledNodes, numbTriangles;

	VertexBuffer * buffer;
	sf::Clock framerateAdapterTimer;

};

}

#endif /* TERRAIN_H_ */
