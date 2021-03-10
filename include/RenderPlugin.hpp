#pragma once

#include "Scene.hpp"
#include "Camera.hpp"

namespace MyEngine {

	template<typename T>
	struct RenderPlugin {
		virtual void render(const IRenderer<T>& renderer, const Scene<T>& scene, const Camera<T>& camera) = 0;
	};

}
