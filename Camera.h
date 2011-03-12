/*
 * Camera.h
 *
 *  Created on: 2011-03-07
 *      Author: zapo
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include <SFML/Graphics.hpp>

class Camera {
public:
	Camera();
	virtual ~Camera();
	virtual sf::Vector3f & GetPosition() = 0;
	virtual sf::Vector3f & GetTarget() = 0;
	virtual void Look() = 0;

protected:
};

#endif /* CAMERA_H_ */
