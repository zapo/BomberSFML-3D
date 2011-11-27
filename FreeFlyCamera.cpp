/*
 * FreeFlyCamera.cpp
 *
 *  Created on: 2011-02-25
 *      Author: zapo
 */

#include "FreeFlyCamera.h"
#include "VectorUtils.h"
#include <iostream>
#include "TerrainNode.h"

namespace Bomber {

FreeFlyCamera::FreeFlyCamera(const sf::Vector3f & position, const sf::Vector3f & target, sf::RenderWindow & win) {

	lastPosition = position;

	up = _UP_;

	fov = 80;
	_near = 0.1;
	_far = 10000;
	ratio = ((float)win.GetWidth() / (float)win.GetHeight());

	speed = 100.f;
	this->position = position;
	this->target = target;

	sensivity = 0.5f;

	theta 	= 45;//utils::VectorsAngle(sf::Vector2f(target.x, target.z), sf::Vector2f(position.x, position.z));
	phi		= -45;//utils::VectorsAngle(sf::Vector2f(target.z, target.y), sf::Vector2f(up.x, up.y));

	psi = 0;

	CompileVectors();

	float tang = (float) tan(fov * 0.5f * M_PI / 180.f);

	Hnear = tang * _near;
	Wnear = Hnear * ratio;


	Hfar = tang * _far;
	Wfar = Hfar * ratio;
}

FreeFlyCamera::~FreeFlyCamera() {
}

void FreeFlyCamera::Animate(float dt, const sf::Input & input) {



	if(input.IsKeyDown(sf::Key::Left)) {
		position += speed * left * dt;
	}

	if(input.IsKeyDown(sf::Key::Right)) {
		position += speed * -left * dt;
	}

	if(input.IsKeyDown(sf::Key::Up)) {
		position += speed * forward * dt;
	}

	if(input.IsKeyDown(sf::Key::Down)) {
		position += speed * -forward * dt;
	}

	CompileVectors();
	SetCameraInternals();

}

void FreeFlyCamera::SetCameraInternals() {

		sf::Vector3f Z = utils::Normalize(position - target);

		sf::Vector3f X = utils::Normalize(utils::CrossProduct(Z, up));

		sf::Vector3f Y = utils::Normalize(utils::CrossProduct(Z, X));

		sf::Vector3f nc = position - Z * _near;
		sf::Vector3f fc = position - Z * _far;

		planes[_NEAR].normal = -Z;
		planes[_NEAR].point = nc;

		planes[_FAR].normal = Z;
		planes[_FAR].point = fc;

		sf::Vector3f aux, normal;

		aux = (nc + (Y * Wnear)) - position;
		aux = utils::Normalize(aux);
		normal = utils::CrossProduct(aux, X);
		planes[_TOP].normal = normal;
		planes[_TOP].point = nc + (Y* Hnear);

		aux = (nc - (Y* Hnear)) - position;
		aux = utils::Normalize(aux);
		normal = utils::CrossProduct(X, aux);
		planes[_BOTTOM].normal = normal;
		planes[_BOTTOM].point = nc - (Y* Hnear);

		aux = (nc - X * Wnear) - position;
		aux = utils::Normalize(aux);
		normal = utils::CrossProduct(aux, Y);
		planes[_LEFT].normal = normal;
		planes[_LEFT].point = nc - (X * Wnear);

		aux = (nc + (X * Wnear)) - position;
		aux = utils::Normalize(aux);
		normal = utils::CrossProduct(Y, aux);
		planes[_RIGHT].normal = normal;
		planes[_RIGHT].point = nc + (X * Wnear);
}


void FreeFlyCamera::CompileVectors() {

	if(phi > 89) {
		phi = 89;
	} else if(phi < -89) {
		phi = -89;
	}

	float r_temp = cos(phi * M_PI / 180.f);

	forward.y = sin(phi * M_PI / 180.f);
	forward.x = r_temp * sin(theta * M_PI / 180.f);
	forward.z = r_temp * cos(theta * M_PI / 180.f);


	left = utils::Normalize(utils::CrossProduct(up, forward));

	target = position + forward;

}

void FreeFlyCamera::SetPsi(double psi) {

	this->psi = psi;

	float psi_rad = psi * M_PI / 180.f;
	up = (float)cos(psi_rad) * up + (1 - (float)cos(psi_rad)) * (utils::DotProduct(up, forward)) * forward + ((float)sin(psi_rad)) * (utils::CrossProduct(forward, up));

	this->psi = 0;
}
double FreeFlyCamera::GetPsi() const {
	return this->psi;
}

const sf::Vector3f & FreeFlyCamera::GetPosition() const {
	return position;
}
void FreeFlyCamera::SetPosition(const sf::Vector3f & position) {
	this->position = position;
}

float FreeFlyCamera::GetSensivity() const {
	return sensivity;
}
void FreeFlyCamera::SetSensivity(float sensivity) {
	this->sensivity = sensivity;
}

void FreeFlyCamera::SetTheta(double theta) {
	this->theta = theta;
}
double FreeFlyCamera::GetTheta() const {
	return theta;
}

void FreeFlyCamera::SetPhi(double phi) {
	this->phi = phi;
}
double FreeFlyCamera::GetPhi() const {
	return phi;
}

void FreeFlyCamera::OnMouseMoved(sf::Event & event, float frametime) {

	theta 	-= (event.MouseMove.X - mousePosition.x) * sensivity * frametime;
	phi 	-= (event.MouseMove.Y - mousePosition.y) * sensivity * frametime;

	CompileVectors();
	SetCameraInternals();
}

void FreeFlyCamera::Look() {

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, (float)ratio, _near, _far);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(position.x, position.y, position.z, target.x, target.y, target.z, up.x, up.y, up.z);
}

void FreeFlyCamera::SetMousePosition(sf::Vector2f position) {
	mousePosition = position;
}


void FreeFlyCamera::SetSpeed(float speed) {
	this->speed = speed;
}
float FreeFlyCamera::GetSpeed() const {
	return speed;
}

sf::Vector3f & FreeFlyCamera::GetTarget() {
	return target;
}


bool FreeFlyCamera::IsInFrustrum(TerrainNode & node) const {

	int out;

	for(int i = 0; i < 6; i++) {
		// reset counters for corners in and out
		out = 0;

		for (int k = 1; k <= 7; k+=2) {

			float d = -(utils::DotProduct(planes[i].normal, planes[i].point));

			if (d + utils::DotProduct(planes[i].normal, node.GetVertex((Vertex::Location)k)->pos) < 0) {
				out++;
			}
		}
		if(out == 4) {
			return false;
		}
	}
	return true;
}

float FreeFlyCamera::GetCurrentSpeed() {

	if(speedFrametime.GetElapsedTime() >= 0.5f) {

		float distance = utils::VectorLength(position, lastPosition);

		currentSpeed = distance / speedFrametime.GetElapsedTime();
		lastPosition = position;

		speedFrametime.Reset();

	}

	return currentSpeed;
}

}



