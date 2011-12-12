/*
 * TerrainNode.h
 *
 *  Created on: 2011-03-12
 *      Author: zapo
 */

#ifndef TERRAINNODE_H_
#define TERRAINNODE_H_

#include <SFML/Graphics.hpp>
#include "Vertex3D.h"


namespace Bomber {


class Terrain;

class TerrainNode {
public:

	enum Type {
		NW = 0, NE = 1, SW = 2, SE = 3, NONE = 4
	};

	enum Border {
		BOTTOM = 0, LEFT = 1, TOP = 2, RIGHT = 3
	};


	TerrainNode(const sf::Vector2f & center, float size, Terrain & t, TerrainNode * parent, Type position);
	virtual ~TerrainNode();

	float GetSize() const {
		return size;
	}

	TerrainNode * GetNeighbor(Border border) {
		return neighbors[border];
	}

	void Disable(){
		for(unsigned int i=0; i < 9; i++) {
			vertices[i]->enabled = false;
		}
	}

	void Enable(){
		for(unsigned int i=0; i < 9; i++) {
			vertices[i]->enabled = true;
		}
	}

	void SetSize(float _size) {
		size = _size;
	}

	sf::Vector2f & GetCenter() {
		return center;
	}

	void SetCenter(const sf::Vector2f & _center) {
		center = _center;
	}

	void SetVertex(Vertex::Location loc, Vertex3D * vertex) {
		vertices[loc] = vertex;
	}

	Vertex3D * GetVertex(Vertex::Location loc) {
		return vertices[loc];
	}

	void SetChild(Type type, TerrainNode * node) {
		children[type] = node;
	}

	TerrainNode * GetChild(Type type) {
		return children[type];
	}

	sf::FloatRect & GetBoundBox() {
		return boundBox;
	}

	void SetColor(sf::Vector3f color) {
		for(unsigned int i=0; i<9; i++) {
			vertices[i]->col = color;
		}
	}

	void SetLocalNeighbors();
	void SetGlobalNeighbors();


	static const unsigned int direction_map[4][2];
	static const unsigned int inverse_direction_map[4][2];
	static const unsigned int oposite_direction[4];
	static const unsigned int local_directions[4][2];

	std::vector<TerrainNode *> GetBorderNodes(Border border);
	std::vector<TerrainNode *> GetAjacentNodes(Border border);

	inline unsigned int GetLod() const {
		return lod;
	}

	inline Type GetOpositePosition(Border border) const {
		if(position == direction_map[border][0]) {
			return (TerrainNode::Type)inverse_direction_map[border][0];
		} else {
			return (TerrainNode::Type)inverse_direction_map[border][1];
		}
	}

	inline bool IsBro(TerrainNode & node) {
		return node.parent == parent;
	}


	void Render();

	static const float MIN_SIZE;

	bool isLeaf;
	std::vector<unsigned int> indexes_ref;

	TerrainNode * neighbors[4]; // border neighbors

private :

	Type position;

	TerrainNode * parent;
	Terrain * terrain;

	sf::Vector2f center;
	float size;

	Vertex3D * vertices[9];

	TerrainNode * children[4];

	unsigned int lod;

	sf::FloatRect boundBox;



};

}

#endif /* TERRAINNODE_H_ */
