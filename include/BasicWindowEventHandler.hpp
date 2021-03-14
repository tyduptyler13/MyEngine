//
// Created by tyler on 3/14/21.
//

#pragma once

#include <unordered_set>
#include <memory>

#include "Window.hpp"

namespace MyEngine {
	class BasicWindowEventHandler : public Window {
	public:
		Window* addMouseHandler(const std::shared_ptr<MouseEventHandler>& eventHandler) override;
		Window* addGamepadButtonHandler(const std::shared_ptr<GamepadButtonEventHandler>& eventHandler) override;
		Window* addGamepadAxisHandler(const std::shared_ptr<GamepadAxisEventHandler>& eventHandler) override;
		Window* addButtonHandler(const std::shared_ptr<KeyboardEventHandler>& eventHandler) override;

		bool removeMouseHandler(const std::shared_ptr<MouseEventHandler>& eventHandler) override;
		bool removeGamepadButtonHandler(const std::shared_ptr<GamepadButtonEventHandler>& eventHandler) override;
		bool removeGamepadAxisHandler(const std::shared_ptr<GamepadAxisEventHandler>& eventHandler) override;
		bool removeButtonHandler(const std::shared_ptr<KeyboardEventHandler>& eventHandler) override;

	protected:

		std::unordered_set<std::shared_ptr<MouseEventHandler>> mouseEventHandlers;
		std::unordered_set<std::shared_ptr<GamepadButtonEventHandler>> gamepadButtonEventHandlers;
		std::unordered_set<std::shared_ptr<GamepadAxisEventHandler>> gamepadAxisEventHandlers;
		std::unordered_set<std::shared_ptr<KeyboardEventHandler>> keyboardEventHandlers;
	};
}
