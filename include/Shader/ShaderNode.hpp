#ifndef MYUPLAY_MYENGINE_SHADERNODE
#define MYUPLAY_MYENGINE_SHADERNODE

#include <vector>
#include <string>
#include <functional>
#include <memory>

#include "Math.hpp"
#include "Vector4.hpp"
#include "Vector3.hpp"
#include "Color.hpp"

namespace MyUPlay {
	namespace MyEngine {

		namespace Shader {

			//Forward declared.
			struct Output;
			struct Input;

			class IShaderNode {

			public:

				/**
				 * When declaring a shader node, it must be known at what level
				 * this code will run so it can be optimized.
				 *
				 * If a node depends on code that runs at a lower level, then that
				 * node will be promoted to run at that lower level, losing performance.
				 */
				enum Scope {
					PerPrimative, //Uniform
					PerVertex, //Attribute
					PerFragment //Varying
				};

			protected:

				IShaderNode(Scope s, std::string uniqueName = generateUniqueName()) : uniqueName(uniqueName), scope(scope) {}

				const std::string uniqueName; //Used for static naming.

			public:

				Scope scope;

				static std::string generateUniqueName();

				Math::UUID uuid = Math::generateUUID();

				/**
				 * This allows the compiler to generate all the code required for
				 * a shader.
				 */
				virtual void traverseInputs(std::function<void(const Input&)> func) const = 0;

				/**
				 * The following function returns the static code related to the
				 * function. (Method body, etc) This can be empty.
				 */
				virtual std::string getStatic() const {
					return ""; //Default is to return nothing.
				};

				/**
				 * This function is called when defining an actual call to the
				 * function or code. (Method call)
				 */
				virtual std::string getInstance() const = 0; //All calls/refs must be defined. If this returns nothing then the node is useless.

			};

			struct Output {
				std::vector<std::weak_ptr<IShaderNode>> inputs; //The inputs the output is connected to.
				const std::string type; //String version of the type, used in validation and code generation.
				const std::string name; //The name of the output variable (as it is in code), this is important!
				Output(std::string type, std::string name) : type(type), name(name) {}
			};

			struct Input {
				std::shared_ptr<IShaderNode> node; //Hold the node in memory.
				Output* output = NULL; //The output the input connects to.
			};

			/**
			 * This class is the base class for all renderers shaders.
			 */
			class IMasterShaderNode : IShaderNode {
			public:

				//Per vertex
				Input<Vector4<float> > position;

				//Per fragment
				Input<Vector3<float> > normal;
				Input<float> reflection; //Default value will be a const value of 0.
				Input<Color> color;
				Input<float> alpha;
				Input<float> specular;

				//This can be applied in post shading or forward shading
				Input<Color> lightColor; //The light component and brightness.

				void traverseInputs(std::function<void(const Input&)> func) const override {
					func(position);
					func(reflection);
					func(color);
					func(alpha);
					func(specular);
					func(lightColor);
				}

				virtual std::string getStatic() const = 0;

				std::string getInstance() const override {
					return ""; //The master node will use main as its function which has no instances.
				}

				/**
				 * TODO Decide if I want to give rendering to shading or keep it
				 * inside the renderers.
				 *
				 * This function is meant to be the final render step. It will
				 * be customized by various implementations of this class.
				 * This method is meant to take in all the attributes of an object
				 * and pass them into the renderer as needed. It will also compile
				 * the shader itself if it has not already done so.
				 */
				virtual void render(std::shared_ptr<Geometry<float>>) = 0;

			};

		}
	}
}


#endif
