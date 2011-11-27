/*
 * FreeFlyCamera.h
 *
 *  Created on: 2011-02-25
 *      Author: zapo
 */

#ifndef FREEFLYCAMERA_H_
#define FREEFLYCAMERA_H_

#include "Camera.h"
#include "VectorUtils.h"
#include <iostream>

namespace Bomber {

class TerrainNode;

class FreeFlyCamera : public Camera {
public:

    enum Planes {
        _TOP, _BOTTOM, _LEFT, _RIGHT, _NEAR, _FAR
    };

	struct Plane {
		sf::Vector3f normal;
		sf::Vector3f point;
	};

	FreeFlyCamera(const sf::Vector3f & position, const sf::Vector3f & target, sf::RenderWindow & win);
	virtual ~FreeFlyCamera();

	void SetUpVec(sf::Vector3f up) { this->up = up; }
	sf::Vector3f GetUpVec() { return up; }

	void Animate(float dt, const sf::Input & input);
	void CompileVectors();

	const sf::Vector3f & GetPosition() const;
	void SetPosition(const sf::Vector3f &);

	float GetSensivity() const;
	void SetSensivity(float);

	void SetTheta(double);
	double GetTheta() const;

	void SetPhi(double);
	double GetPhi() const;

	void SetPsi(double);
	double GetPsi() const;

	void SetSpeed(float);
	float GetSpeed() const;

	float GetCurrentSpeed();

	void OnMouseMoved(sf::Event & event, float frametime);

	void SetMousePosition(sf::Vector2f position);

	sf::Vector3f & GetTarget();

	bool IsInFrustrum(TerrainNode & node) const;

	void SetCameraInternals();

	void Look();

private:

	sf::Clock speedFrametime;
	sf::Vector3f lastPosition;

	double theta;
	double phi, psi;

	float sensivity;

	float currentSpeed;

	sf::Vector3f position;
	sf::Vector3f forward;
	sf::Vector3f left;

	float speed;

	sf::Vector3f up;

	sf::Vector3f target;

	sf::Vector2f mousePosition;

	unsigned int fov;
	float _near;
	float _far;
	float ratio;

	Plane planes[6];

	float Hnear, Wnear, Hfar, Wfar;
};
}
#endif /* FREEFLYCAMERA_H_ */
