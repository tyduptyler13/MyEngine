#pragma once

#include "Renderer.hpp"

namespace MyEngine {
	/**
	 * The ultimate generic renderer. While more specialized renderers may offer better control, memory efficiency, or speed,
	 * this renderer will work on nearly every platform. All of the platform details will be abstracted away for you to keep
	 * this renderer's use as simple as possible.
	 *
	 * Notes: This renderer will create exactly one window using
	 */
	struct DefaultRenderer : public Renderer<float> {
		DefaultRenderer();

		~DefaultRenderer() override;

		enum WindowSystemHint {
			ANY, // Let us pick our preference (default)
			X11, // older linux (nearly always supported)
			WAYLAND, // newer linux (fairly wide support)
			COCOA, // OSX
			UIKIT, // IOS
			ANDROID,
			Win32, // older windows
			UWP, // universal windows platform
		};

		enum GraphicsAPIHint {
			VULKAN,
			OPENGL,
			GLES,
			D3D11,
			D3D12,
		};

		/**
		 * A set of hints for how to open the window.
		 *
		 * Note: These are only hints and may not always be possible for every window system, none will cause init
		 * to fail if that feature is missing.
		 */
		struct InitHints {
			WindowSystemHint windowSystemHint = WindowSystemHint::ANY;
			bool fullScreen = false;
			unsigned int widthHint = 800;
			unsigned int heightHint = 600;
			unsigned int xPosHint; // Defaults to the default position for the window system unless specified
			unsigned int yPosHint; // Defaults to the default position for the window system unless specified
			std::string windowTitleHint = "MyEngine";
		};

		// WIP area, this may get moved to the constructor
		void init(const InitHints& hints);

	private:
		std::unique_ptr<Window> window;
	};
}
