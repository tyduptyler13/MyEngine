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

			/**
			 * All of the following are templated to do static type restriction.
			 * This keeps all shaders specific to their renderer.
			 */

			//Forward declared.
			template <class R, typename T>
			struct Output;
			template <class R, typename T>
			struct Input;

			/**
			 * When declaring a shader node, it must be known at what level
			 * this code will run so it can be optimized.
			 *
			 * If a node depends on code that runs at a lower level, then that
			 * node will be promoted to run at that lower level, losing performance.
			 */
			enum ShaderScope {
				PerPrimative, //Uniform
				PerVertex, //Attribute
				PerFragment //Varying
			};

			template <class R> //Renderer
			class IShaderNode {
			protected:

				IShaderNode(ShaderScope s, std::string uniqueName = generateUniqueName()) : uniqueName(uniqueName), scope(scope) {}
				IShaderNode(const IShaderNode& node) //Copy
				: uniqueName(node.uniqueName), scope(node.scope){}
				IShaderNode(IShaderNode&& node) //Move
				: uniqueName(node.uniqueName), scope(node.scope), uuid(node.uuid){}

				virtual ~IShaderNode(){}

				const std::string uniqueName; //Used for static variables and functions, can be overridden.

			public:

				ShaderScope scope;

				static std::string generateUniqueName();

				Math::UUID uuid = Math::generateUUID();

				/**
				 * The following function returns the static code related to the
				 * function. (Method body, etc) This can be empty. Static will be
				 * generated once per shader compile per class involved.
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

			template <class R, typename T> //Renderer, internal type
			struct Output {
				std::vector<std::weak_ptr<IShaderNode<R>>> inputs; //The inputs the output is connected to.
				const std::string name; //The name of the output variable (as it is in code), this is important!
				//It is likely that this gets set by something further down the line in the code, like when a
				//variable is initially declared, then it is passed down through the call chain and reused.
				//If a variable is copied vs referenced, a new name would be used from either a pre-declaration
				//or is internally declared and returned. (out vs inout in glsl)
				Output(std::string name) : name(name) {}
			};

			template <class R, typename T> //Renderer, internal type
			struct Input {
				std::shared_ptr<IShaderNode<R>> node; //Hold the node in memory.
				Output<R, T>* output = NULL; //The output the input connects to.
			};

			/**
			 * This shader node is used in all renderers and is the core to
			 * creating dynamically compiled shaders.
			 */
			template <class R>
			class IMasterShaderNode : public IShaderNode<R> {
			public:

				//Per vertex
				Input<R, Vector4<float> > position;

				//Per fragment
				Input<R, Vector3<float> > normal;
				Input<R, float> reflection; //Default value will be a const value of 0.
				Input<R, Color> color;
				Input<R, float> alpha;
				Input<R, float> specular;

				//This can be applied in post shading or forward shading
				Input<R, Color> lightColor; //The light component and brightness.

			};

		}
	}
}


#endif
