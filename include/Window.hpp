#pragma once

#include <functional>
#include <string>

namespace MyEngine {

	struct Modifier {
		enum {
			NONE = 0,
			KEY_LEFT_SHIFT = 0x1,
			KEY_LEFT_CONTROL = 0x2,
			KEY_LEFT_ALT = 0x4,
			KEY_LEFT_SUPER = 0x8,
			KEY_RIGHT_SHIFT = 0x10,
			KEY_RIGHT_CONTROL = 0x20,
			KEY_RIGHT_ALT = 0x40,
			KEY_RIGHT_SUPER = 0x80
		};
	};

	enum MouseButton {
		None = 0, Left = 0x1, Middle = 0x2, Right = 0x4
	};

	struct KeyboardKey {
		enum {
			// For the most part, we follow GLFW number scheme which maps keys to
			// their
			// ascii equivalent.
			KEY_UNKNOWN = 0,
			KEY_SPACE = 32,
			KEY_APOSTROPHE = 39 /* ' */,
			KEY_COMMA = 44 /* , */,
			KEY_MINUS = 45 /* - */,
			KEY_PERIOD = 46 /* . */,
			KEY_SLASH = 47 /* / */,
			KEY_0 = 48,
			KEY_1 = 49,
			KEY_2 = 50,
			KEY_3 = 51,
			KEY_4 = 52,
			KEY_5 = 53,
			KEY_6 = 54,
			KEY_7 = 55,
			KEY_8 = 56,
			KEY_9 = 57,
			KEY_SEMICOLON = 59 /* ; */,
			KEY_EQUAL = 61 /* = */,
			KEY_A = 65,
			KEY_B = 66,
			KEY_C = 67,
			KEY_D = 68,
			KEY_E = 69,
			KEY_F = 70,
			KEY_G = 71,
			KEY_H = 72,
			KEY_I = 73,
			KEY_J = 74,
			KEY_K = 75,
			KEY_L = 76,
			KEY_M = 77,
			KEY_N = 78,
			KEY_O = 79,
			KEY_P = 80,
			KEY_Q = 81,
			KEY_R = 82,
			KEY_S = 83,
			KEY_T = 84,
			KEY_U = 85,
			KEY_V = 86,
			KEY_W = 87,
			KEY_X = 88,
			KEY_Y = 89,
			KEY_Z = 90,
			KEY_LEFT_BRACKET = 91 /* [ */,
			KEY_BACKSLASH = 92 /* \ */,
			KEY_RIGHT_BRACKET = 93 /* ] */,
			KEY_GRAVE_ACCENT = 96 /* ` */,
			KEY_ESCAPE = 256,
			KEY_ENTER = 257,
			KEY_TAB = 258,
			KEY_BACKSPACE = 259,
			KEY_INSERT = 260,
			KEY_DELETE = 261,
			KEY_RIGHT = 262,
			KEY_LEFT = 263,
			KEY_DOWN = 264,
			KEY_UP = 265,
			KEY_PAGE_UP = 266,
			KEY_PAGE_DOWN = 267,
			KEY_HOME = 268,
			KEY_END = 269,
			KEY_CAPS_LOCK = 280,
			KEY_SCROLL_LOCK = 281,
			KEY_NUM_LOCK = 282,
			KEY_PRINT_SCREEN = 283,
			KEY_PAUSE = 284,
			KEY_F1 = 290,
			KEY_F2 = 291,
			KEY_F3 = 292,
			KEY_F4 = 293,
			KEY_F5 = 294,
			KEY_F6 = 295,
			KEY_F7 = 296,
			KEY_F8 = 297,
			KEY_F9 = 298,
			KEY_F10 = 299,
			KEY_F11 = 300,
			KEY_F12 = 301,
			KEY_F13 = 302,
			KEY_F14 = 303,
			KEY_F15 = 304,
			KEY_F16 = 305,
			KEY_F17 = 306,
			KEY_F18 = 307,
			KEY_F19 = 308,
			KEY_F20 = 309,
			KEY_F21 = 310,
			KEY_F22 = 311,
			KEY_F23 = 312,
			KEY_F24 = 313,
			KEY_F25 = 314,
			KEY_KP_0 = 320,
			KEY_KP_1 = 321,
			KEY_KP_2 = 322,
			KEY_KP_3 = 323,
			KEY_KP_4 = 324,
			KEY_KP_5 = 325,
			KEY_KP_6 = 326,
			KEY_KP_7 = 327,
			KEY_KP_8 = 328,
			KEY_KP_9 = 329,
			KEY_KP_DECIMAL = 330,
			KEY_KP_DIVIDE = 331,
			KEY_KP_MULTIPLY = 332,
			KEY_KP_SUBTRACT = 333,
			KEY_KP_ADD = 334,
			KEY_KP_ENTER = 335,
			KEY_KP_EQUAL = 336,
			KEY_LEFT_SHIFT = 340,
			KEY_LEFT_CONTROL = 341,
			KEY_LEFT_ALT = 343,
			KEY_LEFT_SUPER = 344,
			KEY_RIGHT_SHIFT = 345,
			KEY_RIGHT_CONTROL = 346,
			KEY_RIGHT_ALT = 347,
			KEY_RIGHT_SUPER = 348
		};
	};

