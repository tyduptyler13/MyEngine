
#include "Scene.hpp"
#include "PerspectiveCamera.hpp"
#include "GLES2Renderer.hpp"

#include "Geometries/BoxGeometry.hpp"
#include "GLES2MaterialLib.hpp"
#include "Color.hpp"

#include <memory>

using namespace std;
using namespace MyUPlay::MyEngine;

int main(){

	Scene<> scene;

	PerspectiveCamera<float>* camera = new PerspectiveCamera<float>(90, 800.0 / 600.0);

	scene.add(camera);

	GLES2Renderer renderer;

	renderer.setSize(800, 600);

	BoxGeometry<float>* geo = new BoxGeometry<float>(10, 10, 10);

	IMaterial* mat = createNormalMaterial<GLES2Renderer>();

	mat->side = DoubleSide;

	Mesh<float>* box = new Mesh<float>(geo, mat);

	box->frustumCulled = false; //TODO fix culling bug

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

