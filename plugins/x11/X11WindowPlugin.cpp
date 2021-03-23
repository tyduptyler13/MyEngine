#include <memory>

#include <stdexcept>
#include <spdlog/spdlog.h>
#include <GL/glx.h>
#include <GL/gl.h>

#include "Plugin.hpp"
#include "DefaultRenderer.hpp"
#include "BasicWindowEventHandler.hpp"

using namespace MyEngine;
using namespace MyEngine::DefaultRenderer;

namespace MyEngine::Linux {
	struct X11Window final : public BasicWindowEventHandler {
		X11Window(const InitHints& initHints) {
			Display* display = XOpenDisplay(0);

			static int attribs[] = {
					GLX_RENDER_TYPE, GLX_RGBA_BIT,
					GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
					GLX_DOUBLEBUFFER, true,

					GLX_RED_SIZE, 8,
					GLX_GREEN_SIZE, 8,
					GLX_BLUE_SIZE, 8,
					GLX_ALPHA_SIZE, 8,

					GLX_DEPTH_SIZE, 24,

					GLX_SAMPLE_BUFFERS, 0,

					None
			};

			int fbcount = 0;
			GLXFBConfig* fbc = glXChooseFBConfig(display, DefaultScreen(display), attribs, &fbcount);
			if (!fbc) {
				const char* error = "Failed to retrieve a framebuffer config";
				SPDLOG_ERROR(error);
				throw std::runtime_error(error);
			}

			XVisualInfo* vi = glXGetVisualFromFBConfig(display, fbc[0]);

			XSetWindowAttributes swa;
			swa.colormap = XCreateColormap(display, RootWindow(display, vi->screen), vi->visual, AllocNone);
			swa.border_pixel = 0;
			swa.event_mask =
					StructureNotifyMask |
					ExposureMask |
					KeyPressMask |
					KeyReleaseMask |
					ButtonPressMask |
					ButtonReleaseMask |
					PointerMotionMask;

			::Window win = XCreateWindow(display, RootWindow(display, vi->screen), 0, 0, initHints.widthHint, initHints.heightHint, 0, vi->depth, InputOutput, vi->visual, CWBorderPixel | CWColormap | CWEventMask, &swa);
			if (!win) {
				const char* error = "Failed to create the window!";
				SPDLOG_ERROR(error);
				throw std::runtime_error(error);
			}

			{
				auto sizeHints = XAllocSizeHints();
				sizeHints->flags = PMinSize;
				sizeHints->min_width = initHints.minWidthHint;
				sizeHints->min_height = initHints.minHeightHint;
				XSetWMNormalHints(display, win, sizeHints);
				XFree(sizeHints);
			}

			XMapWindow(display, win);

			// TODO https://github.com/DiligentGraphics/DiligentTools/blob/master/NativeApp/src/Linux/LinuxMain.cpp#L388
		}

		Window* show() override {
			return this;
		}

		Window* hide() override {
			return this;
		}

		Window* close() override {
			return this;
		}

		Window* setPos(unsigned int x, unsigned int y) override {
			return this;
		}

		Window* resize(unsigned int width, unsigned int height) override {
			return this;
		}

		Window* setTitle(const std::string& title) override {
			return this;
		}

		Window* toggleBorder() override {
			return this;
		}

		Window* toggleFullscreen() override {
			return this;
		}

		bool hasBorder() override {
			return false;
		}

		bool isFullScreen() override {
			return false;
		}

		Window* setMouseLock() override {
			return nullptr;
		}

		Window* processEvents() override {
			return Window::processEvents();
		}
	};

	struct X11WindowPlugin final : public Plugin {
		X11WindowPlugin() = default;
		~X11WindowPlugin() override = default;

		static std::unique_ptr<Window> createX11Window(const InitHints& initHints) {
			return std::make_unique<X11Window>(initHints);
		}

		WindowFactoriesContainer::const_iterator it;

		void onLoad() override {
			it = DefaultRenderer::dynamicWindowFactories.emplace(WindowSystemHint::X11, X11WindowPlugin::createX11Window);
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
