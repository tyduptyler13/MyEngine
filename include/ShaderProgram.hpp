#ifndef MYUPLAY_MYENGINE_SHADERPROGRAM
#define MYUPLAY_MYENGINE_SHADERPROGRAM

#include <string>

namespace MyUPlay {

	namespace MyEngine {

		class ShaderProgram {

			private:

				static int programIdCount;

			public:

				std::string code;
				int usedTimes = 1;
				int id = programIdCount++;

		};

		int ShaderProgram::programIdCount = 0;

	}

}

#endif

