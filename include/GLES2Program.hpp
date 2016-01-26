#ifndef MYUPLAY_MYENGINE_GLES2Program
#define MYUPLAY_MYENGINE_GLES2Program

#include <vector>
#include <utility>
#include <string>

#include "GLES2Renderer.hpp"

namespace MyUPlay {

	namespace MyEngine {

		//NOTE: GLES2Renderer must be loaded and constructed before this code can work.

		class GLES2Program : public ShaderProgram {

			private:

				static std::string generateDefines(const std::vector<std::pair<std::string, std::string> >& defines);
				static std::vector<std::pair<std::string, GLint> > fetchUniformLocations(GLuint program);
				static std::vector<std::pair<std::string, GLint> > fetchAttributeLocations(GLuint program);

			public:

				GLES2Program(GLES2Renderer*);
				~GLES2Program();
				std::vector<std::pair<std::string, GLint> > getAttributes();
				std::vector<std::pair<std::string, GLint> > getUniforms();

				GLint program;
				//These get deleted in the constructor. Leftovers?
				//GLint vertexShader;
				//GLint fragmentShader;

		};

	}

}

#endif

