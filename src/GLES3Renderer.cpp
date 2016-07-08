#include <stdexcept>
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include <SDL2/SDL_opengles.h>

#include "Log.hpp"
#include "GLES3Renderer.hpp"

using namespace std;
using namespace MyUPlay::MyEngine;

Log log("GLES3Renderer");

GLES3Renderer::GLES3Renderer(){
	SDL_Init(SDL_INIT_VIDEO);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	window = SDL_CreateWindow("MyEngine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_OPENGL);

	if (!window){
		log.warn("Failed to create sdl window!");
		throw new runtime_error("Failed to create sdl window!");
	}

	context = SDL_GL_CreateContext(window);

	if (!context){
		log.warn("Failed to create a gl context!");
		throw new runtime_error("Failed to create a gl context!");
	}

	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextures);

}

GLES3Renderer::~GLES3Renderer(){

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);

}

void GLES3Renderer::setScissor(int x, int y, unsigned width, unsigned height) override {
	glScissor(x, y, width, height);
}
void GLES3Renderer::setScissorTest(bool enable = true) override {
	if (enable){
		glEnable(GL_SCISSOR_TEST);
	} else {
		glDisable(GL_SCISSOR_TEST);
	}
}

void GLES3Renderer::setClearColor(const Color& c, float alpha) override {
	glClearColor(c.r, c.g, c.b, alpha);
}
Color GLES3Renderer::getClearColor() const override {
	GLfloat bkColor[4];
	glGetFloatv(GL_COLOR_CLEAR_VALUE, bkColor);
	Color c;
	c.setRGB(bkColor[0], bkColor[1], bkColor[2]);
	return color;
}
float GLES3Renderer::getClearAlpha() const override {
	GLfloat bkColor[4];
	glGetFloatv(GL_COLOR_CLEAR_VALUE, bkColor);
	return bkColor[3];
}

void GLES3Renderer::GLclear(bool color = true, bool depth = true, bool stencil = true) override {
	GLbitfield mask;
	mask |= color ? GL_COLOR_BUFFER_BIT : 0;
	mask |= depth ? GL_DEPTH_BUFFER_BIT : 0;
	mask |= stencil ? GL_STENCIL_BUFFER_BIT : 0;
	glClear(mask);
}

void GLES3Renderer::clearColor() override {
	glClear(GL_COLOR_BUFFER_BIT);
}
void GLES3Renderer::clearDepth() override {
	glClear(GL_DEPTH_BUFFER_BIT);
}
void GLES3Renderer::clearStencil() override {
	glClear(GL_STENCIL_BUFFER_BIT);
}
void GLES3Renderer::clearTarget(RenderTarget<T>& target, bool color = true, bool depth = true, bool stencil = true) override {
	log.warn("TODO: Implement clearTarget");
}

unsigned GLES3Renderer::getMaxAnisotripy() const override {
	GLint i;
	glGetIntegerv(GL_TEXTURE_MAX_ANISOTROPY_EXT, &i);
	return i;
}

std::tuple<unsigned, unsigned> GLES3Renderer::getSize() const override {
	
}
void setSize(unsigned width, unsigned height) override;

void setViewport(int x, int y, unsigned width, unsigned height) override;
std::tuple<int, int, unsigned, unsigned> getViewport() const override;

void renderBufferImmediate(const Object3D<T>& object, const ShaderProgram& program, const Material<T>& material) override;
void renderBufferDirect(const Camera<T>& camera, const std::vector<Light<T> >& lights, const Fog<T>& fog, const Material<T>& material, const Object3D<T>& object, const Object3D<T>& objectGroup) override;

void render(const Scene<T>& scene, const Camera<T>& camera, RenderTarget<T>* renderTarget = NULL, bool forceClear = false) override;

void setFaceCulling(short cullFace, short frontFaceDirection) override;
void setTexture(const Texture<T>& texture, unsigned slot override);
void setRenderTarget(RenderTarget<T>& target) override;
void RenderTarget<T>& getRenderTarget() override;
void readRenderTargetPixels(RenderTarget<T>& target, int x, int y, unsigned width, unsigned height, void** buffer) override; //TODO Find type for buffer
