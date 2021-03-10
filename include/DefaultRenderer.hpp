#pragma once

#include "IRenderer.hpp"

namespace MyEngine {

	enum struct WindowSystemHint {
		ANY, // Let us pick our preference (default)
		X11, // older linux (nearly always supported)
		WAYLAND, // newer linux (fairly wide support)
		COCOA, // OSX
		UIKIT, // IOS
		ANDROID,
		Win32, // older windows
		UWP, // universal windows platform
	};

	/**
	 * A list of graphics apis we support in order of default preference.
	 */

	enum struct GraphicsAPIHint {
		ANY,
		VULKAN,
		D3D12,
		D3D11,
		OPENGL,
		GLES,
	};

	/**
	 * A set of hints for how to open the window.
	 *
	 * Note: These are only hints and may not always be possible for every window system, none will cause init
	 * to fail if that feature is missing.
	 */
	struct InitHints {
		WindowSystemHint windowSystemHint = WindowSystemHint::ANY;
		GraphicsAPIHint graphicsApiHint = GraphicsAPIHint::ANY;
		bool fullScreen = false;
		unsigned int widthHint = 800;
		unsigned int heightHint = 600;
		unsigned int xPosHint; // Defaults to the default position for the window system unless specified
		unsigned int yPosHint; // Defaults to the default position for the window system unless specified
		std::string windowTitleHint = "MyEngine";
	};

	/**
	 * A special factory for creating the default renderer for the platform
	 * @param hints
	 * @return
	 */
	IRenderer<float> createDefaultRenderer(InitHints hints = InitHints());
}
