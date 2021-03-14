//
// Created by tyler on 3/14/21.
//

#include "../include/BasicWindowEventHandler.hpp"

using namespace MyEngine;

Window* BasicWindowEventHandler::addMouseHandler(const std::shared_ptr<MouseEventHandler>& eventHandler) {
	mouseEventHandlers.insert(eventHandler);
	return this;
}

Window* BasicWindowEventHandler::addGamepadButtonHandler(const std::shared_ptr<GamepadButtonEventHandler>& eventHandler) {
	gamepadButtonEventHandlers.insert(eventHandler);
	return this;
}

Window* BasicWindowEventHandler::addGamepadAxisHandler(const std::shared_ptr<GamepadAxisEventHandler>& eventHandler) {
	gamepadAxisEventHandlers.insert(eventHandler);
	return this;
}

Window* BasicWindowEventHandler::addButtonHandler(const std::shared_ptr<KeyboardEventHandler>& eventHandler) {
	keyboardEventHandlers.insert(eventHandler);
	return this;
}

bool BasicWindowEventHandler::removeMouseHandler(const std::shared_ptr<MouseEventHandler>& eventHandler) {
	return mouseEventHandlers.erase(eventHandler) == 1;
}

bool BasicWindowEventHandler::removeGamepadButtonHandler(const std::shared_ptr<GamepadButtonEventHandler>& eventHandler) {
	return gamepadButtonEventHandlers.erase(eventHandler) == 1;
}

bool BasicWindowEventHandler::removeGamepadAxisHandler(const std::shared_ptr<GamepadAxisEventHandler>& eventHandler) {
	return gamepadAxisEventHandlers.erase(eventHandler) == 1;
}

bool BasicWindowEventHandler::removeButtonHandler(const std::shared_ptr<KeyboardEventHandler>& eventHandler) {
	return keyboardEventHandlers.erase(eventHandler) == 1;
}
