#ifndef MYUPLAY_MYENGINE_SHADERNODE
#define MYUPLAY_MYENGINE_SHADERNODE

#include <vector>
#include <string>
#include <functional>

#include "Math.hpp"

namespace MyUPlay {
	namespace MyEngine {

		class GLES2Renderer;

		namespace Shader {

			class IShaderNode {

			protected:

				static const std::string uniqueName; //Used for static naming.

			public:

				static std::string generateUniqueName();

				/**
				 * This allows the compiler to generate all the code required for
				 * a shader.
				 */
				virtual traverseInputs(std::function<void(std::shared_ptr<IShaderNode>)> func) const = 0;

				/**
				 * The following function returns the static code related to the
				 * function.
				 */
				virtual std::string getStatic() const = 0;

				/**
				 * This function is called when defining an actual call to the
				 * function or code.
				 */
				virtual std::string getInstance() const = 0;

			};

			/**
			 * All shaders are specialized based on the renderer.
			 */
			template <class R>
			class ShaderNode : IShaderNode {
			public:



			};

		}
	}
}


#endif
