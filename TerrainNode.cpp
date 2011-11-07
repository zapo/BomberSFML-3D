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

	Vertex3D * vcenter = new Vertex3D;
	vcenter->pos = sf::Vector3f(center.x, terrain->GetHeightAt(center.x, center.y), center.y);
	vcenter->enabled = true;
	vcenter->tex = sf::Vector2f(texCenter.x, texCenter.y);

	vertices[Vertex::CENTER] = vcenter;

	Vertex3D * vtopleft = new Vertex3D;
	vtopleft->pos = sf::Vector3f(boundBox.Left, terrain->GetHeightAt(boundBox.Left, boundBox.Top), boundBox.Top);
	vtopleft->enabled = true;
	vtopleft->tex = sf::Vector2f(texCoords.Left, texCoords.Top);

	vertices[Vertex::TOPLEFT] = vtopleft;

	Vertex3D * vtop = new Vertex3D;
	vtop->pos = sf::Vector3f(center.x, terrain->GetHeightAt(center.x, boundBox.Top), boundBox.Top);
	vtop->enabled = true;
	vtop->tex = sf::Vector2f(texCenter.x, texCoords.Top);

	vertices[Vertex::TOP] = vtop;

	Vertex3D * vtopright = new Vertex3D;
	vtopright->pos = sf::Vector3f(boundBox.Right, terrain->GetHeightAt(boundBox.Right, boundBox.Top), boundBox.Top);
	vtopright->enabled = true;
	vtopright->tex = sf::Vector2f(texCoords.Right, texCoords.Top);

	vertices[Vertex::TOPRIGHT] = vtopright;

	Vertex3D * vright = new Vertex3D;
	vright->pos = sf::Vector3f(boundBox.Right, terrain->GetHeightAt(boundBox.Right, center.y), center.y);
	vright->enabled = true;
	vright->tex = sf::Vector2f(texCoords.Right, texCenter.y);

	vertices[Vertex::RIGHT] = vright;

	Vertex3D * vbottomright = new Vertex3D;
	vbottomright->pos = sf::Vector3f(boundBox.Right, terrain->GetHeightAt(boundBox.Right, boundBox.Bottom), boundBox.Bottom);
	vbottomright->enabled = true;
	vbottomright->tex = sf::Vector2f(texCoords.Right, texCoords.Bottom);

	vertices[Vertex::BOTTOMRIGHT] = vbottomright;

	Vertex3D * vbottom = new Vertex3D;
	vbottom->pos = sf::Vector3f(center.x, terrain->GetHeightAt(center.x, boundBox.Bottom),  boundBox.Bottom);
	vbottom->enabled = true;
	vbottom->tex = sf::Vector2f(texCenter.x, texCoords.Bottom);

	vertices[Vertex::BOTTOM] = vbottom;

	Vertex3D * vbottomleft = new Vertex3D;
	vbottomleft->pos = sf::Vector3f(boundBox.Left, terrain->GetHeightAt(boundBox.Left, boundBox.Bottom), boundBox.Bottom);
	vbottomleft->enabled = true;
	vbottomleft->tex = sf::Vector2f(texCoords.Left, texCoords.Bottom);

	vertices[Vertex::BOTTOMLEFT] = vbottomleft;

	Vertex3D * left = new Vertex3D;
	left->pos = sf::Vector3f(boundBox.Left, terrain->GetHeightAt(boundBox.Left, center.y), center.y);
	left->enabled = true;
	left->tex = sf::Vector2f(texCoords.Left, texCenter.y);

	vertices[Vertex::LEFT] = left;


	t.nodes.push_back(this);
}

void TerrainNode::Render() {

	if(!terrain->GetCamera().IsInFrustrum(*this)) {
		return;
	}

	if(isLeaf) {
		glBegin(GL_TRIANGLE_FAN);

		Vertex3D * firstEnabled = NULL;

		for(int i=0; i < 9; i++) {
			if(vertices[i]->enabled) {

				if(firstEnabled == NULL && i != Vertex::CENTER) {
					firstEnabled = vertices[i];
				}

				//glTexCoord2f(vertices[i].tex.x, vertices[i].tex.y);
				glVertex3f(vertices[i]->pos.x, vertices[i]->pos.y, vertices[i]->pos.z );
			}
		}

		if(firstEnabled != NULL) {
			//glTexCoord2f(firstEnabled->tex.x, firstEnabled->tex.y);
			glVertex3f(firstEnabled->pos.x, firstEnabled->pos.y, firstEnabled->pos.z);
		}

		glEnd();
	} else {
		for(unsigned int i=0; i <= 3; i++) {
			children[i]->Render();
		}
	}

}

TerrainNode::~TerrainNode() {

	for(unsigned int i = 0; i < 4; i++) {
		delete children[i];
	}

}

}
