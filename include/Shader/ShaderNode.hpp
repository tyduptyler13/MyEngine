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
				virtual void traverseInputs(std::function<void(std::shared_ptr<IShaderNode>)> func) const = 0;

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

			template <typename T>
			struct Input;

			template <typename T>
			struct Output {
				const std::shared_ptr<IShaderNode> node; //The node the output is connected to.
				std::vector<std::weak_ptr<IShaderNode>> inputs; //The inputs the output is connected to.
				Output(std::shared_ptr<IShaderNode> node) : node(node) {}
			};

			template <typename T>
			struct Input {
				std::shared_ptr<Output<T>> output; //The output the input connects to.
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

				void traverseInputs(std::function<void(std::shared_ptr<IShaderNode>)> func) const override {
					func(position.output->node);
					func(reflection.output->node);
					func(color.output->node);
					func(alpha.output->node);
					func(specular.output->node);
					func(lightColor.output->node);
				}

				virtual std::string getStatic() const = 0;

				std::string getInstance() const override {
					return ""; //The master node will use main as its function which has no instances.
				}

			};

		}
	}
}


#endif
