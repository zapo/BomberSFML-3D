/*
 * Terrain.h
 *
 *  Created on: 2011-02-25
 *      Author: zapo
 */

#ifndef TERRAIN_H_
#define TERRAIN_H_

#include <SFML/Graphics.hpp>
#include <list>

namespace Bomber {

class TerrainNode;
class Camera;
class VertexBuffer;

class Terrain : public sf::Drawable {
public:

	enum NodeState {
		UNKNOWN, TRUE, FALSE
	};

	Terrain(const std::string & texturePath, Camera &);
	virtual ~Terrain();

	bool LoadHeightMap(const std::string & filename);
	void UnloadHeightMap();

	sf::Uint32 GetHeightAt(unsigned int x, unsigned y) const;

	void Render(float framerate);
	void Update();

	sf::Uint32 GetNbTriangles() const {
		return numbTriangles;
	}

	void SetNbTriangles(sf::Uint32 nb) {
		numbTriangles = nb;
	}

	void SetScale(const sf::Vector3i & _scale) {
		scale = _scale;
	}

	sf::Uint32 GetNbNodes() const {
		return numbNodes;
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


	const sf::Vector3i & GetScale() {
		return scale;
	}

	const sf::Image & GetMainTexture() const {
		return mainTexture;
	}

	void RefineNode(TerrainNode & node);

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

	sf::Uint32 GetRelHeightAt(unsigned int x, unsigned y) const;

	TerrainNode * root;

	bool textureRepeat;
	sf::Vector3i scale;
	sf::Uint8 precision;
	sf::Image mainTexture;
	sf::Image heightMap;
	sf::Uint32 * heights;
	Camera * camera;



	unsigned int size;

	float framerate;

	float maxResolution;
	float minResolution;

	unsigned int numbNodes;
	unsigned int numbTriangles;

	VertexBuffer * buffer;
	sf::Clock framerateAdapterTimer;

};

}

#endif /* TERRAIN_H_ */
