#include <stdexcept>
#include <SDL2/SDL_opengles2.h>

#include <SOIL/SOIL.h>

#include "Log.hpp"
#include "GLES2Renderer.hpp"
#include "Vector4.hpp"
#include "Shader/Shader.hpp"
#include "Frustum.hpp"
#include "RenderTarget.hpp"


using namespace std;
using namespace MyUPlay::MyEngine;

static Log logger("GLES2Renderer");

GLES2Renderer::GLES2Renderer(unsigned antialias) {
	SDL_Init(SDL_INIT_VIDEO);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	if (antialias != 0){
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, antialias);
	}

	window = SDL_CreateWindow("MyEngine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_OPENGL);

	if (!window){
		logger.warn("Failed to create sdl window!");
		throw runtime_error("Failed to create sdl window!");
	}

	context = SDL_GL_CreateContext(window);

	if (!context){
		logger.warn("Failed to create a gl context!");
		throw runtime_error("Failed to create a gl context!");
	}

	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextures);

}

GLES2Renderer::~GLES2Renderer(){
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
}

void GLES2Renderer::setScissor(int x, int y, unsigned width, unsigned height) {
	glScissor(x, y, width, height);
}
void GLES2Renderer::setScissorTest(bool enable)  {
	if (enable){
		glEnable(GL_SCISSOR_TEST);
	} else {
		glDisable(GL_SCISSOR_TEST);
	}
}

void GLES2Renderer::setClearColor(const Color& c, float alpha)  {
	glClearColor(c.r, c.g, c.b, alpha);
}
Color GLES2Renderer::getClearColor() const  {
	GLfloat bkColor[4];
	glGetFloatv(GL_COLOR_CLEAR_VALUE, bkColor);
	Color c;
	c.setRGB(bkColor[0], bkColor[1], bkColor[2]);
	return c;
}
float GLES2Renderer::getClearAlpha() const  {
	GLfloat bkColor[4];
	glGetFloatv(GL_COLOR_CLEAR_VALUE, bkColor);
	return bkColor[3];
}

void GLES2Renderer::clear(bool color, bool depth, bool stencil)  {
	GLbitfield mask;
	mask |= color ? GL_COLOR_BUFFER_BIT : 0;
	mask |= depth ? GL_DEPTH_BUFFER_BIT : 0;
	mask |= stencil ? GL_STENCIL_BUFFER_BIT : 0;
	glClear(mask);
}

void GLES2Renderer::clearColor()  {
	glClear(GL_COLOR_BUFFER_BIT);
}
void GLES2Renderer::clearDepth()  {
	glClear(GL_DEPTH_BUFFER_BIT);
}
void GLES2Renderer::clearStencil()  {
	glClear(GL_STENCIL_BUFFER_BIT);
}
void GLES2Renderer::clearTarget(std::shared_ptr<RenderTarget<GLES2Renderer>> target, bool color, bool depth, bool stencil)  {
	logger.warn("TODO: Implement clearTarget");
}

unsigned GLES2Renderer::getMaxAnisotripy() const  {
	GLint i;
	glGetIntegerv(GL_TEXTURE_MAX_ANISOTROPY_EXT, &i);
	return i;
}

void GLES2Renderer::setViewport(int x, int y, unsigned width, unsigned height)  {
	glViewport(x, y, width, height);
}
std::tuple<int, int, unsigned, unsigned> GLES2Renderer::getViewport() const  {
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	return make_tuple(viewport[0], viewport[1], viewport[2], viewport[3]);
}
void GLES2Renderer::setDefaultViewport() {
	int w, h;
	SDL_GL_GetDrawableSize(window, &w, &h);
	setViewport(0, 0, w, h);
}

void GLES2Renderer::renderBufferImmediate(Object3D<float>& object, std::shared_ptr<Shader::Shader> program, Material<float>& material)  {



}

void GLES2Renderer::renderBufferDirect(Camera<float>& camera, std::vector<Light<float> >& lights,
		Fog<float>& fog, Material<float>& material, Object3D<float>& object, Object3D<float>& objectGroup)  {

}

void GLES2Renderer::render(Scene<float>& scene, Camera<float>& camera, std::shared_ptr<RenderTarget<GLES2Renderer>> renderTarget, bool forceClear)  {

	if (scene.autoUpdate) scene.updateMatrixWorld();

	if (camera.parent.expired()) camera.updateMatrixWorld();

	camera.matrixWorldInverse.getInverse(camera.matrixWorld);

	Matrix4f projScreenMatrix = camera.projectionMatrix * camera.matrixWorldInverse;
	Frustum<float> frustum;
	frustum.setFromMatrix(projScreenMatrix);

}

