#include "Material.hpp"
#include "Shader/Shader.hpp"

using namespace MyUPlay::MyEngine;

unsigned IMaterial::materialIdCounter = 0;

IMaterial& IMaterial::copy(const IMaterial& mat) {

	shader = mat.shader;

	name = mat.name;

	lights = mat.lights;

	blending = mat.blending;
	side = mat.side;
	shading = mat.shading;
	vertexColors = mat.vertexColors;

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

	colorWrite = mat.colorWrite;

	//precision = mat.precision;

	polygonOffset = mat.polygonOffset;
	polygonOffsetFactor = mat.polygonOffsetFactor;
	polygonOffsetUnits = mat.polygonOffsetUnits;

	alphaTest = mat.alphaTest;

	//overdraw = mat.overdraw;

	visible = mat.visible;

	return *this;

}
