#pragma once

#include "Scene.hpp"
#include "Camera.hpp"

namespace MyEngine {

	template<typename T>
	class RenderPlugin {

		~RenderPlugin() {}

		virtual void render(const Scene<T>& scene, const Camera<T>& camera, unsigned viewportwidth,
		                    unsigned viewportHeight) = 0;

	};

}