void GLES2Renderer::setFaceCulling(CullConstant cullFace, CullDirection frontFaceDirection)  {

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
		default:
			break;
		}

	}

	if (frontFaceDirection == FrontFaceDirectionCW){
		glFrontFace(GL_CW);
	} else {
		glFrontFace(GL_CCW);
	}

}

void GLES2Renderer::setTexture(shared_ptr<Texture> texture, unsigned slot)  {

	auto pt = textures.find(texture->uuid);

	if (pt == textures.end()){
		//We need to add the texture to the gpu and create a new object for it.
		GPUTexture gt;
		gt.texture = texture;

		int channels = texture->format;
		unsigned flags = 0;
		flags |= SOIL_FLAG_POWER_OF_TWO; //Always force textures to a power of two
		flags |= texture->generateMipmaps ? SOIL_FLAG_MIPMAPS : 0;
		flags |= texture->enableRepeat ? SOIL_FLAG_TEXTURE_REPEATS : 0;
		flags |= texture->premultiplyAlpha ? SOIL_FLAG_MULTIPLY_ALPHA : 0;
		flags |= texture->flipY ? SOIL_FLAG_INVERT_Y : 0;
		flags |= texture->compress ? SOIL_FLAG_COMPRESS_TO_DXT : 0;

		gt.id = SOIL_load_OGL_texture_from_memory(texture->getData()->data(), texture->getDataLength(), channels, SOIL_CREATE_NEW_ID, flags);

		textures.insert(make_pair(texture->uuid, gt));
	}

	GPUTexture& t = textures[texture->uuid];

	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, t.id);

}

void GLES2Renderer::setRenderTarget(std::shared_ptr<RenderTarget<GLES2Renderer>> target) {

}
std::shared_ptr<RenderTarget<GLES2Renderer>> GLES2Renderer::getRenderTarget() {

}
void GLES2Renderer::readRenderTargetPixels(std::shared_ptr<RenderTarget<GLES2Renderer>> target, int x, int y, unsigned width, unsigned height, void* buffer) {

}

//Specializations for shaders (Allows a renderer to work)
template <> const char* Shader::Utility<GLES2Renderer, bool>::type = "bool";
template <> const char* Shader::Utility<GLES2Renderer, int>::type = "int";
template <> const char* Shader::Utility<GLES2Renderer, unsigned>::type = "uint";
template <> const char* Shader::Utility<GLES2Renderer, float>::type = "float";
template <> const char* Shader::Utility<GLES2Renderer, Vector2<float>>::type = "vec2";
template <> const char* Shader::Utility<GLES2Renderer, Vector3<float>>::type = "vec3";
template <> const char* Shader::Utility<GLES2Renderer, Vector4<float>>::type = "vec4";
template <> const char* Shader::Utility<GLES2Renderer, Vector2<int>>::type = "ivec2";
template <> const char* Shader::Utility<GLES2Renderer, Vector3<int>>::type = "ivec3";
template <> const char* Shader::Utility<GLES2Renderer, Vector4<int>>::type = "ivec3";
template <> const char* Shader::Utility<GLES2Renderer, Vector2<unsigned>>::type = "uvec2";
template <> const char* Shader::Utility<GLES2Renderer, Vector3<unsigned>>::type = "uvec3";
template <> const char* Shader::Utility<GLES2Renderer, Vector4<unsigned>>::type = "uvec4";
template <> const char* Shader::Utility<GLES2Renderer, Matrix3<float>>::type = "mat3";
template <> const char* Shader::Utility<GLES2Renderer, Matrix4<float>>::type = "mat4";
template <> const char* Shader::Utility<GLES2Renderer, Texture>::type = "sampler2D";
//Other specializations left out for now.

RenderTarget<GLES2Renderer>::RenderTarget(unsigned width, unsigned height, bool depthBuffer, bool stencilBuffer)
: width(width), height(height), depthBuffer(depthBuffer), stencilBuffer(stencilBuffer)
{
	glGenFramebuffers(1, &id);
}

RenderTarget<GLES2Renderer>::~RenderTarget(){
	glDeleteFramebuffers(1, &id);
}

