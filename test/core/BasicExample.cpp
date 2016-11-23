
#include "Scene.hpp"
#include "PerspectiveCamera.hpp"
#include "GLES2Renderer.hpp"

#include "Geometries/BoxGeometry.hpp"
#include "GLES2MaterialLib.hpp"
#include "Clock.hpp"

#include <memory>

using namespace std;
using namespace MyUPlay::MyEngine;

int main(){

	Scene<> scene;

	PerspectiveCamera<float>* camera = new PerspectiveCamera<float>(90, 800.0 / 600.0);

	scene.add(camera);

	camera->position.set(20, 20, 10);
	camera->lookAt(scene.position);

	GLES2Renderer renderer(4);

	renderer.setSize(800, 600);

	BoxGeometry<float>* geo = new BoxGeometry<float>(10, 10, 10, 8, 4, 2);

	IMaterial* mat = createNormalMaterial<GLES2Renderer>();

	Mesh<float>* box = new Mesh<float>(geo, mat);

	scene.add(box);

	Clock<> clock;

	bool quit = false;

	while (!quit){ //Closes when the window closes

		auto d = clock.getDelta();

		float rotation = Clock<>::durationToSeconds(d) * Math::PI * 0.5;
		box->rotateX(rotation);

		//Framerate is managed internally. You can tell sdl you don't want the framerate limited if you like.
		renderer.render(scene, camera);

		SDL_Event event;

		while (SDL_PollEvent(&event)){ //We can have many events. (This should speed up closing the window)
			if (event.type == SDL_QUIT){
				quit = true;
			}
		}
	}

	return 0;

}

