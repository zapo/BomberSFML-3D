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
#include <algorithm>
#include "TerrainNode.h"
#include "VectorUtils.h"

namespace Bomber {

Terrain::Terrain(const std::string & texturePath) {
	maxResolution = 3.f;
	minResolution = 1.f;

	if(mainTexture.LoadFromFile(texturePath)) {
		std::cout << "texture loaded" << std::endl;


	}
	mainTexture.SetSmooth(true);
	heights = NULL;

}

Terrain::~Terrain() {

	UnloadHeightMap();
	delete root;
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

	root = new TerrainNode(sf::Vector2i(size / 2, size / 2), size - 1, *this);

	return true;
}

void Terrain::Update() {

	numbTriangles = 0;
	numbNodes = 0;

	RefineNode(*root);
}


void Terrain::Render(float framerate) {

	this->framerate = framerate;
	root->Render();
}

void Terrain::RefineNode(TerrainNode & node) {


	unsigned int size = node.GetSize();

	float distance = ( float ) utils::VectorLength(node.GetVertex(Vertex::CENTER)->pos, camera->GetPosition());

	float maxheight = 0;
	float _heights[5];

	sf::IntRect boundBox = node.GetBoundBox();

	_heights[0] = GetRelHeightAt(node.GetCenter().x	, boundBox.Top);
	_heights[1] = GetRelHeightAt(node.GetCenter().x	, boundBox.Bottom);
	_heights[2] = GetRelHeightAt(boundBox.Right	, node.GetCenter().y);
	_heights[3] = GetRelHeightAt(boundBox.Left	, node.GetCenter().y);

	float _center_height = GetRelHeightAt(node.GetCenter().x , node.GetCenter().y);

	for(int i = 0; i < 4; i++) {
		maxheight = std::max(maxheight, (float) fabs(_center_height - _heights[i]));
	}


	float f = distance / ((float) size * std::max(maxResolution, 0.1f) * std::max(maxheight ,1.f));

	if(f < 1.0f && (size / 2) >= TerrainNode::MIN_SIZE) {

		//subdivide

		node.isLeaf = false;

		for(unsigned int i=0; i <= 3; i++) {
			RefineNode(*(node.GetChild((TerrainNode::Type)i)));
			numbNodes ++;
		}

	} else {
		node.isLeaf = true;

		if(camera->IsInFrustrum(node)) {
			numbNodes ++;
			numbTriangles += 8;
		}

	}

}

}















