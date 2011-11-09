/*
 * TerrainNode.cpp
 *
 *  Created on: 2011-03-12
 *      Author: zapo
 */

#include "TerrainNode.h"
#include "VectorUtils.h"
#include "Terrain.h"
#include "Camera.h"
#include <iostream>

namespace Bomber {

unsigned int TerrainNode::MIN_SIZE = 2;

TerrainNode::TerrainNode(const sf::Vector2i & center, unsigned int size, Terrain & t) : terrain(&t), center(center), size(size) {

	boundBox = sf::IntRect(
			center.x - size / 2, center.y - size / 2,
			center.x + size / 2, center.y + size / 2
	);

	TerrainNode * childNW = NULL;
	TerrainNode * childNE = NULL;
	TerrainNode * childSW = NULL;
	TerrainNode * childSE = NULL;

	unsigned int new_size = size / 2;
	unsigned int new_offset = new_size / 2;

	if(new_size >= MIN_SIZE) {

		childNW = new TerrainNode(sf::Vector2i(center.x - new_offset, center.y + new_offset), new_size, t);
		childNE = new TerrainNode(sf::Vector2i(center.x + new_offset, center.y + new_offset), new_size, t);
		childSW = new TerrainNode(sf::Vector2i(center.x - new_offset, center.y - new_offset), new_size, t);
		childSE = new TerrainNode(sf::Vector2i(center.x + new_offset, center.y - new_offset), new_size, t);
		isLeaf = false;

	} else {
		isLeaf = true;
	}

	SetChild(NW, childNW);
	SetChild(NE, childNE);
	SetChild(SW, childSW);
	SetChild(SE, childSE);

	sf::FloatRect texCoords = terrain->GetMainTexture().GetTexCoords(sf::IntRect(boundBox.Left, boundBox.Top, boundBox.Right + 1, boundBox.Bottom + 1));
	sf::Vector2f texCenter(texCoords.Left + texCoords.GetWidth() / 2, texCoords.Top + texCoords.GetHeight() / 2);

	for(unsigned int i=0; i < 9; i++) {

		Vertex3D * vertex = new Vertex3D;
		vertex->col = sf::Vector3f(1, 1, 1);
		vertex->enabled = true;

		switch((Vertex::Location)i) {

		case Vertex::CENTER:
			vertex->pos = sf::Vector3f(center.x, terrain->GetHeightAt(center.x, center.y), center.y);
			break;
		case Vertex::TOPLEFT:
			vertex->pos = sf::Vector3f(boundBox.Left, terrain->GetHeightAt(boundBox.Left, boundBox.Top), boundBox.Top);
			break;
		case Vertex::TOP:
			vertex->pos = sf::Vector3f(center.x, terrain->GetHeightAt(center.x, boundBox.Top), boundBox.Top);
			break;
		case Vertex::TOPRIGHT:
			vertex->pos = sf::Vector3f(boundBox.Right, terrain->GetHeightAt(boundBox.Right, boundBox.Top), boundBox.Top);
			break;
		case Vertex::RIGHT:
			vertex->pos = sf::Vector3f(boundBox.Right, terrain->GetHeightAt(boundBox.Right, center.y), center.y);
			break;
		case Vertex::BOTTOMRIGHT:
			vertex->pos = sf::Vector3f(boundBox.Right, terrain->GetHeightAt(boundBox.Right, boundBox.Bottom), boundBox.Bottom);
			break;
		case Vertex::BOTTOM:
			vertex->pos = sf::Vector3f(center.x, terrain->GetHeightAt(center.x, boundBox.Bottom),  boundBox.Bottom);
			break;
		case Vertex::BOTTOMLEFT:
			vertex->pos = sf::Vector3f(boundBox.Left, terrain->GetHeightAt(boundBox.Left, boundBox.Bottom), boundBox.Bottom);
			break;
		case Vertex::LEFT:
			vertex->pos = sf::Vector3f(boundBox.Left, terrain->GetHeightAt(boundBox.Left, center.y), center.y);
			break;
		}

		vertices[i] = vertex;
	}

	t.nodes.push_back(this);
}


TerrainNode::~TerrainNode() {

	for(unsigned int i = 0; i < 4; i++) {
		delete children[i];
	}

}

}
