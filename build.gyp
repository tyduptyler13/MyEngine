{
	"targets": [
		{
			"target_name": "MyEngine",
			"product_name": "MyEngine",
			"type": "<(library)",
			"sources": [
				"src/Log.cpp",
				"src/AmbientLight.cpp",
				"src/Curve.cpp",
				"src/Gyroscope.cpp",
				"src/Matrix3.cpp",
				"src/PointCloud.cpp",
				"src/Spline.cpp",
				"src/Animation.cpp",
				"src/CurvePath.cpp",
				"src/HemisphereLight.cpp",
				"src/Matrix4.cpp",
				"src/PointLight.cpp",
				"src/SpotLight.cpp",
				"src/AnimationHandler.cpp",
				"src/DataTexture.cpp",
				"src/KeyFrameAnimation.cpp",
				"src/Mesh.cpp",
				"src/Quanternion.cpp",
				"src/Sprite.cpp",
				"src/AnimationMorphTarget.cpp",
				"src/DirctionalLight.cpp",
				"src/Light.cpp",
				"src/MorphAnimMesh.cpp",
				"src/Ray.cpp",
				"src/Texture.cpp",
				"src/AreaLight.cpp",
				"src/Euler.cpp",
				"src/Object3D.cpp",
				"src/RayTraceRenderer.cpp",
				"src/Triangle.cpp",
				"src/Bone.cpp",
				"src/Loader.cpp",
				"src/OpenGLRenderer.cpp",
				"src/Renderer.cpp",
				"src/Box3.cpp",
				"src/LOD.cpp",
				"src/Scene.cpp",
				"src/Vector3.cpp",
				"src/FoxExp2.cpp",
				"src/Path.cpp",
				"src/Shape.cpp",
				"src/Vector4.cpp",
				"src/Geometry.cpp",
				"src/PerspectiveCamera.cpp",
				"src/SkinnedMesh.cpp",
				"src/CompressedTexture.cpp",
				"src/GLES2Renderer.cpp",
				"src/Material.cpp",
				"src/Math.cpp"
			],
			"include_dirs": [
				"include"
			],
			"direct_dependent_settings": {
				"include_dirs": [ "include/" ]
			},
			"cflags": [
				"-Wall",
				"-Wextra",
				"-pedantic",
				"-std=c++11"
			]
		}
	]
}

