/*
 * main.cpp
 *
 *  Created on: 2011-02-23
 *      Author: zapo
 */

#include <iostream>
#include <cstdlib>
#include "Terrain.h"
#include "FreeFlyCamera.h"
#include <sstream>


bool showWireFrame = true;

void togglePolygonMode() {

	showWireFrame = !showWireFrame;

	if(showWireFrame) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

int main(int argc, char** argv) {

	sf::RenderWindow window(sf::VideoMode(1024, 768, 32), "Test 3D");
	window.SetActive(false);

	sf::Vector2f center(window.GetWidth() / 2.f, window.GetHeight() / 2.f);

	window.SetPosition(100.f, 100.f);
	window.ShowMouseCursor(false);

	sf::View interface;

	sf::String info;
	info.SetPosition(0.f, 0.f);


	Terrain t("resources/heightmap2.png");

	t.SetTextureRepeat(false);
	t.SetPosition(0, 0);

	t.SetScale(sf::Vector3i(1, 5, 1));
	t.LoadHeightMap("resources/heightmap2.png");

	sf::Vector2f terrainCenter = t.GetCenter();

	FreeFlyCamera camera(
			sf::Vector3f(0, 1, 0),
			sf::Vector3f(terrainCenter.x, 0, terrainCenter.y)
	);

	camera.SetSpeed(100.f);
	t.SetCamera(camera);

	while(window.IsOpened()) {

		window.SetView(window.GetDefaultView());

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(70, (double)1024/678, 0.1, 5000);

		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		glClearColor(125.f/255.f, 163.f/255.f, 246.f/255.f, 0.f);

		window.SetCursorPosition(center.x, center.y);
		camera.SetMousePosition(center);

		sf::Event event;

		while(window.GetEvent(event)) {

			if(event.Type == sf::Event::Closed) {
				window.Close();
			}

			if(event.Type == sf::Event::MouseMoved) {
				camera.OnMouseMoved(event);
			}

			if(event.Type == sf::Event::KeyPressed && event.Key.Code == sf::Key::Slash) {
				togglePolygonMode();
			}
		}

		if(window.GetInput().IsKeyDown(sf::Key::Q)) {
			window.Close();
		}

		camera.Animate(window.GetFrameTime(), window.GetInput());
		camera.Look();

		t.Update();

		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		t.Render();

		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		/*window.SetView(interface);

		std::stringstream infoStr;
		int framerate = 1.f/window.GetFrameTime();

		infoStr << framerate 		<< " fps" << std::endl;
		infoStr << "x : " 			<< camera.GetPosition().x << std::endl;
		infoStr << "y : " 			<< camera.GetPosition().y << std::endl;
		infoStr << "z : " 			<< camera.GetPosition().z << std::endl;
		infoStr << "Triangles # : " << t.GetNbNodes() << std::endl;

		info.SetText(infoStr.str());


		window.Draw(info);

		if(showWireFrame) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}*/

		window.Display();
	}

	return EXIT_SUCCESS;
}
