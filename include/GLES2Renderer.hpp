#ifndef MYUPLAY_MYENGINE_GLES2RENDERER
#define MYUPLAY_MYENGINE_GLES2RENDERER

#include <string>
#include <stdexcept>
#include <unordered_map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>

#include "Renderer.hpp"
#include "Log.hpp"
#include "Camera.hpp"
#include "Vector3.hpp"
#include "Matrix4.hpp"
#include "Frustum.hpp"

namespace MyUPlay {

	namespace MyEngine {

		class GLES2Renderer : public Renderer<float> {

			//Determine if this is needed.
			//friend class GLES2Program;

		private:

			SDL_Window* window;
			SDL_GLContext gl;
			Log& log = Log::getInstance();

			GLuint mCurrentProgram,
			       mCurrentFramebuffer;
			int mCurrentMaterialId = -1;
			GLint mCurrentGeometryProgram;
			int mCurrentCamera = -1;

			unsigned mUsedTextureUnits = 0;

			unsigned mCurrentWidth = 0,
				 mCurrentHeight = 0;

			std::unordered_map<GLenum, unsigned> capabilities; //0 false, 1+ true or other value.

			Frustum<float> mFrustum;

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
				clearColor(this->mClearColor.r, this->mClearColor.g, this->mClearColor.b, this->mClearAlpha);
			}

			void resetGLState() {

			}

			void enable(GLenum cap){
				if (capabilities[cap] == 0){ //If its disabled or just discovered.
					glEnable(cap);
					capabilities[cap] = 1; //Show its enabled
				}
			}

			void disable(GLenum cap){
				if (capabilities[cap] != 0){ //Is it already enabled?
					glDisable(cap);
					capabilities[cap] = 0; //Show its disabled.
				}
			}

			void setBlending(short blending, short blendEquation, short blendSrc, short blendDst,
					short blendEquationAlpha, short blendSrcAlpha, short blendDstAlpha);

			//TODO getCompressedTextureFormats
			
			void enableAttribute(uint8_t attribute);
			void enableAttributeAndDivisor(uint8_t attribute, uint8_t meshPerAttribute);
			void disableUnusedAttributes();
			void setDepthFunc(short depthFunc);
			void setDepthTest(bool);
			void setDepthWrite(bool);
			void setColorwrite(bool);
			void setFlipSided(bool);
			void setLineWidth(float width);
			void setPolygonOffset(bool, float factor, float units);
			void setScissorTest(bool);
			void activeTexture(unsigned short slot = 0);
			void bindTexture(GLenum type, GLuint texture);
			void reset();

		public:

			GLES2Renderer(const std::string& title, unsigned width, unsigned height){

				SDL_Init(SDL_INIT_VIDEO);

				SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
				SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
				SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

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

				//TODO capabilities, state, properties, objects, programcache?
				//
				//buffer renderer, indexedBufferRenderer?


			}

			~GLES2Renderer(){
				SDL_GL_DeleteContext(gl);
				SDL_DestroyWindow(window);
				SDL_Quit();
			}

			void clearColor(){
				clearColor(mClearColor.r, mClearColor.g, mClearColor.b, 0);
			}

			void clearColor(float r, float g, float b, float a){
				if (this->premultipliedAlpha){
					r *= a; g *= a; b *= a;
				}

				glClearColor(r, g, b, a);
			}

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
			virtual std::tuple<unsigned, unsigned> getSize() override {
				return std::make_tuple(width, height);
			}

			virtual void setSize(unsigned width, unsigned height) override {
				this->width = width;
				this->height = height;

				SDL_SetWindowSize(window, width, height);

				setViewport(0,0,width,height);
			}

			virtual void setViewport(int x, int y, unsigned widht, unsigned height) override {
				mViewportX = x * mPixelRatio;
				mViewportY = y * mPixelRatio;
				mViewportWidth = width * mPixelRatio;
				mViewportHeight = height * mPixelRatio;

				glViewport(mViewportX, mViewportY, mViewportWidth, mViewportHeight);
			}

			virtual std::tuple<int, int, unsigned, unsigned> getViewport() override {
				return std::make_tuple<int, int, unsigned, unsigned>(
						mViewportX / mPixelRatio,
						mViewportY / mPixelRatio,
						mViewportWidth / mPixelRatio,
						mViewportHeight / mPixelRatio
				);
			}

			virtual void setScissor(int x, int y, unsigned width, unsigned height) override {
				glScissor(
						x * mPixelRatio,
						y * mPixelRatio,
						width * mPixelRatio,
						height * mPixelRatio
				);
			}

			void enableScissorTest(bool value){
				
			}

		};

	}

}

#endif

