#ifndef MYUPLAY_MYENGINE_SHADER
#define MYUPLAY_MYENGINE_SHADER

#include <vector>
#include <memory>

#include "Shader/Attribute.hpp"
#include "Shader/ShaderNode.hpp"
#include "Math.hpp"

namespace MyUPlay {
	namespace MyEngine {
		namespace Shader {

			//This class is specialized based on which renderer you are using.
			template <typename R>
			class Shader {

			private:

				bool dirty = true;
				std::vector<IAttribute> attributes;

				void onChangeFunc(){
					dirty = true;
				}

			public:

				const Math::UUID uuid = Math::generateUUID();

				MasterShaderNode root;

				bool optimize = true;
				bool debug = false;

				Shader(){}
				virtual ~Shader(){}

				bool isDirty(){
					return dirty;
				}

				void compile();
				void bind();

			};

		}
	}
}


#endif
