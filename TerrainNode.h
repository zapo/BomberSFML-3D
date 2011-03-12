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

class TerrainNode {
public:

	enum Type {
		NW, NE, SW, SE
	};

	TerrainNode(const sf::Vector2i & center, unsigned int size);
	virtual ~TerrainNode();

	unsigned int GetSize() const {
		return size;
	}

	void SetSize(unsigned int _size) {
		size = _size;
	}

	sf::Vector2i & GetCenter() {
		return center;
	}

	void SetCenter(const sf::Vector2i & _center) {
		center = _center;
	}

	void SetVertex(Vertex::Location loc, const Vertex3D & vertex) {
		vertices[loc] = vertex;
	}

	Vertex3D & GetVertex(Vertex::Location loc) {
		return vertices[loc];
	}

	void SetChild(Type type, TerrainNode & node) {
		children[type] = &node;
	}

	TerrainNode & GetChild(Type type) {
		return *(children[type]);
	}

private :

	sf::Vector2i center;
	unsigned int size;

	Vertex3D vertices[9];

	TerrainNode * children[4];
};

#endif /* TERRAINNODE_H_ */
