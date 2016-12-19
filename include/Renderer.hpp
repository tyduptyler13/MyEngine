#ifndef MYUPLAY_MYENGINE_RENDERER
#define MYUPLAY_MYENGINE_RENDERER

#include <memory>
#include <vector>
#include <array>
#include <unordered_map>
#include <tuple>
#include <functional>
#include <thread>

namespace MyUPlay {

	namespace MyEngine {

		template<typename>
		class Renderer;

	}
}

#include "Color.hpp"
#include "Constants.hpp"
#include "RenderPlugin.hpp"
#include "RenderTarget.hpp"
#include "Light.hpp"
#include "Object3D.hpp"
#include "Scene.hpp"
#include "Camera.hpp"
#include "Material.hpp"
#include "Texture.hpp"
#include "Shader/Shader.hpp"
#include "Quaternion.hpp"

template <typename T = float>
class MyUPlay::MyEngine::Renderer {

public:

	virtual ~Renderer(){}

	T sortEpsilon = 0.1;
	T frustumEpsilon = 0.1;

	unsigned currentLineWidth;

	bool autoClear = true,
			autoClearColor = true,
			autoClearDepth = true,
			autoClearStencil = true;

	bool alpha = false,
			depth = true,
			stencil = true,
			antialias = false,
			premultipliedAlpha = true,
			preserveDrawingBuffer = false;

	T gammaFactor = 2.0;
	bool gammaInput = false,
			gammaOutput = false;

	bool shadowMapEnabled = false;
	ShadowMapType shadowMapType = PCFShadowMap;
	CullConstant shadowMapCullFace = CullFaceFront;
	bool shadowMapDebug = false,
			shadowMapCascade = false;

	bool autoScaleCubeMaps = true;

	struct {
		struct {
			unsigned programs = 0,
					geometries = 0,
					textures = 0;
		} memory;
		struct {
			unsigned calls = 0,
					vertices = 0,
					faces = 0,
					points = 0;
		} renderer;
	} info;

	virtual void setScissor(int x, int y, unsigned width, unsigned height) = 0;
	virtual void setScissorTest(bool enable = true) = 0;

	virtual void setClearColor(const Color&, float alpha) = 0;
	virtual Color getClearColor() const = 0;
	virtual float getClearAlpha() const = 0;

	virtual void clear(bool color = true, bool depth = true, bool stencil = true) = 0;

	virtual void clearColor() = 0;
	virtual void clearDepth() = 0;
	virtual void clearStencil() = 0;
	virtual void clearTarget(std::shared_ptr<IRenderTarget> target, bool color = true, bool depth = true, bool stencil = true) = 0;

	virtual unsigned getMaxAnisotripy() const = 0;

	virtual std::tuple<unsigned, unsigned> getSize() const = 0;
	virtual void setSize(unsigned width, unsigned height) = 0;
	virtual void setPos(unsigned x, unsigned y) = 0;

	virtual void setViewport(int x, int y, unsigned width, unsigned height) = 0;
	virtual std::tuple<int, int, unsigned, unsigned> getViewport() const = 0;
	virtual void setDefaultViewport() = 0;

	virtual void renderBufferImmediate(Mesh<T>* object, std::shared_ptr<Shader::Shader> shader, IMaterial* material) = 0;
	virtual void renderBufferDirect(Camera<T>*, Fog<T>*, IGeometry<T>*, IMaterial*, Mesh<T>* object, int group) = 0;

	virtual void render(Scene<T>& scene, Camera<T>* camera, std::shared_ptr<IRenderTarget> renderTarget = nullptr, bool forceClear = false) = 0;
	void render(std::shared_ptr<Scene<T>> scene, std::shared_ptr<Camera<T>> camera) {
		render(*scene, camera.get());
	}
	template <class CB>
	void renderAsync(std::shared_ptr<Scene<T>> scene, std::shared_ptr<Camera<T>> camera, CB& cb){
		CB* cbpointer = new CB(cb);
		std::thread([this, scene, camera, cbpointer]{
			this->render(scene, camera);
			(*cbpointer)();
			delete cbpointer;
		}).detach();
	}

