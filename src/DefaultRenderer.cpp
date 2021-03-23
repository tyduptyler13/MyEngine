#include <memory>
#include <stdexcept>

#include "DefaultRenderer.hpp"
#include "spdlog/spdlog.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

using namespace std;
using namespace MyEngine;
using namespace MyEngine::DefaultRenderer;

static std::unique_ptr<Window> createWindow(const InitHints& hints) {
	if (hints.windowSystemHint == WindowSystemHint::ANY) {
		for (WindowSystemHint windowType : windowSystemPriority) {
			for (auto it = dynamicWindowFactories.find(windowType); it != dynamicWindowFactories.end(); it++) {
				try {
					return it->second(hints);
				} catch (...) {
					SPDLOG_DEBUG("Failed to instantiate a window!");
				}
			}
		}
		SPDLOG_DEBUG("Ran out of windows to try for ANY type");
	} else {
		for (auto it = dynamicWindowFactories.find(hints.windowSystemHint); it != dynamicWindowFactories.end(); it++) {
			try {
				return it->second(hints);
			} catch (...) {
				SPDLOG_DEBUG("Failed to instantiate a window!");
			}
		}
		SPDLOG_DEBUG("Ran out of windows to try for {}", hints.windowSystemHint);
	}
	throw runtime_error("No window system could be found that supports this system!");
}

IRenderer<float> DefaultRenderer::createRenderer(InitHints hints) {
	std::unique_ptr<Window> window = createWindow(hints);

  // TODO
}

#if 0

// TODO reimplement all of the following via bgfx
// TODO implement a window factory that works on every platform (gross but required for vulkan/etc)

GLES2Renderer::GLES2Renderer(unsigned antialias, GLFWmonitor* monitor, GLES2Renderer* share) : IRenderer() {

	std::vector<std::pair<int, int>> hints = {{
			                                          std::make_pair(GLFW_CLIENT_API, GLFW_OPENGL_ES_API), //Hard constraint.
			                                          std::make_pair(GLFW_CONTEXT_VERSION_MAJOR, 2),
			                                          std::make_pair(GLFW_CONTEXT_VERSION_MINOR, 0),
			                                          std::make_pair(GLFW_SAMPLES, antialias)
	                                          }};

	glfw = new GLFWManager(hints, 800, 600, "MyEngine", monitor, share ? share->glfw : nullptr);

	auto lock = GLFWManager::getLock();

	glfw->makeContextCurrent();

	try {
		std::string version(reinterpret_cast<const char*>(glGetString(GL_VERSION)));
		logger.log("GL VERSION: " + version);
		std::string renderer(reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
		logger.log("GL RENDERER: " + renderer);
	} catch (...) {
		logger.log("GL Error" + std::to_string(glGetError()));
		throw runtime_error("Failed to call basic Opengl function. Context problems!");
	}

	//Every window instance needs to be added.

	glfw->onResize([this](int width, int height){
		this->windowWidth = width;
		this->windowHeight = height;
	});

	onResize([](int width, int height){
		glViewport(0, 0, width, height);
	});

	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextures);

}

