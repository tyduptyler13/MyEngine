#ifndef MYUPLAY_MYENGINE_GLES2RENDERER
#define MYUPLAY_MYENGINE_GLES2RENDERER

#include <string>
#include <stdexcept>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengles2.h>

#include "Renderer.hpp"
#include "Log.hpp"

namespace MyUPlay {

	namespace MyEngine {

		template <typename T>
		class GLES2Renderer : public Renderer<T> {

		private:

			SDL_Window* window;
			SDL_GLContext* gl;
			Log& log = Log::getInstance();

			void setDefaultGLState(){
				//state?
				//TODO SDL set window size / set fullscreen size.
				glClearColor(this->mClearColor.r, this->mClearColor.g, this->mClearColor.b, this->mClearAlpha);
			}

			void resetGLState() {

			}

		public:

			GLES2Renderer(const std::string& title, unsigned width, unsigned height){

				SDL_Init(SDL_INIT_VIDEO);

				window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);

				if (!window) {
					log << "Failed to create a window!";
					throw new std::runtime_error("Could not create window.");
				}

				if (!gl){
					log << "Failed to load opengl!";
					throw new std::runtime_error("Could not get opengl handle.");
				}

				//TODO capabilities, state, properties, objects, programcache?
				//
				//buffer renderer, indexedBufferRenderer?


			}

			~GLES2Renderer(){
				SDL_GL_DeleteContext(gl);
			}

			void setSize(unsigned width, unsigned height){
				SDL_SetWindowSize(window, width, height);
			}

			void clearColor(){
				clearColor(mClearColor.r, mClearColor.g, mClearColor.b, 0);
			}

			void clearColor(short r, short g, short b, short a){
				if (this->premultipliedAlpha){
					r *= a; g *= a; b *= a;
				}

				glClearColor(r, g, b, a);
			}

		};

	}

}

#endif

