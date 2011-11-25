/*
 * TerrainNode.cpp
 *
 *  Created on: 2011-03-12
 *      Author: zapo
 */
#include "Terrain.h"

#include "TerrainNode.h"
#include "Camera.h"
#include <iostream>





namespace Bomber {

const float TerrainNode::MIN_SIZE = 2;

TerrainNode::TerrainNode(const sf::Vector2f & center, float size, Terrain & t, TerrainNode * parent, Type position) :
		position(position),
		parent(parent),
		terrain(&t),
		center(center),
		size(size),
		boundBox(sf::FloatRect(
					center.x - size / 2.f, center.y + size / 2.f,
					center.x + size / 2.f, center.y - size / 2.f
		)){



	lod = (parent != NULL) ? parent->GetLod() + 1 : 1;

	// get brothers
	for(unsigned int i = 0; i < 4; i++) {

		int positions[2] = {
			inverse_direction_map[(unsigned int)(Border)i][0],
			inverse_direction_map[(unsigned int)(Border)i][1]
		};

		TerrainNode * bro = NULL;

		if(position == positions[0]) {
			bro = parent->GetChild((Type)direction_map[i][0]);
		} else if(position == positions[1]) {
			bro = parent->GetChild((Type)direction_map[i][1]);
		}


		neighbors[i] = bro;
	}

	TerrainNode * childNW = NULL;
	TerrainNode * childNE = NULL;
	TerrainNode * childSW = NULL;
	TerrainNode * childSE = NULL;

	unsigned int new_size = size / 2;
	unsigned int new_offset = new_size / 2;

	if(new_size >= MIN_SIZE) {

		childNW = new TerrainNode(sf::Vector2f(center.x + new_offset, center.y - new_offset), new_size, t, this, NW);
		childNE = new TerrainNode(sf::Vector2f(center.x + new_offset, center.y + new_offset), new_size, t, this, NE);
		childSW = new TerrainNode(sf::Vector2f(center.x - new_offset, center.y - new_offset), new_size, t, this, SW);
		childSE = new TerrainNode(sf::Vector2f(center.x - new_offset, center.y + new_offset), new_size, t, this, SE);

		sf::Vector3f red(1.f, 0.f, 0.f);
		sf::Vector3f green(0.f, 1.f, 0.f);
		sf::Vector3f blue(0.f, 0.f, 1.f);

		isLeaf = true;
	} else {
		isLeaf = false;
	}

	SetChild(NW, childNW);
	SetChild(NE, childNE);
	SetChild(SW, childSW);
	SetChild(SE, childSE);

	sf::FloatRect texCoords = terrain->GetMainTexture().GetTexCoords(sf::IntRect(boundBox.Left, boundBox.Top, boundBox.Right, boundBox.Bottom));
	sf::Vector2f texCenter(texCoords.Left + texCoords.GetWidth() / 2, texCoords.Top + texCoords.GetHeight() / 2);

	for(unsigned int i=0; i < 9; i++) {

		Vertex3D * vertex = NULL;

		sf::Vector3f pos;
		sf::Vector2f tex;

		switch((Vertex::Location)i) {

		case Vertex::CENTER:
			pos = sf::Vector3f(center.x, terrain->GetHeightAt(center.x, center.y), center.y);
			tex = sf::Vector2f(texCoords.Left + texCoords.GetWidth() / 2, texCoords.Top + texCoords.GetHeight() / 2);
			break;
		case Vertex::BOTTOMRIGHT:
			pos = sf::Vector3f(boundBox.Left, terrain->GetHeightAt(boundBox.Left, boundBox.Top), boundBox.Top);
			tex = sf::Vector2f(texCoords.Left, texCoords.Top);
			break;
		case Vertex::RIGHT:
			pos = sf::Vector3f(center.x, terrain->GetHeightAt(center.x, boundBox.Top), boundBox.Top);
			tex = sf::Vector2f(texCoords.Left + texCoords.GetWidth() / 2, texCoords.Top);
			break;
		case Vertex::TOPRIGHT:
			pos = sf::Vector3f(boundBox.Right, terrain->GetHeightAt(boundBox.Right, boundBox.Top), boundBox.Top);
			tex = sf::Vector2f(texCoords.Right, texCoords.Top);
			break;
		case Vertex::TOP:
			pos = sf::Vector3f(boundBox.Right, terrain->GetHeightAt(boundBox.Right, center.y), center.y);
			tex = sf::Vector2f(texCoords.Right, texCoords.Top + texCoords.GetHeight() / 2);
			break;
		case Vertex::TOPLEFT:
			pos = sf::Vector3f(boundBox.Right, terrain->GetHeightAt(boundBox.Right, boundBox.Bottom), boundBox.Bottom);
			tex = sf::Vector2f(texCoords.Right, texCoords.Bottom);
			break;
		case Vertex::LEFT:
			pos = sf::Vector3f(center.x, terrain->GetHeightAt(center.x, boundBox.Bottom),  boundBox.Bottom);
			tex = sf::Vector2f(texCoords.Left + texCoords.GetWidth() / 2, texCoords.Bottom);
			break;
		case Vertex::BOTTOMLEFT:
			pos = sf::Vector3f(boundBox.Left, terrain->GetHeightAt(boundBox.Left, boundBox.Bottom), boundBox.Bottom);
			tex = sf::Vector2f(texCoords.Left, texCoords.Bottom);
			break;
		case Vertex::BOTTOM:
			pos = sf::Vector3f(boundBox.Left, terrain->GetHeightAt(boundBox.Left, center.y), center.y);
			tex = sf::Vector2f(texCoords.Left, texCoords.Top + texCoords.GetHeight() / 2);
			break;
		}

		if(terrain->position_vertice.find(pos) != terrain->position_vertice.end()) {
			vertex = terrain->position_vertice[pos];
		} else {
			vertex = new Vertex3D;
			terrain->position_vertice[pos] = vertex;
			vertex->pos = pos;
			vertex->tex = tex;
			vertex->col = sf::Vector3f(1.f, 1.f, 1.f);
			vertex->alpha = 1.f;
		}

		vertex->enabled = true;
		vertices[i] = vertex;
	}


	for(unsigned int i = 0; i < 24; i+=3) {

		sf::Vector3f fnor = utils::PointsNormal(
              	vertices[Model::ordering[i]]->pos,
	                vertices[Model::ordering[i+1]]->pos,
					vertices[Model::ordering[i+2]]->pos
	    );

		vertices[Model::ordering[i+1]]->nor += fnor;
		vertices[Model::ordering[i+2]]->nor += fnor;
		vertices[Model::ordering[i]]->nor += fnor;
	}

	vertices[Vertex::TOPLEFT]->nor 		/= 2.f;
	vertices[Vertex::TOP]->nor 			/= 2.f;
	vertices[Vertex::TOPRIGHT]->nor 	/= 2.f;
	vertices[Vertex::RIGHT]->nor 		/= 2.f;
	vertices[Vertex::BOTTOMRIGHT]->nor 	/= 2.f;
	vertices[Vertex::BOTTOM]->nor 		/= 2.f;
	vertices[Vertex::BOTTOMLEFT]->nor 	/= 2.f;
	vertices[Vertex::LEFT]->nor 		/= 2.f;
	vertices[Vertex::CENTER]->nor 		/= 8.f;


	t.nodes.push_back(this);
}


TerrainNode::~TerrainNode() {

	for(unsigned int i = 0; i < 4; i++) {
		delete children[i];
	}

}

std::vector<TerrainNode *> TerrainNode::GetBorderNodes(Border border) {

	std::vector<TerrainNode *> border_nodes;

	if(!isLeaf && terrain->GetCamera().IsInFrustrum(*this)) {

		for(unsigned int i = 0; i < 2; i++) {
			Type type = (Type) direction_map[(unsigned int)border][i];

			if(GetChild(type)->isLeaf) {
				border_nodes.push_back(GetChild(type));
			} else {
				TerrainNode * child = GetChild(type);
				std::vector<TerrainNode *> temp = child->GetBorderNodes(border);
				border_nodes.insert(border_nodes.end(), temp.begin(), temp.end());
			}
		}
	}

	return border_nodes;
}

std::vector<TerrainNode *> TerrainNode::GetAjacentNodes(Border border) {

	std::vector<TerrainNode *> adjacent_nodes;
	return adjacent_nodes;

	if(parent == NULL) return adjacent_nodes; // i am root

	// if i can ask directly to my siblings
	TerrainNode * bro = neighbors[border];

	if(bro != NULL) {
		if(bro->isLeaf) {
			adjacent_nodes.push_back(bro);
		} else {
			std::vector<TerrainNode *> temp = bro->GetBorderNodes((Border)oposite_direction[border]);
			adjacent_nodes.insert(adjacent_nodes.end(), temp.begin(), temp.end());
		}
	} else {

	}

	return adjacent_nodes;

}

inline TerrainNode::Type TerrainNode::GetOpositePosition(Border border) const {
	if(position == direction_map[border][0]) {
		return (TerrainNode::Type)inverse_direction_map[border][0];
	} else {
		return (TerrainNode::Type)inverse_direction_map[border][1];
	}
}

const unsigned int TerrainNode::direction_map[4][2] = {
		{(unsigned int)SW, (unsigned int)SE}, // BOTTOM
		{(unsigned int)NW, (unsigned int)SW}, // LEFT
		{(unsigned int)NW, (unsigned int)NE}, // TOP
		{(unsigned int)NE, (unsigned int)SE}  // RIGHT
};
const unsigned int TerrainNode::inverse_direction_map[4][2] = {
		{(unsigned int)NW, (unsigned int)NE}, // BOTTOM
		{(unsigned int)NE, (unsigned int)SE}, // LEFT
		{(unsigned int)SW, (unsigned int)SE}, // TOP
		{(unsigned int)NW, (unsigned int)SW}  // RIGHT
};

const unsigned int TerrainNode::oposite_direction[4] = {
		TOP, 	// BOTTOM
		RIGHT, 	// LEFT
		BOTTOM, // TOP
		LEFT  	// RIGHT
};

}
