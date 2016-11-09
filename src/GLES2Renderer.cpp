#include <stdexcept>
#include <algorithm>

#include <SDL2/SDL_opengles2.h>

#include <SOIL/SOIL.h>

#include "Log.hpp"
#include "GLES2Renderer.hpp"
#include "Vector4.hpp"
#include "Shader/Shader.hpp"
#include "Frustum.hpp"

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

	this->window = SDL_CreateWindow("MyEngine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_OPENGL);

	if (!this->window){
		logger.warn("Failed to create sdl window!");
		throw runtime_error("Failed to create sdl window!");
	}

	context = SDL_GL_CreateContext(this->window);

	if (!context){
		logger.warn("Failed to create a gl context!");
		throw runtime_error("Failed to create a gl context!");
	}

	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextures);

}

GLES2Renderer::~GLES2Renderer(){
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(this->window);
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
	GLbitfield mask = 0;
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
void GLES2Renderer::clearTarget(std::shared_ptr<IRenderTarget> target, bool color, bool depth, bool stencil) {
	target->bind();
	clear(color, depth, stencil);
	glBindFramebuffer(GL_FRAMEBUFFER, 0); //Unbind
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

void GLES2Renderer::renderBufferImmediate(Object3D<float>* object, std::shared_ptr<Shader::Shader> program, IMaterial* material)  {



}

void GLES2Renderer::renderBufferDirect(Camera<float>* camera, std::vector<Light<float>*>& lights,
		Fog<float>& fog, IMaterial* material, Object3D<float>* object, Object3D<float>* objectGroup) {

}

float GLES2Renderer::stablePainterSort(const RenderItem<Mesh<float>>& a, const RenderItem<Mesh<float>>& b){
	if (a.object->renderOrder != b.object->renderOrder){
		return a.object->renderOrder - b.object->renderOrder;
	} else if (a.object->material->id != b.object->material->id) {
		return a.object->material->id - b.object->material->id;
	} else if (a.z != b.z) {
		return a.z - b.z;
	} else {
		return a.object->id - b.object->id;
	}
}

float GLES2Renderer::reverseStablePainterSort(const RenderItem<Mesh<float>>& a, const RenderItem<Mesh<float>>& b){
	if (a.object->renderOrder != b.object->renderOrder){
		return a.object->renderOrder - b.object->renderOrder;
	} else if (a.z != b.z) {
		return b.z - a.z;
	} else {
		return a.object->id - b.object->id;
	}
}

void GLES2Renderer::render(Scene<float>& scene, Camera<float>* camera, std::shared_ptr<IRenderTarget> renderTarget, bool forceClear)  {

	if (scene.autoUpdate) scene.updateMatrixWorld();

	if (camera->parent == nullptr) camera->updateMatrixWorld();

	camera->matrixWorldInverse.getInverse(camera->matrixWorld);

	projScreenMatrix = camera->projectionMatrix * camera->matrixWorldInverse;

	frustum.setFromMatrix(projScreenMatrix);

	if (renderTarget != nullptr){
		renderTarget->bind();
	}

	projectObject(&scene, camera);

	if (sortObjects){
		std::stable_sort(opaqueObjects.begin(), opaqueObjects.end(), stablePainterSort);
		std::stable_sort(transparentObjects.begin(), transparentObjects.end(), reverseStablePainterSort);
	}

	//TODO Shadowmap goes here

	info.renderer.calls = 0;
	info.renderer.vertices = 0;
	info.renderer.faces = 0;
	info.renderer.points = 0;

	setRenderTarget(renderTarget);

	if (autoClear || forceClear){
		clear(this->autoClearColor, this->autoClearDepth, this->autoClearStencil);
	}

	if (scene.overrideMaterial != nullptr){

		renderObjects(opaqueObjects, scene, camera, scene.overrideMaterial.get());
		renderObjects(transparentObjects, scene, camera, scene.overrideMaterial.get());

	} else {

		//FIXME setBlending(NoBlending);
		renderObjects(opaqueObjects, scene, camera);

		renderObjects(transparentObjects, scene, camera);

	}

	//TODO handle sprites and lensflares

	//FIXME setDepthTest(true);
	//FIXME setDepthWrite(true);
	//FIXME setColorWrite(true);

}

void GLES2Renderer::projectObject(Object3D<>* o, Camera<float>* camera) {

	typedef typename Object3D<float>::ObjectType ObjectType;

	if (o->visible){
		switch(o->type){
		case ObjectType::LIGHT:
			lights.push_back(dynamic_cast<Light<float>*>(o));
			break;
		case ObjectType::SPRITE:
			//if (o->frustumCulled == false || isSpriteViewable(o) == true){
			//TODO
			//}
			break;
		case ObjectType::LENSFLARE:
			//TODO
			break;
		case ObjectType::MESH:
		case ObjectType::LINE:
		case ObjectType::POINTS: { //Need a scope to have new objects on the stack

			Mesh<float>* m = dynamic_cast<Mesh<float>*>(o);
			Vector3f vector;

			//TODO handle skins

			if (o->frustumCulled == false || isObjectViewable(m) == true){

				if (m->material->visible){

					if (sortObjects){
						vector.setFromMatrixPosition(m->matrixWorld);
						vector.applyProjection(projScreenMatrix);
					}

					if (m->geometry->isMultiMaterial()){

						const auto& groups = m->geometry->getGroups();

						for (unsigned i = 0; i < groups.size(); ++i){
							//TODO handle multimaterials, need to check for transparency
						}

					} else {
						if (m->material->transparent){
							transparentObjects.emplace_back(RenderItem<Mesh<float>>(m, vector.z));
						} else {
							opaqueObjects.emplace_back(RenderItem<Mesh<float>>(m, vector.z));
						}
					}

				}

			}

			break;
		} //Close a scope
		case ObjectType::BASIC:
		case ObjectType::SCENE:
			//Do nothing
			break;
		} //Close the switch
	}

	for (std::unique_ptr<Object3D<float>>& child : o->children){
		projectObject(child.get(), camera);
	}

}

void GLES2Renderer::renderObjects(std::vector<RenderItem<Mesh<float>>>& objects,
		Scene<float>& scene, Camera<float>* camera, IMaterial* mat){

	for (RenderItem<Mesh<float>>& item : objects) {

		Mesh<float>* object = item.object;
		IGeometry<float>* geometry = object->geometry.get();
		IMaterial* material = mat != nullptr ? mat : object->material.get();

		object->modelViewMatrix.multiplyMatrices(camera->matrixWorldInverse, object->matrixWorld);
		object->normalMatrix.getNormalMatrix(object->modelViewMatrix);

		//TODO Trigger on before render event for object

		//TODO Immediate rendering?

		renderBufferDirect(camera, scene.fog, geometry, material, object, item.group);

		//TODO handle post render events

	}

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

void GLES2Renderer::setRenderTarget(std::shared_ptr<IRenderTarget> target) {
	if (target == nullptr){
		glBindFramebuffer(GL_FRAMEBUFFER, 0); //Bind to default target (screen)
	} else {
		target->bind();
	}
	//TODO We need to store every render target that comes through here.
}
std::shared_ptr<IRenderTarget> GLES2Renderer::getRenderTarget() {
	return nullptr; //TODO We need to get gls framebuffer number and connect it to some list of known render targets. (0 is none)
}

std::vector<unsigned char> GLES2Renderer::readRenderTargetPixels(std::shared_ptr<IRenderTarget> target, int x, int y, unsigned width, unsigned height) {
	std::vector<unsigned char> data(width * height * 3);
	target->bind();
	glReadPixels(x, y, width, height, GL_RGB, GL_UNSIGNED_BYTE, data.data());
	return data;
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
