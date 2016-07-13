#ifndef MYUPLAY_MYENGINE_TEXTURE
#define MYUPLAY_MYENGINE_TEXTURE

#include <string>
#include <memory>
#include <functional>

#include <SOIL/SOIL.h>

#include "Math.hpp"
#include "Constants.hpp"
#include "Vector2.hpp"

namespace MyUPlay {

	namespace MyEngine {

		class Texture {

			std::shared_ptr<unsigned char*> data = NULL;

		public:

			static short DEFAULT_MAPPING;

			Math::UUID uuid = Math::generateUUID();

			std::string name;
			std::string sourceFile;

			int width, height;
			int format;

			short mapping = DEFAULT_MAPPING;

			short wrapS = ClampToEdgeWrapping,
			      wrapT = ClampToEdgeWrapping;

			short magFilter = LinearFilter,
			      minFilter = LinearMipMapLinearFilter;

			unsigned short anisotropy = 1;

			Vector2<float> offset = Vector2<float>(0,0);
			Vector2<float> repeat = Vector2<float>(1,1);

			bool generateMipmaps = true,
			    premultiplyAlpha = false,
			    flipY = true,
					compress = false; //Enables DXT compression

			short unpackAlignment = 4;

			std::unique_ptr<std::function<void(Texture*)> > onUpdate = NULL;

			Texture(const std::string& image, short mapping = DEFAULT_MAPPING,
				 short wrapS = ClampToEdgeWrapping, short wrapT = ClampToEdgeWrapping,
				 short magFilter = LinearFilter, short minFilter = LinearMipMapLinearFilter,
				 short anisotropy = 1, int forceFormat = SOIL_LOAD_AUTO)
				: sourceFile(image), mapping(mapping), wrapS(wrapS), wrapT(wrapT),
				magFilter(magFilter), minFilter(minFilter), anisotropy(anisotropy){

				sourceFile = image;
				data = std::shared_ptr<unsigned char*>(
						SOIL_load_image(image.c_str(), &width, &height, &format, forceFormat),
						std::default_delete<unsigned char[]>()); //Delete array, not single char

			}

			Texture(const Texture& t){
				copy(t);
			}

			void copy(const Texture& t){
				name = t.name;
				sourceFile = t.sourceFile;

				data = t.data;

				mapping = t.mapping;
				wrapS = t.wrapS;
				wrapT = t.wrapT;

				magFilter = t.magFilter;
				minFilter = t.minFilter;

				anisotropy = t.anisotropy;

				format = t.format;

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
				if (mapping == UVMapping) return;

				uv *= repeat;
				uv += offset;

				if (uv.x < 0 || uv.x > 1){
					switch(wrapS){
						case RepeatWrapping:
						uv.x = uv.x - floor(uv.x);
						break;

						case ClampToEdgeWrapping:
						uv.x = uv.x < 0 ? 0 : 1;
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
						uv.y = uv.y < 0 ? 0 : 1;
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

			const unsigned char * const getData() const {
				return data;
			}

			int getDataLength() const {

				return width * height * sizeof(char);
			}

		};

	}

}

#endif
