#ifndef MYUPLAY_MYENGINE_MATERIAL
#define MYUPLAY_MYENGINE_MATERIAL

#include "Constants.hpp"
#include "Math.hpp"
#include "EventDispatcher.hpp"

#include <string>
#include <vector>
#include <map>

namespace MyUPlay {

	namespace MyEngine {

		template <typename T>
		class Material : EventDispatcher<Material<T>, std::string> {

		public:

			enum MaterialType { //Must be caps to avoid name clash
				LINEBASICMATERIAL,
				LINEDASHEDMATERIAL,
				MATERIAL,
				MESHBASICMATERIAL,
				MESHDEPTHMATERIAL,
				MESHFACEMATERIAL,
				MESHLAMBERTMATERIAL,
				MESHNORMALMATERIAL,
				MESHPHONGMATERIAL,
				POINTSMATERIAL,
				RAWSHADERMATERIAL,
				SHADERMATERIAL,
				SPRITEMATERIAL
			};

			MaterialType type = MATERIAL;

			unsigned id;
			Math::UUID uuid = Math::generateUUID();

			std::map<std::string, std::map<std::string, std::string> > shaderData;
			std::vector<std::pair<std::string, std::string> > defines;

			std::string name;

			short side = FrontSide;

			float opacity = 1;
			bool transparent = false;

			short blending = NormalBlending;
			short blendSrc = SrcAlphaFactor;
			short blendDst = OneMinusSrcAlphaFactor;
			short blendEquation = AddEquation;
			short blendSrcAlpha = -1;
			short blendDstAlpha = -1;
			short blendEquationAlpha = -1;

			short depthFunc = LessEqualDepth;
			bool depthTest = false;
			bool depthWrite = true;

			bool colorWrite = true;

			short precision = -1;

			bool polygonOffset = false;
			int polygonOffsetFactor = 0;
			int polygonOffsetUnits = 0;

			short alphaTest = 0;

			float overdraw = 0;

			bool visible = true;

			bool needsUpdate = true;

			Material& copy(const Material& mat) {

				name = mat.name;

				side = mat.side;

				opacity = mat.opacity;
				transparent = mat.transparent;

				blendSrc = mat.blendSrc;
				blendDst = mat.blendDst;
				blendEquation = mat.blendEquation;
				blendSrcAlpha = mat.blendSrcAlpha;
				blendDstAlpha = mat.blendDstAlpha;
				blendEquationAlpha = mat.blendEquationAlpha;

				depthFunc = mat.depthFunc;
				depthTest = mat.depthTest;
				depthWrite = mat.depthWrite;

				precision = mat.precision;

				polygonOffset = mat.polygonOffset;
				polygonOffsetFactor = mat.polygonOffsetFactor;
				polygonOffsetUnits = mat.polygonOffsetUnits;

				alphaTest = mat.alphaTest;

				overdraw = mat.overdraw;

				visible = mat.visible;

				return *this;

			}

			Material& operator=(const Material& mat) {
				return copy(mat);
			}

			void update(){

				dispatchEvent("update", *this);

			}

			void dispose(){

				dispatchEvent("dispose", *this);

			}

		};

	}

}

#endif

