#pragma once

#include <memory>

#include "Math.hpp"
#include "Texture.hpp"


namespace MyEngine {

	struct RenderTarget {
		const Math::UUID uuid = Math::generateUUID();

		const unsigned width, height;

	protected:
		RenderTarget(unsigned width, unsigned height)
				: width(width), height(height) {}
	};
}