	struct EventType {
		enum {
			None, // We aren't sure what type of event this was, maybe this window
			// doesn't support the types. (Probably can assume its pressed)
			Pressed, // A key was pressed and is still down
			Repeat,  // This happens when the key has been down for awhile. Not all
			// windows will support this...
			Released // This happens when a key is released
		};
	};

	typedef std::function<void(MouseButton, EventType,
														 unsigned x, unsigned y,
	                           unsigned dx, unsigned dy)> MouseEvent;
	typedef std::function<void(KeyboardKey, EventType)> KeyboardEvent;
	// Missing scrolling
	// Missing file drops? Do we even want them?
	typedef std::function<void(unsigned gamepadId, unsigned gamepadAxis,
	                           float x, float y)> GamepadAxisEvent;

	/**
	 * A function that handles joystick specific buttons.
	 *
	 * @param gamepadId An identifier for which gamepad is starting the event
	 * @param button A button id for which button is being pressed
	 * @param intensity A degree to which the button is pressed (Will be 1 for
	 * buttons that don't have force sensitivity.)
	 */
	typedef std::function<void(unsigned gamepadId, unsigned button,
	                           float intensity)> GamepadButtonEvent;

	/**
	 * An interface for dealing with windows and their contexts.
	 */
	struct Window {
		/**
		 * This is not an implementation!
		 */
		Window() = delete;

		virtual ~Window() = default;

		virtual Window* create(unsigned width = 800, unsigned height = 600,
		                       unsigned posX = 0, unsigned posY = 0) = 0;

		virtual Window* close() = 0;

		virtual Window* setPos(unsigned x, unsigned y) = 0;

		virtual Window* resize(unsigned width, unsigned height) = 0;

		virtual Window* setTitle(const std::string& title) = 0;

		/**
		 * Removes window border
		 */
		virtual Window* toggleBorder() = 0;

		virtual Window* toggleFullscreen() = 0;

		/**
		 * Hides the default mouse cursor.
		 */
		virtual Window* setMouseLock() = 0;

		/**
		 * Give time to the window to process any events.
		 */
		virtual Window* processEvents() {
			// Defaults to doing nothing (just in case the window isn't providing the events)
		};

		virtual Window* addMouseHandler(MouseEvent) = 0;

		virtual Window* addGamepadButtonHandler(GamepadButtonEvent) = 0;

		virtual Window* addGamepadAxisHandler(GamepadAxisEvent) = 0;

		virtual Window* addButtonHandler(KeyboardEvent) = 0;

		virtual unsigned getModifiers() = 0;
	};
}
