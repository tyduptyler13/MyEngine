#ifndef MYUPLAY_MYENGINE_RENDERER
#define MYUPLAY_MYENGINE_RENDERER

#include <memory>
#include <vector>
#include <array>
#include <unordered_map>
#include <tuple>

#include <SDL2/SDL.h>

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

namespace MyUPlay {

namespace MyEngine {

template <typename T = float>
class Renderer {

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

	std::tuple<unsigned, unsigned> getSize() const {
		int width, height;
		SDL_GetWindowSize(window, &width, &height);
		return std::make_tuple(width, height);
	}
	void setSize(unsigned width, unsigned height){
		SDL_SetWindowSize(window, width, height);
	}
	void setPos(unsigned x, unsigned y){
		SDL_SetWindowPosition(window, x, y);
	}

	virtual void setViewport(int x, int y, unsigned width, unsigned height) = 0;
	virtual std::tuple<int, int, unsigned, unsigned> getViewport() const = 0;
	virtual void setDefaultViewport() = 0;

	virtual void renderBufferImmediate(Object3D<T>* object, std::shared_ptr<Shader::Shader> shader, IMaterial* material) = 0;
	virtual void renderBufferDirect(Camera<T>* camera, std::vector<Light<T>*>& lights, Fog<T>& fog, IMaterial* material,
			Object3D<T>* object, Object3D<T>* objectGroup) = 0;

	virtual void render(Scene<T>& scene, Camera<T>* camera, std::shared_ptr<IRenderTarget> renderTarget = nullptr, bool forceClear = false) = 0;

	virtual void setFaceCulling(CullConstant cullFace, CullDirection frontFaceDirection) = 0;
	virtual void setTexture(std::shared_ptr<Texture> texture, unsigned slot = 0) = 0;
	virtual void setRenderTarget(std::shared_ptr<IRenderTarget> target) = 0;
	virtual std::shared_ptr<IRenderTarget> getRenderTarget() = 0;
	virtual std::vector<unsigned char> readRenderTargetPixels(std::shared_ptr<IRenderTarget> target, int x, int y, unsigned width, unsigned height) = 0;

	/**
	 * Use this function to get a list of display modes for all monitors.
	 * The top level vector is the monitors
	 * The second level is a list of display modes for that monitor
	 */
	std::vector<std::vector<SDL_DisplayMode> > getDisplayModes() const {

		unsigned monitors = SDL_GetNumVideoDisplays();

		std::vector<std::vector<SDL_DisplayMode> > monitorList(monitors);

		for (unsigned i = 0; i < monitors; ++i){

			unsigned displayModes = SDL_GetNumDisplayModes(i);

			monitorList[i].reserve(displayModes);

			for (unsigned j = 0; j < displayModes; ++j){
				SDL_GetDisplayMode(i, j, &monitorList[i][j]);
			}
		}

		return monitorList;
	}

	/**
	 * This mode alters screen resolution and settings for this program.
	 * This is what most people consider real full screen.
	 */
	void setFullScreen(){
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
		SDL_GetWindowPosition(window, &windowX, &windowY);
	}

	/**
	 * In this mode no resolution change is made and the window will take the size of the desktop.
	 * This is useful for quickly changing the window to desktop size without a border.
	 */
	void setFakeFullScreen(){
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		SDL_GetWindowSize(window, &windowWidth, &windowHeight);
		SDL_GetWindowPosition(window, &windowX, &windowY);
	}

	void setWindowed(){
		SDL_SetWindowFullscreen(window, 0);
		SDL_GetWindowSize(window, &windowWidth, &windowHeight);
		SDL_GetWindowPosition(window, &windowX, &windowY);
	}

protected:

	int maxTextures;

	int windowX = 0,
			windowY = 0;
	int windowWidth,
	windowHeight;

	int opaqueObjectsLastIndex = -1, transparentObjectsLastIndex = -1;

	SDL_Window* window;

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

}

}

#endif
