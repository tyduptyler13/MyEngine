//
// Created by tyler on 3/14/21.
//

#pragma once

#include <unordered_set>
#include <memory>

#include <spdlog/spdlog.h>

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

		void triggerMouseHandlers(MouseButton button, EventType type,
		                          unsigned x, unsigned y,
		                          unsigned dx, unsigned dy) {
			for (auto it = mouseEventHandlers.begin(); it != mouseEventHandlers.end(); ++it) {
				try {
					(**it)(button, type, x, y, dx, dy);
				} catch(...) {
					SPDLOG_DEBUG("Failed to execute a mouse handler!")
				}
			}
		}

		void triggerGamepadButtonHandlers(unsigned gamepadId, unsigned button, float intensity) {
			for (auto it = gamepadButtonEventHandlers.begin(); it != gamepadButtonEventHandlers.end(); ++it) {
				try {
					(**it)(gamepadId, button, intensity);
				} catch(...) {
					SPDLOG_DEBUG("Failed to execute a gamepad button handler!")
				}
			}
		}

		void triggerGamepadAxisHandlers(unsigned gamepadId, unsigned gamepadAxis,
		                                float x, float y) {
			for (auto it = gamepadAxisEventHandlers.begin(); it != gamepadAxisEventHandlers.end(); ++it) {
				try {
					(**it)(gamepadId, gamepadAxis, x, y);
				} catch(...) {
					SPDLOG_DEBUG("Failed to execute a gamepad axis handler!")
				}
			}
		}

		void triggerKeyboardHandlers(KeyboardKey key, EventType type) {
			for (auto it = keyboardEventHandlers.begin(); it != keyboardEventHandlers.end(); ++it) {
				try {
					(**it)(key, type);
				} catch(...) {
					SPDLOG_DEBUG("Failed to execute a keyboard handler!")
				}
			}
		}

		std::unordered_set<std::shared_ptr<MouseEventHandler>> mouseEventHandlers;
		std::unordered_set<std::shared_ptr<GamepadButtonEventHandler>> gamepadButtonEventHandlers;
		std::unordered_set<std::shared_ptr<GamepadAxisEventHandler>> gamepadAxisEventHandlers;
		std::unordered_set<std::shared_ptr<KeyboardEventHandler>> keyboardEventHandlers;
	};
}
