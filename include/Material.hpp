#ifndef MYUPLAY_MYENGINE_MATERIAL
#define MYUPLAY_MYENGINE_MATERIAL

#include <string>
#include <memory>

namespace MyUPlay {
	namespace MyEngine {
		class IMaterial;
	}
}

#include "Constants.hpp"
#include "Math.hpp"
#include "Shader/Shader.hpp"

class MyUPlay::MyEngine::IMaterial {

public:

	static unsigned materialIdCounter;

	unsigned id = materialIdCounter++;
	Math::UUID uuid = Math::generateUUID();

	std::shared_ptr<Shader::Shader> shader; //This can be shared with other materials

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

	IMaterial(){}

	IMaterial(const IMaterial& m) {
		copy(m);
	}

	IMaterial& copy(const IMaterial& mat) {

		shader = mat.shader;

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

	IMaterial& operator=(const IMaterial& mat) {
		return copy(mat);
	}

};

#endif

