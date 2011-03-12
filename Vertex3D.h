/*
 * Vertex3D.h
 *
 *  Created on: 2011-03-09
 *      Author: zapo
 */

#ifndef VERTEX3D_H_
#define VERTEX3D_H_

#include <SFML/Graphics.hpp>

struct Vertex3D { // 32 bit

	sf::Vector3f pos;  //Vertex
	sf::Vector3f nor;  //Normal
	sf::Vector2f tex;  //Texcoord0
	sf::Vector3f col;  // col
	float padding[19];
	bool enabled;

};


namespace Vertex {

	enum Location {
		TOP, TOPRIGHT, RIGHT, BOTTOMRIGHT, BOTTOM, BOTTOMLEFT, LEFT, TOPLEFT, CENTER
	};
}

#endif /* VERTEX3D_H_ */
