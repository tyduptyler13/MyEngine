#ifndef MYUPLAY_MYENGINE_SHADER
#define MYUPLAY_MYENGINE_SHADER

#include <vector>
#include <memory>

#include "Shader/Attribute.hpp"
#include "Shader/ShaderComponent.hpp"
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
				/**
				 * The following could be confusing but is intentional.
				 * We have lists of INPUTS because they are designed to accept the OUTPUTS of
				 * other shader components. This also allows us to keep the references to
				 * the other shaderComponents up the chain of calls.
				 * A better way to think of it: the inputs are for the next stage
				 * (fragment shader or drawing to the screen)
				 */
				std::vector<Input<R>> vertexOutputs; //Fragment shader inputs. (Don't have to be used)
				std::vector<Input<R>> fragmentOutputs; //Fragment shader output for final color
				void onChangeFunc(){
					dirty = true;
				}

			public:

				const Math::UUID uuid = Math::generateUUID();

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