	virtual void setFaceCulling(CullConstant cullFace, CullDirection frontFaceDirection) = 0;
	virtual void setTexture(std::shared_ptr<Texture> texture, unsigned slot = 0) = 0;
	virtual void setRenderTarget(std::shared_ptr<IRenderTarget> target) = 0;
	virtual std::shared_ptr<IRenderTarget> getRenderTarget() = 0;
	virtual std::vector<unsigned char> readRenderTargetPixels(std::shared_ptr<IRenderTarget> target, int x, int y, unsigned width, unsigned height) = 0;

	/**
	 * This mode alters screen resolution and settings for this program.
	 * This is what most people consider real full screen.
	 */
	virtual void setFullScreen() = 0;

	/**
	 * In this mode, a window removes its frame decoration and matches the desktop size.
	 * This is useful for quickly changing the window to desktop size without a border.
	 */
	virtual void setFakeFullScreen() = 0;

	/**
	 * This is the standard mode.
	 */
	virtual void setWindowed() = 0;

	virtual void setVsync(bool enable) = 0;

	/**
	 * The following is a blocking call to a loop. The user is expected to
	 * put a lambda function in the call of this function to handle rendering
	 * and do any game logic expected at every frame. This function will call the
	 * function parameter every time the screen needs to be drawn, if vsync is disabled
	 * then the function will be called immediately after rendering is finished.
	 * Note: A buffer swap will occur after every call of this function, not drawing may
	 * result in a black screen if color clearing is automatic.
	 * @param function - The function with the following params
	 * 	@param delta - The built in clock for this function. You don't need to have
	 *		your own.
	 * @return - Return true to exit the loop early. False will continue the loop
	 * 	infinitely (until window close).
	 */
	virtual void loop(std::function<bool(double delta)>) = 0;

	virtual bool needsToClose() = 0;

	virtual void onResize(std::function<void(int, int)>) = 0;

	template <typename T2>
	void onResize(T2& func) {
		//We have to create a pointer because the lambda will make the function const.
		T2* pfunc = new T2(func); //We don't actually own the memory! Node will clean it up.
		onResize([pfunc](int width, int height){ //Func will be copied and stored with this lambda.
			(*pfunc)(width, height);
		});
	}

protected:

	int maxTextures;

	int windowX = 0,
			windowY = 0;
	int windowWidth,
	windowHeight;

	int opaqueObjectsLastIndex = -1, transparentObjectsLastIndex = -1;

	Color clearColorv;
	T clearAlpha;

	//TODO type? currentBlending;
	BlendingEquation currentBlendEquation;
	BlendingSource currentBlendSrc;
	BlendingDestination currentBlendDst;
	BlendingEquation currentBlendEquationAlpha;
	BlendingSource currentBlendSrcAlpha;
	BlendingDestination currentBlendDstAlpha;

	/**
	 * This variable is for tracking the last known good sorting of objects for the camera.
	 * As long as the camera doesn't move beyond some epsilon and the scene doesn't have
	 * any new changes then we can skip sorting and use previous sortings. Rotation won't
	 * matter because it is distance sorted anyways. Frustum will use lastSortedRotation.
	 */
	Vector3f lastSortedPos;

	/**
	 * This is the same type of variable as above except it triggers a new check for
	 * objects that have left/entered the frustum. Sorting stays the same.
	 */
	Quaternionf lastSortedRot;

};

#ifdef NBINDING_MODE

#include "nbind/api.h"

namespace {

	using namespace MyUPlay::MyEngine;

	NBIND_CLASS(Renderer<float>, Renderer) {

		method(getSize);
		method(setSize);
		method(setViewport);
		method(getViewport);

		multimethod(render, args(std::shared_ptr<Scene<float>>, std::shared_ptr<Camera<float>>));
		multimethod(template renderAsync<nbind::cbFunction>, args(std::shared_ptr<Scene<float>>, std::shared_ptr<Camera<float>>, nbind::cbFunction&), "renderAsync");

		method(setFullScreen);
		method(setWindowed);

		multimethod(template onResize<nbind::cbFunction>, args(nbind::cbFunction&), "onResize");
		//Note: Loop is not recommended to be exposed to javascript as it is a long term blocking call.
		//If you wish to use loop behavior, duplicate its code in javascript using a callback and settimeout.


		method(needsToClose);

	}

}

#endif

#endif
