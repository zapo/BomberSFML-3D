/*
 * main.cpp
 *
 *  Created on: 2011-02-23
 *      Author: zapo
 */

#include <GL/glew.h>
#include <iostream>
#include "Terrain.h"
#include "Light.h"
#include "FreeFlyCamera.h"
#include "Window.h"
#include <sstream>



using namespace Bomber;




bool showWireFrame = true;
bool capturePointer = true;

void togglePolygonMode() {

	showWireFrame = !showWireFrame;

	if(showWireFrame) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void HandleEvents(sf::RenderWindow & window, Camera & camera, Terrain & t, Light & light,  float frametime) {
	sf::Event event;

	while(window.GetEvent(event)) {

		if(event.Type == sf::Event::Closed) {
			window.Close();
		}

		if(event.Type == sf::Event::MouseMoved && capturePointer) {
			camera.OnMouseMoved(event, frametime );
		}

		if(event.Type == sf::Event::KeyPressed && event.Key.Code == sf::Key::Slash) {
			togglePolygonMode();
		}

		if(event.Type == sf::Event::MouseWheelMoved) {
			camera.SetSpeed(camera.GetSpeed() + 50 *event.MouseWheel.Delta * frametime);
		}

		if(event.Type == sf::Event::LostFocus) {
			capturePointer = false;
		}
		if(event.Type == sf::Event::GainedFocus) {
			capturePointer = true;
		}
	}


	if(window.GetInput().IsKeyDown(sf::Key::Q)) {
		window.Close();
	}

	if(window.GetInput().IsKeyDown(sf::Key::L)) {
		camera.SetPsi(camera.GetPsi() - 1 * frametime * 10);
	}

	if(window.GetInput().IsKeyDown(sf::Key::M)) {
		camera.SetPsi(camera.GetPsi() + 1 * frametime * 10);
	}


	if(window.GetInput().IsKeyDown(sf::Key::J)) {
		light.GetPosition().y -= 100 * frametime;
	}

	if(window.GetInput().IsKeyDown(sf::Key::K)) {
		light.GetPosition().y += 100 * frametime;
	}

	if(window.GetInput().IsKeyDown(sf::Key::PageUp)) {

		float max = t.GetMaxResolution();
		max += (0.1 * frametime);
		t.SetMaxResolution(max);
	}

	if(window.GetInput().IsKeyDown(sf::Key::PageDown)) {
		if(t.GetMaxResolution() > 0) {
			float max = t.GetMaxResolution();
			max -= (0.1 * frametime);

			t.SetMaxResolution(max);
		}
	}

	t.Update();

}

int main(int argc, char** argv) {

	sf::WindowSettings Settings;

	Settings.DepthBits         = 24;
	Settings.StencilBits       = 8;
	Settings.AntialiasingLevel = 4;

	sf::RenderWindow window(sf::VideoMode(1024, 768, 32), "BomberSFML-3D", sf::Style::Resize, Settings);



	sf::Vector2f center(window.GetWidth() / 2.f, window.GetHeight() / 2.f);

	//window.SetPosition(100.f, 100.f);

	sf::View interface;
	sf::String info;

	info.SetPosition(0.f, 0.f);

	FreeFlyCamera camera(
		sf::Vector3f(0, 200, 0),
		sf::Vector3f(0, 0, 0),
		window
	);

	camera.SetSpeed(10.f);

	Terrain t("text.bmp", camera);

	t.SetTextureRepeat(true);

	if (!t.LoadHeightMap("heights.bmp")) {
		return EXIT_FAILURE;
	}

	Light sun = Light(Light::POSITIONAL, sf::Vector3f(500, 500, 500));


	glClearColor(125.f/255.f, 163.f/255.f, 246.f/255.f, 1.f);


	glEnable(GL_DEPTH_TEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);




	while(window.IsOpened()) {

		int framerate = 1.f/window.GetFrameTime();

		window.ShowMouseCursor(!capturePointer);

		if(capturePointer) {
			window.SetCursorPosition(center.x, center.y);
			camera.SetMousePosition(center);
		}

		window.SetView(window.GetDefaultView());

		HandleEvents(window, camera, t, sun,  window.GetFrameTime());


		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		camera.Animate(window.GetFrameTime(), window.GetInput());
		camera.Look();

		if(showWireFrame) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		} else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		//sun.Render();
		t.Render();

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glLoadIdentity();

		std::stringstream infoStr;

		infoStr << framerate 		<< " FPS" << std::endl;
		infoStr << "x : " 			<< camera.GetPosition().x << std::endl;
		infoStr << "y : " 			<< camera.GetPosition().y << std::endl;
		infoStr << "z : " 			<< camera.GetPosition().z << std::endl;
		infoStr << "Triangles # : " << t.GetNbTriangles() << std::endl;
		infoStr << "Nodes # : " << t.GetNbNodes() << std::endl;

		info.SetText(infoStr.str());
		info.SetColor(sf::Color::White);



		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDisable(GL_LIGHTING);

		window.Draw(info);

		glDisable(GL_TEXTURE_2D);


		window.Display();
	}

	return EXIT_SUCCESS;
}
