/*
 * ShaderUtil.hpp
 *
 *  Created on: Jul 20, 2016
 *      Author: tyler
 */

#ifndef SHADER_SHADERUTIL
#define SHADER_SHADERUTIL

#include <string>

namespace MyUPlay {
	namespace MyEngine {
		namespace Shader {

			/**
			 * R - Renderer
			 * T - Type for conversion to some shading language.
			 * @return A renderer specific type related to a c type.
			 */
			template <typename R, typename T>
			struct Utility {

				static const char* type;
				static const char* getType(){
					return type;
				}

			};

			/**
			 * Creates a unique variable name. (Low chance of collision)
			 */
			std::string generateUniqueName();

		}
	}
}


#endif
