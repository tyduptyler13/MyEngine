#include <stdexcept>
#include <GLES3/gl3.h>
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

bool GLES3Renderer::supportsVertexTextures() const override {



}

void setScissor(int x, int y, unsigned width, unsigned height) override;
void setScissorTest(bool enable = true) override;

void setClearColor(const Color&, float alpha) override;
Color getClearColor() const override;
float getClearAlpha() const override;

void clear(bool color = true, bool depth = true, bool stencil = true) override;

void clearColor() override;
void clearDepth() override;
void clearStencil() override;
void clearTarget(RenderTarget<T>& target, bool color = true, bool depth = true, bool stencil = true) override;

bool supportsFloatTextures() const override;
bool supportsStandardDerivatives() const override;
bool supportsCompressedTextureS3TC() const override;

unsigned getMaxAnisotripy() const override;

float getPixelRatio() const override;
void setPixelRatio(float) override;

std::tuple<unsigned, unsigned> getSize() const override;
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