GLES2Renderer::~GLES2Renderer(){
	delete glfw;
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
void GLES2Renderer::clearTarget(std::shared_ptr<RenderTarget> target, bool color, bool depth, bool stencil) {
	target->bind();
	clear(color, depth, stencil);
	glBindFramebuffer(GL_FRAMEBUFFER, 0); //Unbind
}

void GLES2Renderer::setDepthTest(bool b){
	if (b) {
		glEnable(GL_DEPTH_TEST);
	} else {
		glDisable(GL_DEPTH_TEST);
	}
}
void GLES2Renderer::setDepthWrite(bool b){
	glDepthMask(b);
}
void GLES2Renderer::setColorWrite(bool b){
	glColorMask(b, b, b, b);
}

unsigned GLES2Renderer::getMaxAnisotripy() const  {
	GLint i;
	glGetIntegerv(GL_TEXTURE_MAX_ANISOTROPY_EXT, &i);
	return i;
}

std::tuple<unsigned, unsigned> GLES2Renderer::getSize() const {
	int w, h;
	glfw->glfwFunction([&](GLFWwindow* window){
		glfwGetWindowSize(window, &w, &h);
	});
	return make_tuple(w, h);
}

void GLES2Renderer::setSize(unsigned width, unsigned height) {
	glfw->glfwFunction([width, height](GLFWwindow* window){
		glfwSetWindowSize(window, width, height);
	});
}

void GLES2Renderer::setPos(unsigned x, unsigned y) {
	glfw->glfwFunction([x, y](GLFWwindow* window){
		glfwSetWindowPos(window, x, y);
	});
}

void GLES2Renderer::setViewport(int x, int y, unsigned width, unsigned height)  {
	auto lock = glfw->getLock();
	glViewport(x, y, width, height);
}
std::tuple<int, int, unsigned, unsigned> GLES2Renderer::getViewport() const {
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	return make_tuple(viewport[0], viewport[1], viewport[2], viewport[3]);
}
void GLES2Renderer::setDefaultViewport() {
	auto lock = glfw->getLock();
	int w, h;
	glfw->glfwFunction([&](GLFWwindow* window){
		glfwGetFramebufferSize(window, &w, &h);
	});
	setViewport(0, 0, w, h);
}

void GLES2Renderer::setFullScreen() {
#ifndef __EMSCRIPTEN__
	glfw->glfwFunction([](GLFWwindow* window){
		GLFWmonitor* const mon = glfwGetPrimaryMonitor();
		const GLFWvidmode* const mode = glfwGetVideoMode(mon);
		glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
		glfwSetWindowMonitor(window, mon, 0, 0, mode->width, mode->height, GLFW_DONT_CARE);
	});
#else
	EM_ASM(
			Document.requestFullscreen();
	);
#endif

}

void GLES2Renderer::setFakeFullScreen() {
	static_assert(true, "FIXME: Implement fake fullscreen.");
	//TODO
}

void GLES2Renderer::setWindowed() {
#ifndef __EMSCRIPTEN__
	glfw->glfwFunction([&](GLFWwindow* window){
		GLFWmonitor* const mon = glfwGetWindowMonitor(window);
		if (mon == nullptr) return; //Already windowed.
		const GLFWvidmode* const mode = glfwGetVideoMode(mon);
		glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
		glfwSetWindowMonitor(window, nullptr, this->windowX, this->windowY, mode->width, mode->height, GLFW_DONT_CARE);
	});
#else
	EM_ASM(
			Document.exitFullscreen();
	);
#endif
}

void GLES2Renderer::processEvents() {
	auto lock = glfw->getLock();
	glfwPollEvents();
}

bool GLES2Renderer::needsToClose() {
	bool shouldClose;
	glfw->glfwFunction([&](GLFWwindow* window){
		//Sneaking in some event handling.
		glfwPollEvents();
		shouldClose = glfwWindowShouldClose(window);
	});
	return shouldClose;
}

void GLES2Renderer::glfwFunction(std::function<void()> func) {
	auto lock = glfw->getLock();
	glfw->makeContextCurrent();
	func();
}

void GLES2Renderer::onResize(std::function<void(int, int)> func) {
	auto lock = glfw->getLock();
	glfw->onFrameResize([this, func](int width, int height){
		this->glfwFunction([&]{
			func(width, height);
		});
	});
}

void GLES2Renderer::renderBufferImmediate(Mesh<float>* object, std::shared_ptr<Shader::Shader> program, IMaterial* material)  {



}

void GLES2Renderer::renderBufferDirect(Camera<float>* camera, Fog<float>* fog, IGeometry<float>* geometry,
                                       IMaterial* material, Mesh<float>* object, int group) {

	setMaterial(material);

	/*
	 * Assume that the last buffer bound is the index buffer if it exists.
	 */
	material->shader->prepare(glfw->contextGroup, camera, object, lights);

	//TODO handle program caching
	//TODO handle fog, probably change it to a shaderNode

	//TODO morph targets (bones and such)

	//Geometry is already sent by the shader prepare statement. (Allows for more custom handling of geometry)

	//TODO Handle wireframe mode
	//TODO Expand to points, lines, sprites

	info.renderer.calls++;

	if (object->geometry->isBufferGeometry()){
		if (group != -1) {
			auto groups = object->geometry->getGroups();
			glDrawElements(GL_TRIANGLES, groups[group].count, GL_UNSIGNED_INT, reinterpret_cast<void*>(groups[group].start * sizeof(unsigned short)));
			info.renderer.faces += groups[group].count / 3;
			info.renderer.vertices += groups[group].count;
		} else {
			glDrawElements(GL_TRIANGLES, object->geometry->size(), GL_UNSIGNED_INT, nullptr);
			info.renderer.faces += object->geometry->size() / 3;
			info.renderer.vertices += object->geometry->size();
		}
	} else {
		if (group != -1){
			auto groups = object->geometry->getGroups();
			glDrawArrays(GL_TRIANGLES, groups[group].start, groups[group].count);
			info.renderer.faces += groups[group].count / 3;
			info.renderer.vertices += groups[group].count;
		} else {
			glDrawArrays(GL_TRIANGLES, 0, object->geometry->size());
			info.renderer.faces += object->geometry->size() / 3;
			info.renderer.vertices += object->geometry->size();
		}
	}


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

void GLES2Renderer::render(Scene<float>& scene, Camera<float>* camera, std::shared_ptr<RenderTarget> renderTarget, bool forceClear)  {

	//Only one render may be in progress at a time.
	auto lock = glfw->getLock();

	glfw->makeContextCurrent();

	for (auto& pair : this->prePlugins) {
		pair.second(scene, camera);
	}

	if (scene.autoUpdate) scene.updateMatrixWorld();

	if (camera->parent == nullptr) camera->updateMatrixWorld();

	camera->matrixWorldInverse.getInverse(camera->matrixWorld);

	projScreenMatrix = camera->projectionMatrix * camera->matrixWorldInverse;

	frustum.setFromMatrix(projScreenMatrix);

	if (renderTarget != nullptr){
		renderTarget->bind();
	}

	opaqueObjects.clear();
	transparentObjects.clear();

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

		renderObjects(opaqueObjects, scene, camera);
		renderObjects(transparentObjects, scene, camera);

	}

	for (auto& pair : this->postPlugins) {
		pair.second(scene, camera);
	}

	glfw->glfwFunction([](GLFWwindow* window){
		glfwSwapBuffers(window);
	});

	//TODO handle sprites and lensflares

	//This allows it to be cleared for the next render.
	setDepthTest(true);
	setDepthWrite(true);
	setColorWrite(true);

}

void GLES2Renderer::setMaterial(IMaterial* mat) {

	//Set culling
	if (mat->side == DoubleSide) {
		glDisable(GL_CULL_FACE);
	} else {
		glEnable(GL_CULL_FACE);
		if (mat->side == FrontSide) {
			glCullFace(GL_BACK); //Cull the back, leaving front
		} else {
			glCullFace(GL_FRONT);
		}
	}

	//Set blending
	if (mat->transparent){

		if (mat->blending == NoBlending){
			glDisable(GL_BLEND);
		} else {
			glEnable(GL_BLEND);

			if (mat->blending == AdditiveBlending) {
				if (premultipliedAlpha) {
					glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
					glBlendFuncSeparate(GL_ONE, GL_ONE, GL_ONE, GL_ONE);
				} else {
					glBlendEquation(GL_FUNC_ADD);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE);
				}
			} else if (mat->blending == SubtractiveBlending) {
				if (premultipliedAlpha) {
					glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
					glBlendFuncSeparate(GL_ZERO, GL_ZERO, GL_ONE_MINUS_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA);
				} else {
					glBlendEquation(GL_FUNC_ADD);
					glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_COLOR);
				}
			} else if (mat->blending == MultiplyBlending) {
				if (premultipliedAlpha) {
					glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
					glBlendFuncSeparate(GL_ZERO, GL_SRC_COLOR, GL_ZERO, GL_SRC_ALPHA);
				} else {
					glBlendEquation(GL_FUNC_ADD);
					glBlendFunc(GL_ZERO, GL_SRC_COLOR);
				}
			} else {
				if (premultipliedAlpha) {
					glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
					glBlendFuncSeparate(GL_ONE, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
				} else {
					glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
					glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
				}
			} //TODO Handle custom blending

		}

	} else {
		glDisable(GL_BLEND);
	}

	//Handle the depth function
	switch(mat->depthFunc) {
		case NeverDepth:
			glDepthFunc(GL_NEVER);
			break;
		case AlwaysDepth:
			glDepthFunc(GL_ALWAYS);
			break;
		case LessDepth:
			glDepthFunc(GL_LESS);
			break;
		case LessEqualDepth:
			glDepthFunc(GL_LEQUAL);
			break;
		case EqualDepth:
			glDepthFunc(GL_EQUAL);
			break;
		case GreaterEqualDepth:
			glDepthFunc(GL_GEQUAL);
			break;
		case GreaterDepth:
			glDepthFunc(GL_GREATER);
			break;
		case NotEqualDepth:
			glDepthFunc(GL_NOTEQUAL);
			break;
		default:
			glDepthFunc(GL_LEQUAL);
	}

	//Handle if gl has the writes enabled.
	setDepthTest(mat->depthTest);
	setDepthWrite(mat->depthWrite);
	setColorWrite(mat->colorWrite);

	//Handle polygon offsets
	if (mat->polygonOffset){
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(mat->polygonOffsetFactor, mat->polygonOffsetUnits);
	} else {
		glDisable(GL_POLYGON_OFFSET_FILL);
	}

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

				if (m->material == nullptr || m->geometry == nullptr) break; //Don't add misformed meshes.

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
								if (m->material->transparent) { //FIXME This is a multimaterial, need to check some sort of array somehow
									transparentObjects.emplace_back(RenderItem<Mesh<float>>(m, vector.z, i));
								} else {
									opaqueObjects.emplace_back(RenderItem<Mesh<float>>(m, vector.z, i));
								}
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

	for (std::shared_ptr<Object3D<float>>& child : o->children){
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

renderBufferDirect(camera, scene.fog.get(), geometry, material, object, item.group);

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

void GLES2Renderer::setRenderTarget(std::shared_ptr<RenderTarget> target) {
	auto lock = glfw->getLock();
	if (target == nullptr){
		glBindFramebuffer(GL_FRAMEBUFFER, 0); //Bind to default target (screen)
	} else {
		target->bind();
	}
	//TODO We need to store every render target that comes through here.
}
std::shared_ptr<RenderTarget> GLES2Renderer::getRenderTarget() {
	return nullptr; //TODO We need to get gls framebuffer number and connect it to some list of known render targets. (0 is none)
}

std::vector<unsigned char> GLES2Renderer::readRenderTargetPixels(std::shared_ptr<RenderTarget> target, int x, int y, unsigned width, unsigned height) {
	std::vector<unsigned char> data(width * height * 3);
	target->bind();
	glReadPixels(x, y, width, height, GL_RGB, GL_UNSIGNED_BYTE, data.data());
	return data;
}

void GLES2Renderer::setVsync(bool enable) {
	auto lock = glfw->getLock();
	glfwSwapInterval(enable ? 1 : 0);
}

bool GLES2Renderer::isObjectViewable(Mesh<float>* object) {

	if (object->geometry->boundingSphere == nullptr) {
		object->geometry->computeBoundingSphere();
	}

	Sphere<float> sphere(*object->geometry->boundingSphere);
	sphere.applyMatrix4(object->matrixWorld);

	return isSphereViewable(sphere);

}

bool GLES2Renderer::isSphereViewable(Spheref s) {

	if (!frustum.intersectsSphere(s)){
		return false;
	}

	//TODO add extra clipping.

	return true;

}

//Other specializations left out for now.

#endif