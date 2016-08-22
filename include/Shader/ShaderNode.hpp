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
			 * All shaders are specialized based on the renderer.
			 */
			template <class R>
			class MasterShaderNode : IShaderNode {
			public:

				//Per vertex
				Input<Vector4> position;

				//Per fragment
				Input<Vector3> normal;
				Input<float> reflection; //Default value will be a const value of 0.
				Input<Color> color;
				Input<float> alpha;
				Input<float> specular;

				//This can be applied in post shading or forward shading
				Input<Color> lightColor; //The light component and brightness.

				traverseInputs(std::function<void(std::shared_ptr<IShaderNode>)> func) const override {
					func(position.output->node);
					func(reflection.output->node);
					func(color.output->node);
					func(alpha.output->node);
					func(specular.output->node);
					func(lightColor.output->node);
				}

				std::string getStatic() const override;

				std::String getInstance() const override;

			};

		}
	}
}


#endif
