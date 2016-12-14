"use strict";

var lib = require('nbind').init().lib;
var Clock = require('../../lib/Clock.js');

var scene = new lib.Scene();

var renderer = new lib.GLES2Renderer(4);

var camera = new lib.PerspectiveCamera(90, 800/600, 0.1, 2000);

/**
 * Note to future implementers:
 * nbind prevents you from directly operating on native attributes!
 * For instance: camera.position.set(...) does not work!
 * Instead, it will copy camera.position into a local variable
 * and set it, then destroy it. Doens't seem to be posible to fix yet.
 */
camera.position = new lib.Vector3(0, 0, 20);
camera.lookAt(scene.position);

renderer.onResize((width, height) => {
	camera.aspect = width / height;
	camera.updateProjectionMatrix();
});

scene.add(camera);

var geo = new lib.BoxGeometry(10, 10, 10, 8, 4, 2);

var mat = lib.createGLES2NormalMaterial();

var box = new lib.Mesh(geo, mat);
box.position = new lib.Vector3(-10, 0, 0);

scene.add(box);

var suzane = lib.GeometryImporter.ImportAsset("suzane.obj");
suzane.position = new lib.Vector3(10, 0, 0);

scene.add(suzane);

var clock = new Clock();

function render() {
	var delta = clock.getDelta();
	var rot = delta * Math.PI * 0.5;
	box.rotateY(rot);
	suzane.rotateY(rot);
	renderer.render(scene, camera);
	if (!renderer.needsToClose()){
		process.nextTick(render, 1); //Delay the next render only a little.
	}
}

render();
