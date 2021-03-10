#pragma once

#include <memory>
#include <vector>
#include <array>
#include <unordered_map>
#include <tuple>
#include <functional>
#include <mutex>


namespace MyEngine {
	template<typename>
	class IRenderer;
}

#include "Color.hpp"
#include "Constants.hpp"
#include "RenderPlugin.hpp"
#include "RenderTarget.hpp"
#include "Scene.hpp"
#include "Camera.hpp"
#include "Material.hpp"
#include "Texture.hpp"
#include "Quaternion.hpp"
#include "Window.hpp"

template<typename T = float>
struct MyEngine::IRenderer {
	enum AutoClearFlag {
		CLEAR_COLOR = 0x1,
		CLEAR_DEPTH = 0x2,
		CLEAR_STENCIL = 0x4,
	};

	static constexpr unsigned char DefaultAutoClear = CLEAR_COLOR | CLEAR_DEPTH | CLEAR_STENCIL;

	virtual void enableAutoClear(bool enable = true) = 0;
	virtual void isAutoClearEnabled() const = 0;

	virtual void setAutoClearFlags(unsigned char flags) = 0;
	virtual unsigned char getAutoClearFlags() const = 0;

	virtual void enableAlpha(bool enable = true) = 0;
	virtual bool isAlphaEnabled() const = 0;

	virtual void enableDepth(bool enable = true) = 0;
	virtual bool isDepthEnabled() const = 0;

	struct Stats {
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
	};

	virtual Stats getStats() const = 0;

	virtual std::shared_ptr<Window> getWindow() = 0;

	virtual void setScissor(int x, int y, unsigned width, unsigned height) = 0;

	virtual void setScissorTest(bool enable = true) = 0;

	virtual void setClearColor(const Color&, float alpha) = 0;

	virtual Color getClearColor() const = 0;

	virtual float getClearAlpha() const = 0;

	virtual void clear(bool color = true, bool depth = true, bool stencil = true) = 0;

	virtual void clearColor() = 0;

	virtual void clearDepth() = 0;

	virtual void clearStencil() = 0;

	virtual void clearTarget(std::shared_ptr<RenderTarget> target, bool color = true, bool depth = true, bool stencil = true) = 0;

	virtual unsigned getMaxAnisotripy() const = 0;

	virtual std::tuple<unsigned, unsigned> getSize() const = 0;

	virtual void setSize(unsigned width, unsigned height) = 0;

	virtual void setPos(unsigned x, unsigned y) = 0;

	virtual void setViewport(int x, int y, unsigned width, unsigned height) = 0;

	virtual std::tuple<int, int, unsigned, unsigned> getViewport() const = 0;

	virtual void setDefaultViewport() = 0;

	virtual void renderBufferImmediate(Mesh<T>* object, std::shared_ptr<Shader::Shader> shader, IMaterial* material) = 0;

	virtual void renderBufferDirect(Camera<T>*, Fog<T>*, IGeometry<T>*, IMaterial*, Mesh<T>* object, int group) = 0;

	virtual void render(Scene<T>& scene, Camera<T>& camera) = 0;

	void render(std::shared_ptr<Scene<T>> scene, std::shared_ptr<Camera<T>> camera) {
		render(*scene, camera.get());
	}

	virtual void setFaceCulling(CullConstant cullFace, CullDirection frontFaceDirection) = 0;

	virtual void setTexture(std::shared_ptr<Texture> texture, unsigned slot = 0) = 0;

	virtual void setRenderTarget(std::shared_ptr<RenderTarget> target) = 0;

	virtual std::shared_ptr<RenderTarget> getRenderTarget() = 0;

	virtual std::vector<unsigned char> readRenderTargetPixels(std::shared_ptr<RenderTarget> target, int x, int y,
	                                                          unsigned width, unsigned height) = 0;

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

	virtual bool needsToClose() = 0;

	virtual void onResize(std::function<void(unsigned, unsigned)>) = 0;
};


