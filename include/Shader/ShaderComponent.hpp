#ifndef MYUPLAY_MYENGINE_SHADERCOMPONENT
#define MYUPLAY_MYENGINE_SHADERCOMPONENT

#include <vector>
#include <string>

#include "Math.hpp"

namespace MyUPlay {
	namespace MyEngine {

		class GLES2Renderer;

		namespace Shader {

			struct Return {
				const std::string type;
				Return(std::string type) : type(type) {}
			};

			struct Output {
				const std::string type;
				const std::string name;
				Output(std::string type, std::string name) : type(type), name(name) {}
			};

			template <typename R>
			class ShaderComponent;

			template <typename R>
			struct Input {
				const std::string type;
				const bool required = true;
				std::shared_ptr<ShaderComponent<R>> source;
				Input(std::string type, bool required = true) : type(type), required(required) {}
			};

			template <typename R>
			class ShaderComponent {
			protected:
				ShaderComponent(std::vector<Input<R>> inputs, std::vector<Output> outputs, Output ret, std::string name = "CustomShaderComponent")
				: name(name), inputs(inputs), outputs(outputs), ret(ret) {}
			public:

				virtual ~ShaderComponent(){};

				const Math::UUID uuid = Math::generateUUID();
				std::string name;

				const std::vector<Input<R>> inputs;
				const std::vector<Output> outputs;
				const Return ret; //Type will be "void" if no return.

				virtual std::string getCode() const = 0;
			};

			/**
			 * This is a very basic shader Component that simply declares a variable.
			 */
			template <typename R>
			class NameComponent : ShaderComponent<R> {
			public:
				std::string value;

				//Create a shaderComponent with no inputs or outputs, a return value, and an optional initial value.
				NameComponent(std::string name, std::string type, std::string value = "")
				: ShaderComponent<R>({}, {}, Return(type), name), value(value){};

				std::string getCode() const override;
			};

			//Example implementation of NameComponent getCode for GLES2
			template <>
			std::string NameComponent<GLES2Renderer>::getCode() const {
				std::string r = ret.type + " " + name;
				if (value.size() > 0){
					r += " = " + value; //Add the initial value if it was specified.
				}
				r += ";"; //Terminate the statement.
				return r;
			}

		}
	}
}


#endif
