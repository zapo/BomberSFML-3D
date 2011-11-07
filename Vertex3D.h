/*
 * Vertex3D.h
 *
 *  Created on: 2011-03-09
 *      Author: zapo
 */

#ifndef VERTEX3D_H_
#define VERTEX3D_H_

#include <SFML/Graphics.hpp>

namespace Bomber {

typedef struct Vertex3D { // 32 bit

	sf::Vector3f pos;  //Vertex
	sf::Vector3f nor;  //Normal
	sf::Vector2f tex;  //Texcoord0
	sf::Vector3f col;  // col
	float padding[19];
	bool enabled;

} Vertex3D;


namespace Vertex {

	enum Location {
		 CENTER = 0, TOPLEFT = 1, TOP = 2, TOPRIGHT = 3, RIGHT = 4, BOTTOMRIGHT = 5, BOTTOM = 6, BOTTOMLEFT = 7, LEFT = 8
	};
}

}

#endif /* VERTEX3D_H_ */
