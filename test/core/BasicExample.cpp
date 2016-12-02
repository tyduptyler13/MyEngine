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

	PerspectiveCamera<float>* camera = new PerspectiveCamera<float>(90, 800.0 / 600.0);

	scene.add(camera);

	camera->position.set(20, 20, 10);
	camera->lookAt(scene.position);

	GLES2Renderer renderer(4);

	renderer.setSize(800, 600);
	renderer.onResize([&camera](int width, int height){
		camera->aspect = float(width) / float(height);
		camera->updateProjectionMatrix();
		//The renderer automatically manages its internal size. No need to set it like three.js
	});

	BoxGeometry<float>* geo = new BoxGeometry<float>(10, 10, 10, 8, 4, 2);

	IMaterial* mat = createNormalMaterial<GLES2Renderer>();

	Mesh<float>* box = new Mesh<float>(geo, mat);

	scene.add(box);

	renderer.loop([&](double delta){
		float rotation = delta * Math::PI * 0.5;
		box->rotateX(rotation);

		//Framerate is managed internally. You can tell sdl you don't want the framerate limited if you like.
		renderer.render(scene, camera);

		return false; //Don't exit early.
	});

	return 0;

}

