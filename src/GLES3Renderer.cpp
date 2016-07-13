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

void GLES3Renderer::setScissor(int x, int y, unsigned width, unsigned height) {
	glScissor(x, y, width, height);
}
void GLES3Renderer::setScissorTest(bool enable = true)  {
	if (enable){
		glEnable(GL_SCISSOR_TEST);
	} else {
		glDisable(GL_SCISSOR_TEST);
	}
}

void GLES3Renderer::setClearColor(const Color& c, float alpha)  {
	glClearColor(c.r, c.g, c.b, alpha);
}
Color GLES3Renderer::getClearColor() const  {
	GLfloat bkColor[4];
	glGetFloatv(GL_COLOR_CLEAR_VALUE, bkColor);
	Color c;
	c.setRGB(bkColor[0], bkColor[1], bkColor[2]);
	return c;
}
float GLES3Renderer::getClearAlpha() const  {
	GLfloat bkColor[4];
	glGetFloatv(GL_COLOR_CLEAR_VALUE, bkColor);
	return bkColor[3];
}

void GLES3Renderer::clear(bool color = true, bool depth = true, bool stencil = true)  {
	GLbitfield mask;
	mask |= color ? GL_COLOR_BUFFER_BIT : 0;
	mask |= depth ? GL_DEPTH_BUFFER_BIT : 0;
	mask |= stencil ? GL_STENCIL_BUFFER_BIT : 0;
	glClear(mask);
}

void GLES3Renderer::clearColor()  {
	glClear(GL_COLOR_BUFFER_BIT);
}
void GLES3Renderer::clearDepth()  {
	glClear(GL_DEPTH_BUFFER_BIT);
}
void GLES3Renderer::clearStencil()  {
	glClear(GL_STENCIL_BUFFER_BIT);
}
void GLES3Renderer::clearTarget(RenderTarget& target, bool color = true, bool depth = true, bool stencil = true)  {
	log.warn("TODO: Implement clearTarget");
}

unsigned GLES3Renderer::getMaxAnisotripy() const  {
	GLint i;
	glGetIntegerv(GL_TEXTURE_MAX_ANISOTROPY_EXT, &i);
	return i;
}

void GLES3Renderer::setViewport(int x, int y, unsigned width, unsigned height)  {
	glViewport(x, y, width, height);
}
std::tuple<int, int, unsigned, unsigned> GLES3Renderer::getViewport() const  {
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, &viewport);
	return make_tuple(viewport[0], viewport[1], viewport[2], viewport[3]);
}
void GLES3Renderer::setDefaultViewport() {
	int w, h;
	SDL_GL_GetDrawableSize(window, &w, &h);
	setViewport(0, 0, w, h);
}

void GLES3Renderer::renderBufferImmediate(const Object3D<float>& object, const Shader& program, const Material<float>& material)  {

}
void GLES3Renderer::renderBufferDirect(const Camera<float>& camera, const std::vector<Light<float> >& lights,
	 const Fog<float>& fog, const Material<float>& material, const Object3D<float>& object, const Object3D<float>& objectGroup)  {

}

void GLES3Renderer::render(const Scene<float>& scene, const Camera<float>& camera, RenderTarget* renderTarget, bool forceClear)  {

}

void GLES3Renderer::setFaceCulling(CullConstant cullFace, CullDirection frontFaceDirection)  {

	if (cullFace == CullFaceNone) {
		glDisable(GL_CULL_FACE);
	} else {
		glEnable(GL_CULL_FACE);

		switch(cullFace){
			case CullFaceBack:
			glCullFace(GL_BACK);
			break;
			case CullFaceFront:
			glCullFace(GL_FRONT);
			break;
			case CullFaceFrontBack:
			glCullFace(GL_FRONT_AND_BACK);
		}

	}

	if (frontFaceDirection == FrontFaceDirectionCW){
		glFrontFace(GL_CW);
	} else {
		glFrontFace(GL_CCW);
	}

}

void GLES3Renderer::setTexture(const Texture& texture, unsigned slot)  {

	auto pt = textures.find(texture.uuid);

	if (pt == textures.end()){
		//We need to add the texture to the gpu and create a new object for it.
		GPUTexture gt;
		gt.texture = texture;

		int channels = texture.format;
		unsigned flags = 0;
		flags |= SOIL_FLAG_POWER_OF_TWO; //Always force textures to a power of two
		flags |= texture.generateMipmaps ? SOIL_FLAG_MIPMAPS : 0;
		flags |= texture.repeat ? SOIL_FLAG_TEXTURE_REPEATS : 0;
		flags |= texture.premultiplyAlpha ? SOIL_FLAG_MULTIPLY_ALPHA : 0;
		flags |= texture.flipY ? SOIL_FLAG_INVERT_Y : 0;
		flags |= texture.compress ? SOIL_FLAG_COMPRESS_TO_DXT : 0;

		gt.id = SOIL_load_OGL_texture_from_memory(texture.getData(), texture.getDataLength(), channels, SOIL_CREATE_NEW_ID, flags);

		textures.insert(make_pair(texture.uuid, gt));
	}

	GPUTexture& t = textures[texture.uuid];

	glBindTexture(GL_TEXTURE_2D, slot);

}

void GLES3Renderer::setRenderTarget(RenderTarget& target) {

}
RenderTarget& GLES3Renderer::getRenderTarget() {

}
void GLES3Renderer::readRenderTargetPixels(RenderTarget& target, int x, int y, unsigned width, unsigned height, void** buffer) {

}
