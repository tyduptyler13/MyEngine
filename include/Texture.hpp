#ifndef MYUPLAY_MYENGINE_TEXTURE
#define MYUPLAY_MYENGINE_TEXTURE

#include <string>
#include <memory>
#include <SDL2/SDL.h>

#include "Math.hpp"
#include "Constants.hpp"
#include "Vector2.hpp"
#include "EventDispatcher.hpp"

namespace MyUPlay {

	namespace MyEngine {

		class Texture : EventDispatcher<Texture, std::string> {

		public:

			static short DEFAULT_MAPPING;

			Math::UUID uuid = Math::generateUUID();

			std::string name;
			std::string sourceFile;

			SDL_Texture* image = NULL;
			//TODO mipmaps

			short mapping = DEFAULT_MAPPING;

			short wrapS = ClampToEdgeWrapping,
			      wrapT = ClampToEdgeWrapping;

			short magFilter = LinearFilter,
			      minFilter = LinearMipMapLinearFilter;

			short anisotropy = 1;

			short format = RGBAFormat;
			short type = UnsignedByteType;

			Vector2<float> offset = Vector2<float>(0,0);
			Vector2<float> repeat = Vector2<float>(1,1);

			bool generateMipmaps = true,
			    premultiplyAlpha = false,
			    flipY = true;

			short unpackAlignment = 4;

			unsigned version = 0;

		};

		short Texture::DEFAULT_MAPPING = UVMapping;

	}

}

#endif

