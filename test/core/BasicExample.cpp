
#include "Scene.hpp"
#include "PerspectiveCamera.hpp"
#include "GLES2Renderer.hpp"

#include "Geometries/BoxGeometry.hpp"
#include "Materials/MeshNormalMaterial.hpp"

#include <memory>

using namespace std;
using namespace MyUPlay::MyEngine;

int main(){

	Scene<> scene;

	std::shared_ptr<PerspectiveCamera<float>> camera = make_shared<PerspectiveCamera<float>>(45, 800.0 / 600.0);

	scene.add(std::static_pointer_cast<Object3D<float>>(camera));

	camera->position.set(0, 150, 400);

	camera->lookAt(scene.position);

	GLES2Renderer renderer;

	renderer.setSize(800, 600);

	std::shared_ptr<BoxGeometry<float>> geo = make_shared<BoxGeometry<float>>(10, 10, 10, 2, 4, 8);

	std::shared_ptr<MeshNormalMaterial> mat = make_shared<MeshNormalMaterial>();

	std::shared_ptr<Mesh<float>> box = make_shared<Mesh<float>>(geo, mat);

	scene.add(box);

	bool quit = false;

	while (!quit){ //Closes when the window closes

		//TODO figure out how to handle the framerate
		renderer.render(scene, *camera);

		SDL_Event event;

		if (SDL_PollEvent(&event)){
			if (event.type == SDL_QUIT){
				quit = true;
			}
		}
	}

	return 0;

}

