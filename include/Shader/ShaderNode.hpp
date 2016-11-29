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
				/*
				 * The following was to allow reverse traversal of the nodes without leaking memory. The
				 * flaw with it is that connecting inputs and outputs required LOTS of bending over backwards
				 * to please this design. Because of this, it has been removed.
				 */
				//std::vector<std::weak_ptr<IShaderNode>> inputs; //The inputs the output is connected to.

				/**
				 * This is the name of the variable in the code. All nodes will use this value to feed into the
				 * next node and then the variable is done. We leave it up to the compiler and optimizer (internal
				 * and the driver) to clean up after we make this mess. If this is a problem in the future, we
				 * will have to redesign how outputs work because without this model, the nodes can have sort of
				 * race conditions except that instead its just the alternate paths executing and using the same
				 * variables.
				 */
				const std::string name = generateUniqueName(); //The value is generated if not specified at creation time.
				Output(){}
				Output(std::string name) : name(name) {}
			};

			template <typename T> //Renderer, internal type
			struct Input {
				std::shared_ptr<IShaderNode> node; //Hold the node in memory.
				Output<T>* output = NULL; //The output the input connects to.
				//We use a pointer because we don't know which output otherwise,
				//and we don't own its memory so a shared_ptr doesn't work.

				/**
				 * This is the new design. It is less complicated as it doesn't require a reference to its
				 * own shared_ptr.
				 */
				void set(std::shared_ptr<IShaderNode> outNode, Output<T>* out){
					node = outNode;
					output = out;
					//Outputs no longer can track inputs!
				}

				/**
				 * This is the easiest way to set an input but can have problems if the
				 * type passed is not a derivative of IShaderNode!
				 */
				template <typename PT> //PT is expected to be a subtype of
				void set(std::shared_ptr<PT> outNode, Output<T>* out){
					node = std::static_pointer_cast<IShaderNode>(outNode);
					output = out;
				}

				/**
				 * This is the least likely to be used method.
				 *
				 * This version of set doesn't save a pointer to the sub node and thus
				 * it will be deleted automatically, except when that node doesn't exist
				 * and you are pointing to Outputs in for instance the base Vertex and Fragment
				 * classes. There is no node to point to in that instance.
				 */
				void set(Output<T>* out){
					output = out;
				}


			};

		}
	}
}


#endif
