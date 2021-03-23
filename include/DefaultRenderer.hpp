#pragma once

#include <unordered_map>
#include <functional>

#include "IRenderer.hpp"

namespace MyEngine::DefaultRenderer {
	enum struct WindowSystemHint {
		ANY, // Let us pick our preference (default)
		X11, // older linux (nearly always supported)
		WAYLAND, // newer linux (fairly wide support)
		COCOA, // OSX
		UIKIT, // IOS
		ANDROID,
		Win32, // older windows
		UWP, // universal windows platform
		HTML, // Specific to html environments
		GLFW, // Cross platform meta windowing system
		SDL, // Cross platform meta window/render system
	};
	WindowSystemHint windowSystemPriority[] = {
			WindowSystemHint::X11,
			WindowSystemHint::WAYLAND,
			WindowSystemHint::COCOA,
			WindowSystemHint::UIKIT,
			WindowSystemHint::ANDROID,
			WindowSystemHint::Win32,
			WindowSystemHint::UWP,
			WindowSystemHint::HTML,
			WindowSystemHint::GLFW,
			WindowSystemHint::SDL,
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
		WEBGL,
	};
	GraphicsAPIHint graphicsApiPriority[] = {
			GraphicsAPIHint::VULKAN,
			GraphicsAPIHint::D3D12,
			GraphicsAPIHint::D3D11,
			GraphicsAPIHint::OPENGL,
			GraphicsAPIHint::GLES,
			GraphicsAPIHint::WEBGL,
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
		int minWidthHint = -1; // The smallest allowed size of the window (-1 for any size)
		int minHeightHint = -1; // The smallest allowed size of the window (-1 for any size)
		int maxWidthHint = -1; // The largest allowed window size (-1 for any size)
		int maxHeightHint = -1; // The largest allwoed window size (-1 for any size)
		unsigned int xPosHint; // Defaults to the default position for the window system unless specified
		unsigned int yPosHint; // Defaults to the default position for the window system unless specified
		std::string windowTitleHint = "MyEngine";
	};

	/**
	 * A special factory for creating the default renderer for the platform
	 * @param hints
	 * @return
	 */
	IRenderer<float> createRenderer(InitHints hints = InitHints());

	typedef std::unordered_multimap<WindowSystemHint, std::function<std::unique_ptr<Window>(
			const InitHints& hints)>> WindowFactoriesContainer;
	typedef std::unordered_multimap<GraphicsAPIHint, std::function<std::unique_ptr<IRenderer<float>>(
			const InitHints& hints)>> RenderFactoriesContainer;
	WindowFactoriesContainer dynamicWindowFactories;
	RenderFactoriesContainer dynamicRendererFactories;
}
