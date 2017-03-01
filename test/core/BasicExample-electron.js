"use strict";

var lib = require('nbind').init().lib;
var Clock = require('../../lib/Clock.js');
var electron = require('electron');

electron.app.on('ready', ()=>{

	var window = new electron.BrowserWindow({
		width: 800,
		height: 600,
	  useContentSize: true,
		resizable: false,
	  transparent: true,
		backgroundColor: "#00000000", //Alpha black ARGB
		frame: false,
		show: false
	});

	var url = require('url').format({
		protocol: 'file',
		slashes: true,
		pathname: require('path').join(__dirname, 'ui.html')
	})
	window.loadURL(url);

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
		try {
			window.setContentSize(width, height);
		} catch (e) {
			console.error(e);
		}
	});

	var overlay = new lib.GLES2OverlaySystem(renderer);

	window.webContents.beginFrameSubscription((framebuffer)=>{
		//console.log("Updating overlay");
		var size = window.getContentSize();
		if (size[0] * size[1] * 4 === framebuffer.length){
		//	console.error("Framebuffer size mismatch, must drop frame!", size[0] * size[1] * 4, framebuffer.length);
		//} else {
			overlay.updatePixelData(framebuffer, size[0], size[1]);
		}
	});

	scene.add(camera);

	var geo = new lib.BoxGeometry(10, 10, 10, 8, 4, 2);

	var mat = new lib.GLES2NormalMaterial();

	var box = new lib.Mesh(geo, mat);
	box.position = new lib.Vector3(-10, 10, 0);

	scene.add(box);

	var geo2 = new lib.SphereGeometry(5);
	var mat2 = new lib.GLES2NormalMaterial();

	var sphere = new lib.Mesh(geo2, mat2);
	sphere.position = new lib.Vector3(-10, -10, 0);

	scene.add(sphere);

	var suzane = lib.GeometryImporter.ImportAsset("suzane.obj");
	if (suzane != null){
		suzane.position = new lib.Vector3(10, 10, 0);
		scene.add(suzane);
	}

	var clock = new Clock();

	console.log("Beginning render.")

	function render() {
		var delta = clock.getDelta();
		var rot = delta * Math.PI * 0.5;
		box.rotateY(rot);
		sphere.rotateY(rot);
		sphere.rotateY(rot);
		if (suzane != null){
			suzane.rotateY(rot);
		}

		try {
			renderer.renderAsync(scene, camera, ()=>{
				if (!renderer.needsToClose()){
					render();
				} else {
					console.log("Window needs to close.");
					electron.app.quit();
				}
			});
		} catch (e) {
			console.error(e);
		}

	}

	render();
});
