"use strict";

nbind.init(Module, (err, binding) => {
	var lib = binding.lib;

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

	var mat = lib.GLES2CreateNormalMaterial();

	var box = new lib.Mesh(geo, mat);
	box.position = new lib.Vector3(-10, 0, 0);

	scene.add(box);

	//var suzane = lib.GeometryImporter.ImportAsset("suzane.obj");
	//suzane.position = new lib.Vector3(10, 0, 0);

	//scene.add(suzane);

	var clock = new Clock();

	function render() {
		var delta = clock.getDelta();
		var rot = delta * Math.PI * 0.5;
		box.rotateY(rot);
		//suzane.rotateY(rot);

		/*
		renderer.render(scene, camera);

		if (!renderer.needsToClose()){
			process.nextTick(render);
		}
		*/

		renderer.render(scene, camera);
		requestAnimationFrame(()=>{
			//console.log("Rendered Frame");
			render();
		});

	}

	requestAnimationFrame(render);

});

/**
 * @author alteredq / http://alteredqualia.com/
 * @author tyduptyler13
 * Modified for use in nodejs
 */

function Clock( autoStart ) {

	this.autoStart = ( autoStart !== undefined ) ? autoStart : true;

	this.startTime = 0;
	this.oldTime = 0;
	this.elapsedTime = 0;

	this.running = false;

}

Clock.prototype = {

	constructor: Clock,

	start: function () {

		this.startTime = Date.now();

		this.oldTime = this.startTime;
		this.elapsedTime = 0;
		this.running = true;

	},

	stop: function () {

		this.getElapsedTime();
		this.running = false;

	},

	getElapsedTime: function () {

		this.getDelta();
		return this.elapsedTime;

	},

	getDelta: function () {

		var diff = 0;

		if ( this.autoStart && ! this.running ) {

			this.start();

		}

		if ( this.running ) {

			var newTime = Date.now();

			diff = ( newTime - this.oldTime ) / 1000;
			this.oldTime = newTime;

			this.elapsedTime += diff;

		}

		return diff;

	}

};
