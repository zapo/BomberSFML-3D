/*
 * FreeFlyCamera.h
 *
 *  Created on: 2011-02-25
 *      Author: zapo
 */

#ifndef FREEFLYCAMERA_H_
#define FREEFLYCAMERA_H_

#include "Camera.h"

class FreeFlyCamera : public Camera {
public:
	FreeFlyCamera(const sf::Vector3f & position, const sf::Vector3f & target);
	virtual ~FreeFlyCamera();

	void Animate(float dt, const sf::Input & input);
	void CompileVectors();

	sf::Vector3f & GetPosition();
	void SetPosition(const sf::Vector3f &);

	float GetSensivity() const;
	void SetSensivity(float);

	void SetTheta(double);
	double GetTheta() const;

	void SetPhi(double);
	double GetPhi() const;

	void SetSpeed(float);
	float GetSpeed() const;

	void OnMouseMoved(sf::Event & event);

	void SetMousePosition(sf::Vector2f position);

	sf::Vector3f & GetTarget();

	void Look();

private:
	double theta;
	double phi;

	float sensivity;

	sf::Vector3f position;
	sf::Vector3f forward;
	sf::Vector3f left;

	float speed;

	sf::Vector3f target;

	sf::Vector2f mousePosition;
};

#endif /* FREEFLYCAMERA_H_ */
