/*
 * Camera.h
 *
 *  Created on: 2011-03-07
 *      Author: zapo
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include <SFML/Graphics.hpp>

namespace Bomber {
class TerrainNode;
class Camera {
public:
	Camera();
	virtual ~Camera();
	virtual const sf::Vector3f & GetPosition() const = 0;
	virtual sf::Vector3f & GetTarget() = 0;
	virtual void Look() = 0;

	virtual void OnMouseMoved(sf::Event &) = 0;
	virtual void SetSpeed(float) = 0;
	virtual float GetSpeed() const = 0;

	virtual bool IsInFrustrum(TerrainNode & node) const = 0;

	virtual float GetSensivity() const = 0;
	virtual float GetCurrentSpeed() = 0;


protected:
};

}

#endif /* CAMERA_H_ */
