#ifndef MYUPLAY_MYENGINE_GLFWMANAGER
#define MYUPLAY_MYENGINE_GLFWMANAGER

#include <GLFW/glfw3.h>
#include <mutex>
#include <vector>
#include <functional>

#include "Log.hpp"

namespace MyUPlay {
	namespace MyEngine {

		/**
		 * A manager allows resources to be managed properly between contexts.
		 *
		 * If two contexts have sharing enabled then buffers won't need to be
		 * copied and updated for multiple windows. All buffers are managed
		 * by GLFWManagers.
		 *
		 * A manager may only own one window each. To create shared contexts,
		 * you pass an existing GLFWManager pointer to the constructor of a
		 * new GLFWManager.
		 */
		class GLFWManager {

		public:


			GLFWManager(std::vector<std::pair<int, int>> hints, int width, int height,
					const char* title, GLFWmonitor* mon = nullptr, const GLFWManager* manager = nullptr){

				std::lock_guard<std::recursive_mutex> guard(lock);

				if (instances == 0){
					glfwInit();
					glfwLog.log("Initialized");
					glfwSetErrorCallback(errorCallback);
				}

				instances++;

				for (std::pair<int, int>& p : hints) {
					glfwWindowHint(p.first, p.second);
				}

				window = glfwCreateWindow(width, height, title, mon, manager ? manager->window : nullptr);

				if (window == nullptr) throw std::runtime_error("Failed to create GLFW window!");

				glfwLog.log("Created window");

				//Event systems
				init();

			}

			~GLFWManager(){

				std::lock_guard<std::recursive_mutex> guard(lock);

				glfwDestroyWindow(window);
				glfwLog.log("Destroyed window");

				instances--;

				if (instances == 0){
					glfwTerminate();
					glfwLog.log("Terminated");
				}

			}

			/**
			 * This is required for any use of glfw.
			 */
			static std::unique_lock<std::recursive_mutex> getLock() {
				return std::unique_lock<std::recursive_mutex>(lock);
			}

			void makeContextCurrent() {
				std::lock_guard<std::recursive_mutex> guard(lock);
				if (glfwGetCurrentContext() != window) {
					glfwMakeContextCurrent(window);
				}
			}

			void glfwFunction(std::function<void(GLFWwindow*)> func) {
				std::lock_guard<std::recursive_mutex> guard(lock);
				func(window);
			}

			/**
			 * Add a function to handle when the window gets resized.
			 */
			void onResize(std::function<void(int, int)> func) const;
			/**
			 * Add a function to handle when the framebuffer gets resized.
			 */
			void onFrameResize(std::function<void(int, int)> func) const;

		private:

			GLFWManager(){} //Default construction is not allowed

			static unsigned instances;

			static Log glfwLog;

			static void errorCallback(int code, const char* error) {
				glfwLog.error(error);
			}

			GLFWwindow* window = nullptr;

			static std::recursive_mutex lock;

			void init();

		};

	}
}

#endif
