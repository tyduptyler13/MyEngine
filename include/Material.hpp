#pragma once

#include <string>
#include <memory>


namespace MyEngine {
	namespace Shader {
		struct Shader;
	}
	struct IMaterial;
}


#include "Constants.hpp"
#include "Math.hpp"

struct MyEngine::IMaterial {

	static unsigned materialIdCounter;

	unsigned id = materialIdCounter++;
	Math::UUID uuid = Math::generateUUID();

	std::shared_ptr<Shader::Shader> shader; //This can be shared with other materials

	std::string name;

	BlendingMode blending = NormalBlending;
	SideConstant side = FrontSide;
	ShadeConstant shading = SmoothShading;
	ColorConstant vertexColors = NoColors;

	float opacity = 1;
	bool transparent = false;

	BlendingFunc blendSrc = SrcAlphaFactor;
	BlendingFunc blendDst = OneMinusSrcAlphaFactor;
	BlendingEquation blendEquation = AddEquation;
	BlendingFunc blendSrcAlpha = NoBlendingFunc;
	BlendingFunc blendDstAlpha = NoBlendingFunc;
	BlendingEquation blendEquationAlpha = NoBlendingEq;

	DepthMode depthFunc = LessEqualDepth;
	bool depthTest = true;
	bool depthWrite = true;

	bool colorWrite = true;

	//Unused for now.
	//short precision = -1;

	bool polygonOffset = false;
	int polygonOffsetFactor = 0;
	int polygonOffsetUnits = 0;

	short alphaTest = 0;

	//Unused in current renderers
	//float overdraw = 0;

	bool visible = true;

	bool needsUpdate = true;

	bool wireframe = false;

	bool fog = false,
			lights = false,
			morphTargets = false;

	virtual ~IMaterial() {}

	IMaterial() {}

	IMaterial(std::string name) : name(name) {}

	IMaterial(const IMaterial& m) {
		copy(m);
	}

	IMaterial& copy(const IMaterial& mat);

	IMaterial& operator=(const IMaterial& mat) {
		return copy(mat);
	}

};

