/*
 * FreeFlyCamera.cpp
 *
 *  Created on: 2011-02-25
 *      Author: zapo
 */

#include "FreeFlyCamera.h"
#include "VectorUtils.h"
#include <iostream>


FreeFlyCamera::FreeFlyCamera(const sf::Vector3f & position, const sf::Vector3f & target) {

	speed = 100.f;
	this->position = position;
	this->target = target;

	sensivity = 0.5f;

	theta 	= utils::VectorsAngle(sf::Vector2f(target.x, target.z), sf::Vector2f(position.x, position.z));
	phi		= utils::VectorsAngle(sf::Vector2f(target.z, target.y), sf::Vector2f(_UP_.x, _UP_.y));

	CompileVectors();

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

	left = utils::Normalize(utils::CrossProduct(_UP_, forward));

	target = position + forward;

}

sf::Vector3f & FreeFlyCamera::GetPosition() {
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

void FreeFlyCamera::OnMouseMoved(sf::Event & event) {

	theta 	-= (event.MouseMove.X - mousePosition.x) * sensivity;
	phi 	-= (event.MouseMove.Y - mousePosition.y) * sensivity;

	CompileVectors();
}

void FreeFlyCamera::Look() {
	gluLookAt(position.x, position.y, position.z, target.x, target.y, target.z, _UP_.x, _UP_.y, _UP_.z);
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
