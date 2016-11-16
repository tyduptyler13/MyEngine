
#include "Scene.hpp"
#include "PerspectiveCamera.hpp"
#include "GLES2Renderer.hpp"

#include "Geometries/BoxGeometry.hpp"
#include "GLES2MaterialLib.hpp"

#include <memory>

using namespace std;
using namespace MyUPlay::MyEngine;

int main(){

	Scene<> scene;

	PerspectiveCamera<float>* camera = new PerspectiveCamera<float>(45, 800.0 / 600.0);

	scene.add(camera);

	camera->position.set(0, 150, 400);

	camera->lookAt(scene.position);

	GLES2Renderer renderer;

	renderer.setSize(800, 600);

	BoxGeometry<float>* geo = new BoxGeometry<float>(10, 10, 10, 2, 4, 8);

	IMaterial* mat = createNormalMaterial<GLES2Renderer>();

	Mesh<float>* box = new Mesh<float>(geo, mat);

	scene.add(box);

	bool quit = false;

	while (!quit){ //Closes when the window closes

		//TODO figure out how to handle the framerate
		renderer.render(scene, camera);

		SDL_Event event;

		if (SDL_PollEvent(&event)){
			if (event.type == SDL_QUIT){
				quit = true;
			}
		}
	}

	return 0;

}

