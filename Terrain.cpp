/*
 * Terrain.cpp
 *
 *  Created on: 2011-02-25
 *      Author: zapo
 */


#define BUFFER_OFFSET(i) ((char*) NULL + (i))

#include "Terrain.h"
#include "Camera.h"
#include <iostream>
#include "VectorUtils.h"
#include <algorithm>
#include "TerrainNode.h"

Terrain::Terrain(const std::string & texturePath) {
	maxResolution = 9.f;
	minResolution = 3.f;

	mainTexture.LoadFromFile(texturePath);
	mainTexture.SetSmooth(true);
	heights = NULL;

	mainTexture.SetSmooth(true);
}

Terrain::~Terrain() {

	UnloadHeightMap();
}

void Terrain::UnloadHeightMap() {
	delete [] heights;
}



sf::Uint32 Terrain::GetRelHeightAt(unsigned int x, unsigned y) const {

	if(heightMap.GetHeight() > 0) {

		return heights[x + heightMap.GetHeight() * y];

	}else {
		return 0;
	}

}

sf::Uint32 Terrain::GetHeightAt(unsigned int x, unsigned z) const {
	sf::Uint32 height = GetRelHeightAt(x / scale.x, z / scale.z);

	return height * scale.y;
}

void Terrain::SetScale(const sf::Vector3i & _scale) {
	scale = _scale;
}

sf::Uint32 Terrain::GetNbNodes() const {
	return numbNodes;
}

void Terrain::SetTextureRepeat(bool repeat) {
	textureRepeat = repeat;
}

void Terrain::SetCamera(Camera & camera) {
	this->camera = &camera;
}
Camera & Terrain::GetCamera() {
	return *camera;
}


bool Terrain::LoadHeightMap(const std::string & filename) {

	// Check if our map has a good format and is loadable

	if(!heightMap.LoadFromFile(filename)
			|| heightMap.GetWidth() != heightMap.GetHeight()) {
		return false;
	}

	// Check if our map width is of type 2^n + 1

	float n = log(heightMap.GetWidth() - 1) / log(2);
	int n_temp = n;


	// n is not integer, not good !
	if(static_cast<float>(n_temp) != n) {
		return false;
	}

	size = heightMap.GetWidth();

	heights = new sf::Uint32[size * size];

	const sf::Uint8 * pixel = heightMap.GetPixelsPtr();

	for(unsigned int i = 0; i < (size * size * 4); i+=4) {

		heights[i/4] = pixel[i] * 100 / 255;
	}

	SetCenter(sf::Vector2f((GetPosition().x + size) / 2, (GetPosition().y + size) / 2));

	quadMatrix = new NodeState * [size];

	for(unsigned int i = 0; i < size; i++) {

		quadMatrix[i] = new NodeState [size];

		for(unsigned int j = 0; j < size; j++) {

			quadMatrix[i][j] = UNKNOWN;

		}
	}

	root = new TerrainNode(sf::Vector2i(size / 2, size / 2), size - 1);

	return true;
}

void Terrain::Render() {

	RenderNode(*root);

}

void Terrain::Update() {

	numbNodes = 1;

	RefineNode(*root);
}

void Terrain::RefineNode(TerrainNode & node) {

	sf::Vector2i center = node.GetCenter();
	unsigned int size 	= node.GetSize();

	float distance = ( float ) utils::VectorLength(sf::Vector3f(center.x, GetHeightAt(center.x, center.y), center.y), camera->GetPosition());

	unsigned int offset = (size / 2);

	float maxheight = 0;

	float _heights[5];

	_heights[0] = GetRelHeightAt(center.x - offset	, center.y + offset);
	_heights[1] = GetRelHeightAt(center.x + offset	, center.y + offset);
	_heights[2] = GetRelHeightAt(center.x - offset	, center.y - offset);
	_heights[3] = GetRelHeightAt(center.x + offset	, center.y - offset);
	_heights[3] = GetRelHeightAt(center.x			, center.y);

	for(int i = 0; i < 5; i++) {

		maxheight = std::max(maxheight, _heights[i]);
	}

	float f = distance /( (float) size * 8 * std::max(maxheight / 10, 1.0f ));

	if(f < 1.0f && size >= 2) {

		//subdivide

		quadMatrix[center.x][center.y] = TRUE;

		unsigned int new_size = size / 2;
		unsigned int new_offset = new_size / 2;

		numbNodes += 4;

		TerrainNode * childNW = new TerrainNode(sf::Vector2i(center.x - new_offset, center.y + new_offset), new_size);
		TerrainNode * childNE = new TerrainNode(sf::Vector2i(center.x + new_offset, center.y + new_offset), new_size);
		TerrainNode * childSW = new TerrainNode(sf::Vector2i(center.x - new_offset, center.y - new_offset), new_size);
		TerrainNode * childSE = new TerrainNode(sf::Vector2i(center.x + new_offset, center.y - new_offset), new_size);

		node.SetChild(TerrainNode::NW, *childNW);
		node.SetChild(TerrainNode::NE, *childNE);
		node.SetChild(TerrainNode::SW, *childSW);
		node.SetChild(TerrainNode::SE, *childSE);

		RefineNode(*childNW);
		RefineNode(*childNE);
		RefineNode(*childSW);
		RefineNode(*childSE);

	} else {
		quadMatrix[center.x][center.y] = FALSE;
	}

}

void Terrain::RenderNode(TerrainNode & node) {

	sf::Vector2i center = node.GetCenter();
	unsigned int size = node.GetSize();

	unsigned int offset = size / 2;

	glColor3ub(255, 0, 0);

	glBegin(GL_TRIANGLE_FAN);

	glVertex3f(center.x, GetHeightAt(center.x, center.y), center.y);


	// left

	glVertex3f(center.x - offset, GetHeightAt(center.x - offset	, center.y), center.y );

	// top left

	glVertex3f(center.x - offset, GetHeightAt(center.x - offset	, center.y + offset), center.y + offset);

	// top

	glVertex3f(center.x, GetHeightAt(center.x, center.y + offset), center.y + offset);

	// top right

	glVertex3f(center.x + offset, GetHeightAt(center.x + offset	, center.y + offset), center.y + offset);

	// right

	glVertex3f(center.x + offset, GetHeightAt(center.x + offset	, center.y), center.y);

	// bottom right

	glVertex3f(center.x + offset, GetHeightAt(center.x + offset	, center.y - offset), center.y - offset);

	// bottom

	glVertex3f(center.x, GetHeightAt(center.x, center.y - offset), center.y - offset);

	// bottom left

	glVertex3f(center.x - offset, GetHeightAt(center.x - offset	, center.y - offset), center.y - offset);

	// left

	glVertex3f(center.x - offset, GetHeightAt(center.x - offset	, center.y), center.y );


	glEnd();


	if(quadMatrix[center.x][center.y] == TRUE) {

		RenderNode(node.GetChild(TerrainNode::NW));
		RenderNode(node.GetChild(TerrainNode::NE));
		RenderNode(node.GetChild(TerrainNode::SW));
		RenderNode(node.GetChild(TerrainNode::SE));

	}


}














