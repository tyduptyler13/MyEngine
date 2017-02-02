#ifndef MYUPLAY_MYENGINE_GLES2MATERIALLIB
#define MYUPLAY_MYENGINE_GLES2MATERIALLIB

#include "Material.hpp"

#include <memory>

namespace MyUPlay {
	namespace MyEngine {

		/**
		 * Creates colors based on the objects normal direction.
		 */
		[[deprecated("Replaced by GLES2NormalMaterial class.")]]
		std::shared_ptr<IMaterial> GLES2CreateNormalMaterial();

		//The following were never actually implemented or available

		/**
		 * Creates a material where the object is one solid color.
		 */
		//std::shared_ptr<IMaterial> GLES2CreateBasicMaterial();

		/**
		 * Creates a material with a diffuse shading model on
		 * either a solid color or textured.
		 */
		//std::shared_ptr<IMaterial> GLES2CreateDiffuseMaterial();

		/**
		 * Creates a material with phong (glossy) shading
		 * with either a solid color or textured.
		 */
		//std::shared_ptr<IMaterial> GLES2CreatePhongMaterial();

		/**
		 * Creates a depth material. Useful for testing depth.
		 */
		//std::shared_ptr<IMaterial> GLES2CreateDepthMaterial();

	}
}

#endif
