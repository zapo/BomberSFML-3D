/*
 * Window.cpp
 *
 *  Created on: 2011-11-04
 *      Author: zapo
 */

#include "Window.h"

namespace Bomber {

Window::Window(sf::VideoMode mode, std::string name) : sf::RenderWindow(mode, name) {
}

Window::~Window() {

}

const sf::Vector3f Window::GetCenter() const {
	return sf::Vector3f(GetWidth() / 2.f, GetHeight() / 2.f, 0);
}

}
