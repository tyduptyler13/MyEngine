#ifndef MYUPLAY_MYENGINE_SHADERNODE
#define MYUPLAY_MYENGINE_SHADERNODE

#include <vector>
#include <string>
#include <functional>
#include <memory>

#include "Math.hpp"

namespace MyUPlay {
	namespace MyEngine {

		namespace Shader {

			/**
			 * Creates a unique variable name. (Low chance of collision)
			 */
			std::string generateUniqueName();

			struct IShaderNode {

				virtual ~IShaderNode(){}

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


				typedef std::function<void(std::shared_ptr<IShaderNode>)> ShaderTraverser;

				/**
				 * This needs to be implemented for every shader node in order to dynamically build the shader list.
				 */
				virtual void traverseChildren(ShaderTraverser) {
					return; //No children by default.
				}

			};

			template <typename T> //Internal type
			struct Output {
				std::vector<std::weak_ptr<IShaderNode>> inputs; //The inputs the output is connected to.
				const std::string name; //The name of the output variable (as it is in code), this is important!
				//It is likely that this gets set by something further down the line in the code, like when a
				//variable is initially declared, then it is passed down through the call chain and reused.
				//If a variable is copied vs referenced, a new name would be used from either a pre-declaration
				//or is internally declared and returned. (out vs inout in glsl)
				Output(std::string name = generateUniqueName()) : name(name) {}
			};

			template <typename T> //Renderer, internal type
			struct Input {
				std::shared_ptr<IShaderNode> node; //Hold the node in memory.
				Output<T>* output = NULL; //The output the input connects to.
				//We use a pointer because we don't know which output otherwise,
				//and we don't own its memory so a shared_ptr doesn't work.

				/**
				 * We need self because we don't have the authority to hold our own shared_ptr,
				 * we need the output node because output doesn't have reference to it,
				 * and we need the output pointer because there is no way to tell which output in the node we want.
				 */
				void set(std::shared_ptr<IShaderNode> self, std::shared_ptr<IShaderNode> outNode, Output<T>* out){
					if (node){ //Remove from previous node.
						unset(self);
					}
					node = outNode;
					output = out;
					out->inputs.push_back(std::weak_ptr<IShaderNode>(self));
				}

				/**
				 * We once again need to know our self pointer.
				 */
				void unset(std::shared_ptr<IShaderNode> self){
					if (node.use_count() > 1){ //If we are not the only owner, actually clean up, otherwise it will get deleted anyways.
						for (auto it = output->inputs.begin(); it != output->inputs.end(); it++){
							if (it->lock() == self){
								output->inputs.erase(it);
							}
						}
					}
					node.reset();
				}

			};

		}
	}
}


#endif
