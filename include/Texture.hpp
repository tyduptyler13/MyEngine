#pragma once

#include <string>
#include <memory>
#include <functional>
#include <vector>

#include "Math.hpp"
#include "Constants.hpp"
#include "Vector2.hpp"


namespace MyEngine {

	class Texture {

		std::shared_ptr<std::vector<unsigned char>> fileData;

		void loadImage();

	public:

		static const MappingMode DEFAULT_MAPPING;

		Math::UUID uuid = Math::generateUUID();

		std::string name;
		std::string sourceFile;

		int format;

		MappingMode mapping = DEFAULT_MAPPING;

		WrappingMode wrapS = ClampToEdgeWrapping,
				wrapT = ClampToEdgeWrapping;

		FilterConstant magFilter = LinearFilter,
				minFilter = LinearMipMapLinearFilter;

		unsigned short anisotropy = 1;

		Vector2<float> offset = Vector2<float>(0, 0);
		Vector2<float> repeat = Vector2<float>(1, 1);

		bool generateMipmaps = true,
				premultiplyAlpha = false,
				flipY = true,
				compress = false, //Enables DXT compression
		enableRepeat = true;

		std::unique_ptr<std::function<void(Texture*)> > onUpdate = NULL;

		Texture(const std::string& image, MappingMode mapping = DEFAULT_MAPPING,
		        WrappingMode wrapS = ClampToEdgeWrapping, WrappingMode wrapT = ClampToEdgeWrapping,
		        FilterConstant magFilter = LinearFilter, FilterConstant minFilter = LinearMipMapLinearFilter,
		        short anisotropy = 1, int format = 0)
				: sourceFile(image), format(format), mapping(mapping), wrapS(wrapS), wrapT(wrapT),
				  magFilter(magFilter), minFilter(minFilter), anisotropy(anisotropy) {
			loadImage();
		}

		Texture(const Texture& t) {
			copy(t);
		}

		void copy(const Texture& t) {
			name = t.name;
			sourceFile = t.sourceFile;

			fileData = t.fileData;

			mapping = t.mapping;
			wrapS = t.wrapS;
			wrapT = t.wrapT;

			magFilter = t.magFilter;
			minFilter = t.minFilter;

			anisotropy = t.anisotropy;

			offset = t.offset;
			repeat = t.repeat;

			generateMipmaps = t.generateMipmaps;
			premultiplyAlpha = t.premultiplyAlpha;
			flipY = t.flipY;

		}

		void operator=(const Texture& t) {
			copy(t);
		}

		void transformUv(Vector2<float> uv) {
			if (mapping == UVMapping) return;

			uv *= repeat;
			uv += offset;

			if (uv.x < 0 || uv.x > 1) {
				switch (wrapS) {
					case RepeatWrapping:
						uv.x = uv.x - floor(uv.x);
						break;

					case ClampToEdgeWrapping:
						uv.x = uv.x < 0 ? 0 : 1;
						break;

					case MirroredRepeatWrapping:
						if (std::abs(fmod(floor(uv.x), 2)) == 1) {
							uv.x = ceil(uv.x) - uv.x;
						} else {
							uv.x = uv.x - floor(uv.x);
						}
				}

			}

			if (uv.y < 0 || uv.y > 1) {
				switch (wrapS) {
					case RepeatWrapping:
						uv.y = uv.y - floor(uv.y);
						break;

					case ClampToEdgeWrapping:
						uv.y = uv.y < 0 ? 0 : 1;
						break;

					case MirroredRepeatWrapping:
						if (std::abs(fmod(floor(uv.y), 2)) == 1) {
							uv.y = ceil(uv.y) - uv.y;
						} else {
							uv.y = uv.y - floor(uv.y);
						}
				}
			}

		}

		std::shared_ptr<std::vector<unsigned char>> getData() const {
			return fileData;
		}

		unsigned getDataLength() const {
			return fileData->size();
		}

	};

}



