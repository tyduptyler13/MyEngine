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

		template <typename T>
		class Texture : EventDispatcher<Texture<T>, std::string> {

		private:

			void loadImage();

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

			Vector2<T> offset = Vector2<T>(0,0);
			Vector2<T> repeat = Vector2<T>(1,1);

			bool generateMipmaps = true,
			    premultiplyAlpha = false,
			    flipY = true;

			short unpackAlignment = 4;

			unsigned version = 0;

			Texture(){}
			Texture(const std::string& image, short mapping = DEFAULT_MAPPING, short wrapS = ClampToEdgeWrapping, short wrapT = ClampToEdgeWrapping, short magFilter = LinearFilter, short minFilter = LinearMipMapLinearFilter, short format = RGBAFormat, short type = UnsignedByteType, short anisotropy = 1)
				: sourceFile(image), mapping(mapping), wrapS(wrapS), wrapT(wrapT), magFilter(magFilter), minFilter(minFilter), format(format), type(type), anisotropy(anisotropy){

				loadImage();

			}
			~Texture(){}
			Texture(const Texture& t);

			void copy(const Texture& t);
			void operator=(const Texture& t){
				copy(t);
			}

			void transformUv(const Vector2<T>& uv);

		};

		template <typename T>
		short Texture<T>::DEFAULT_MAPPING = UVMapping;

	}

}

#endif

