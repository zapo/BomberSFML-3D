/*
 * Terrain.h
 *
 *  Created on: 2011-02-25
 *      Author: zapo
 */

#ifndef TERRAIN_H_
#define TERRAIN_H_

#include "Vertex3D.h"
#include <boost/shared_ptr.hpp>
#include <SFML/Graphics.hpp>
#include <list>

class TerrainNode;
class Camera;


class Terrain : public sf::Drawable {
public:

	enum NodeState {
		UNKNOWN, TRUE, FALSE
	};

	Terrain(const std::string & texturePath);
	virtual ~Terrain();

	bool LoadHeightMap(const std::string & filename);
	void UnloadHeightMap();

	sf::Uint32 GetHeightAt(unsigned int x, unsigned y) const;

	void Render();
	void Update();
	sf::Uint32 GetNbNodes() const;

	void SetTextureRepeat(bool);

	void SetCamera(Camera & camera);
	Camera & GetCamera();

	void SetScale(const sf::Vector3i &);

	void RefineNode(TerrainNode & node);
	void RenderNode(TerrainNode & node);

	void Render(sf::RenderTarget &) const {

	}

private:

	sf::Uint32 GetRelHeightAt(unsigned int x, unsigned y) const;

	NodeState ** quadMatrix;

	TerrainNode * root;

	bool textureRepeat;
	sf::Vector3i scale;
	sf::Uint8 precision;
	sf::Image mainTexture;
	sf::Image heightMap;
	sf::Uint32 * heights;
	Camera * camera;

	GLuint vbo;
	GLuint ibo;
	unsigned int size;

	float maxResolution;
	float minResolution;

	unsigned int numbNodes;

	std::list<Vertex3D> vertices;

};

#endif /* TERRAIN_H_ */
