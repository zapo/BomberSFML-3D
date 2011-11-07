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
#include "Window.h"
#include <sstream>

using namespace Bomber;


bool showWireFrame = true;

void togglePolygonMode() {

	showWireFrame = !showWireFrame;

	if(showWireFrame) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void HandleEvents(sf::RenderWindow & window, Camera & camera, Terrain & t) {
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

		if(event.Type == sf::Event::MouseWheelMoved) {
			camera.SetSpeed(camera.GetSpeed() + 10*event.MouseWheel.Delta);
		}
	}

	if(window.GetInput().IsKeyDown(sf::Key::Q)) {
		window.Close();
	}

	if(window.GetInput().IsKeyDown(sf::Key::PageUp)) {

		float max = t.GetMaxResolution();
		max += 0.1;
		t.SetMaxResolution(max);
	}

	if(window.GetInput().IsKeyDown(sf::Key::PageDown)) {
		if(t.GetMaxResolution() > 0) {
			float max = t.GetMaxResolution();
			max -= 0.1;

			t.SetMaxResolution(max);
		}
	}
}

int main(int argc, char** argv) {

	Window window(sf::VideoMode(1024, 768, 32), "BomberSFML-3D");


	sf::Vector2f center(window.GetWidth() / 2.f, window.GetHeight() / 2.f);

	window.SetPosition(100.f, 100.f);
	window.ShowMouseCursor(false);

	sf::View interface;
	sf::String info;
	info.SetPosition(0.f, 0.f);

	Terrain t("resources/heightmap2.png");

	t.SetTextureRepeat(true);
	t.SetPosition(0, 0);

	t.SetScale(sf::Vector3i(1, 0, 1));
	t.LoadHeightMap("resources/heightmap2.png");


	sf::Vector2f terrainCenter = t.GetCenter();

	FreeFlyCamera camera(
			sf::Vector3f(0, 0, 0),
			sf::Vector3f(terrainCenter.x, 0, terrainCenter.y),
			window
	);

	camera.SetSpeed(10.f);
	t.SetCamera(camera);

	window.PreserveOpenGLStates(true);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	while(window.IsOpened()) {

		window.SetCursorPosition(center.x, center.y);
		camera.SetMousePosition(center);

		window.SetView(window.GetDefaultView());

		HandleEvents(window, camera, t);

		window.SetView(window.GetDefaultView());

		glClearColor(125.f/255.f, 163.f/255.f, 246.f/255.f, 0.f);


		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		t.Update();
		t.Render(1.f / window.GetFrameTime());

		camera.Animate(window.GetFrameTime(), window.GetInput());
		camera.Look();

		glEnable(GL_DEPTH_TEST);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glColor3ub(255, 255, 255);






		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		std::stringstream infoStr;
		int framerate = 1.f/window.GetFrameTime();

		infoStr << framerate 		<< " fps" << std::endl;
		infoStr << "x : " 			<< camera.GetPosition().x << std::endl;
		infoStr << "y : " 			<< camera.GetPosition().y << std::endl;
		infoStr << "z : " 			<< camera.GetPosition().z << std::endl;
		infoStr << "Triangles # : " << t.GetNbTriangles() << std::endl;
		infoStr << "Nodes # : " << t.GetNbNodes() << std::endl;

		info.SetText(infoStr.str());

		window.Draw(info);

		if(showWireFrame) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}

		window.Display();
	}

	return EXIT_SUCCESS;
}
