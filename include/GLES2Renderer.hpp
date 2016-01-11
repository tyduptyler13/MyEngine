#ifndef MYUPLAY_MYENGINE_GLES2RENDERER
#define MYUPLAY_MYENGINE_GLES2RENDERER

#include <string>
#include <stdexcept>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengles2.h>

#include "Renderer.hpp"
#include "Log.hpp"
#include "Frustrum.hpp"
#include "Camera.hpp"
#include "Vector3.hpp"
#include "Matrix4.hpp"

namespace MyUPlay {

	namespace MyEngine {

		template <typename T = float>
		class GLES2Renderer : public Renderer<T> {

		private:

			SDL_Window* window;
			SDL_GLContext* gl;
			Log& log = Log::getInstance();

			GLuint mCurrentProgram,
			       mCurrentFramebuffer;
			int mCurrentMaterialId = -1;
			//TODO mCurrentGeometryProgram
			int mCurrentCamera -1;

			unsigned mUsedTextureUnits = 0;

			unsigned mCurrentWidth = 0,
				 mCurrentHeight = 0;

			Frustum<T> mFrustum;

			Matrix4f mProjScreenMatrix;

			Vector3f mVector3;

			Vector3f direction;

			bool lightsNeedUpdate = true;

			struct {
				//TODO
			} mLights;

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

				window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);

				if (!window) {
					log << "Failed to create a window!";
					throw new std::runtime_error("Could not create window.");
				}

				gl = SDL_GL_CreateContext(window);

				if (!gl){
					log << "Failed to load opengl!";
					throw new std::runtime_error("Could not get opengl handle.");
				}

				if (GLEW_OK != glewInit()){
					log << "Failed to load glew!";
				}

				if (!GLEW_ARB_texture_float){
					log << "Float textures are not supported!";
				}

				if (!GLEW_

				//TODO capabilities, state, properties, objects, programcache?
				//
				//buffer renderer, indexedBufferRenderer?


			}

			~GLES2Renderer(){
				SDL_GL_DeleteContext(gl);
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

			void init() override;

			float getMaxAnisotropy(){
				float aniso = 0;
				glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &aniso);
				return aniso;
			}

			float getPixelRatio() const {
				return mPixelRatio;
			}

			void setPixelRatio(float p){
				mPixelRatio = p;
			}

			/**
			 * Returns width and height in a std::tuple
			 */
			std::tuple<unsigned, unsigned> getSize() override {
				return std::make_tuple(width, height);
			}

			void setSize(unsigned width, unsigned height) override {
				this->width = width;
				this->height = height;

				SDL_SetWindowSize(window, width, height);

				setViewport(0,0,width,height);
			}

		};

	}

}

#endif

