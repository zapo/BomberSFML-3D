/*
 * Window.h
 *
 *  Created on: 2011-11-04
 *      Author: zapo
 */

#ifndef WINDOW_H_
#define WINDOW_H_

#include <SFML/Graphics.hpp>
#include <string>

namespace Bomber {

class Window: public sf::RenderWindow {
public:
	Window(sf::VideoMode, std::string);

	const sf::Vector3f GetCenter() const;


	virtual ~Window();
};

}


#endif /* WINDOW_H_ */
