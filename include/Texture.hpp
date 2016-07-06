#ifndef MYUPLAY_MYENGINE_TEXTURE
#define MYUPLAY_MYENGINE_TEXTURE

#include <string>
#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <functional>

#include "Math.hpp"
#include "Constants.hpp"
#include "Vector2.hpp"

namespace MyUPlay {

	namespace MyEngine {

		class Texture {

		private:

			static int load;

			void loadImage();

		public:

			static short DEFAULT_MAPPING;

			Math::UUID uuid = Math::generateUUID();

			std::string name;
			std::string sourceFile;

			unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> texture; //vram
			unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> image; //Ram
			//TODO mipmaps

			short mapping = DEFAULT_MAPPING;

			short wrapS = ClampToEdgeWrapping,
			      wrapT = ClampToEdgeWrapping;

			short magFilter = LinearFilter,
			      minFilter = LinearMipMapLinearFilter;

			unsigned short anisotropy = 1;

			short format = RGBAFormat;
			short type = UnsignedByteType;

			Vector2<float> offset = Vector2<float>(0,0);
			Vector2<float> repeat = Vector2<float>(1,1);

			bool generateMipmaps = true,
			    premultiplyAlpha = false,
			    flipY = true;

			short unpackAlignment = 4;

			unique_ptr<function<void(Texture*)> > onUpdate = NULL;

			Texture(){}
			Texture(const std::string& image, short mapping = DEFAULT_MAPPING, short wrapS = ClampToEdgeWrapping, short wrapT = ClampToEdgeWrapping, short magFilter = LinearFilter, short minFilter = LinearMipMapLinearFilter, short format = RGBAFormat, short type = UnsignedByteType, short anisotropy = 1)
				: sourceFile(image), mapping(mapping), wrapS(wrapS), wrapT(wrapT), magFilter(magFilter), minFilter(minFilter), format(format), type(type), anisotropy(anisotropy){

				loadImage();

			}
			~Texture(){}
			Texture(const Texture& t){
				copy(t);
			}

			void copy(const Texture& t){
				name = t.name;
				sourceFile = t.sourceFile;

				image = t.image;

				mapping = t.mapping;
				wrapS = t.wrapS;
				wrapT = t.wrapT;

				magFilter = t.magFilter;
				minFilter = t.minFilter;

				anisotropy = t.anisotropy;

				format = t.format;
				type = t.type;

				offset = t.offset;
				repeat = t.repeat;

				generateMipmaps = t.generateMipmaps;
				premultiplyAlpha = t.premultiplyAlpha;
				flipY = t.flipY;

				unpackAlignment = t.unpackAlignment;

			}
			void operator=(const Texture& t){
				copy(t);
			}

			void transformUv(Vector2<float> uv){
				if (this.mapping == UVMapping) return;

				uv *= repeat;
				uv += offset;

				if (uv.x < 0 || uv.x > 1){
					switch(wrapS){
						case RepeatWrapping:
						uv.x = uv.x - floor(uv.x);
						break;

						case ClampToEdgeWrapping:
						uv.x = uv.x < 0 : ? 0 : 1;
						break;

						case MirroredRepeatWrapping:
						if (abs(fmod(floor(uv.x), 2)) == 1){
							uv.x = ceil(uv.x) - uv.x;
						} else {
							uv.x = uv.x - floor(uv.x);
						}
					}

				}

				if (uv.y < 0 || uv.y > 1){
					switch(wrapS){
						case RepeatWrapping:
						uv.y = uv.y - floor(uv.y);
						break;

						case ClampToEdgeWrapping:
						uv.y = uv.y < 0 : ? 0 : 1;
						break;

						case MirroredRepeatWrapping:
						if (abs(fmod(floor(uv.y), 2)) == 1){
							uv.y = ceil(uv.y) - uv.y;
						} else {
							uv.y = uv.y - floor(uv.y);
						}
					}
				}

			}

		};

	}

}

#endif
