#include <memory>

#include "Plugin.hpp"
#include "DefaultRenderer.hpp"
#include "BasicWindowEventHandler.hpp"

using namespace MyEngine;
using namespace MyEngine::DefaultRenderer;

namespace MyEngine::Linux {
	struct X11Window final : public BasicWindowEventHandler {
		X11Window() {

		}

		virtual Window* show() override {
			return this;
		}

		virtual Window* hide() override {
			return this;
		}

		virtual Window* close() override {
			return this;
		}

		virtual Window* setPos(unsigned int x, unsigned int y) override {
			return this;
		}

		virtual Window* resize(unsigned int width, unsigned int height) override {
			return this;
		}

		virtual Window* setTitle(const std::string& title) override {
			return this;
		}

		virtual Window* toggleBorder() override {
			return this;
		}

		virtual Window* toggleFullscreen() override {
			return this;
		}

		virtual bool hasBorder() override {
			return false;
		}

		virtual bool isFullScreen() override {
			return false;
		}

		virtual Window* setMouseLock() override {
			return nullptr;
		}

		virtual Window* processEvents() override {
			return Window::processEvents();
		}
	};

	struct X11WindowPlugin final : public Plugin {
		X11WindowPlugin() = default;
		~X11WindowPlugin() override = default;

		static std::unique_ptr<Window> createX11Window() {
			return std::make_unique<X11Window>();
		}

		WindowFactoriesContainer::const_iterator it;

		void onLoad() override {
			it = DefaultRenderer::dynamicWindowFactories.insert(std::make_pair(WindowSystemHint::X11, X11WindowPlugin::createX11Window));
		}

		void onUnload() override {
			DefaultRenderer::dynamicWindowFactories.erase(it);
		}
	};
}

extern "C" {
	MyEngine::Plugin* exportMyEngineX11Plugin() {
		return new Linux::X11WindowPlugin();
	}

	void cleanupMyEngineX11Plugin(MyEngine::Plugin* plugin) {
		delete plugin;
	}
}
