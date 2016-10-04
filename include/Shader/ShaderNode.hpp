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

#include "Shader/ShaderUtil.hpp"

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

			template <class R> //Renderer
			class IShaderNode {
			protected:

				IShaderNode(){}
				IShaderNode(const IShaderNode& node){}
				IShaderNode(IShaderNode&& node) //Move
				: uuid(node.uuid){}

				virtual ~IShaderNode(){}

				bool dirty = true;

				static void makeDirty(std::vector<std::weak_ptr<IShaderNode<R>>>& nodes){
					for (std::weak_ptr<IShaderNode<R>>& ptr : nodes){
						if (ptr.expired()) continue;
						auto node = ptr.lock();
						node.makeDirty();
					}
				}

			public:

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

				/** Recursively sets all of its parents to dirty. */
				virtual void makeDirty() = 0;

				bool isDirty(){
					return dirty;
				}

			};

			template <class R, typename T> //Renderer, internal type
			struct Output {
				std::vector<std::weak_ptr<IShaderNode<R>>> inputs; //The inputs the output is connected to.
				const std::string name; //The name of the output variable (as it is in code), this is important!
				//It is likely that this gets set by something further down the line in the code, like when a
				//variable is initially declared, then it is passed down through the call chain and reused.
				//If a variable is copied vs referenced, a new name would be used from either a pre-declaration
				//or is internally declared and returned. (out vs inout in glsl)
				Output(std::string name = generateUniqueName()) : name(name) {}
			};

			template <class R, typename T> //Renderer, internal type
			struct Input {
				std::shared_ptr<IShaderNode<R>> node; //Hold the node in memory.
				Output<R, T>* output = NULL; //The output the input connects to.
				//We use a pointer because we don't know which output otherwise,
				//and we don't own its memory so a shared_ptr doesn't work.

				/**
				 * We need self because we don't have the authority to hold our own shared_ptr,
				 * we need the output node because output doesn't have reference to it,
				 * and we need the output pointer because there is no way to tell which output in the node we want.
				 */
				void set(std::shared_ptr<IShaderNode<R>> self, std::shared_ptr<IShaderNode<R>> outNode, Output<R, T>* out){
					node = outNode;
					output = out;
					out->inputs.push_back(weak_ptr<IShaderNode<R>>(self));
				}

				/**
				 * We once again need to know our self pointer.
				 */
				void unset(std::shared_ptr<IShaderNode<R>> self){
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

			template <class R>
			struct Shader {

			};

			/**
			 * This shader node is used in all renderers and is the core to
			 * creating dynamically compiled shaders.
			 */
			template <class R>
			class DeferredShaderNode : public IShaderNode<R> {
			public:

				//Per vertex
				Input<R, Vector4<float> > position;

				//Per fragment
				Input<R, Vector3<float> > normal;
				Input<R, Color> color; //Vec3
				Input<R, float> alpha;
				//Color and alpha are combined in the last step with gl_fragColor = vec4(color, alpha); -- GLES2Renderer
				Input<R, Color> specular; //Specular color, black means no shine. Default white

				/**
				 * This is a special node, it should be the final value of the object.
				 * If the object gets run in post shading or forward shading, this value
				 * should be the same and ONLY should rely on the normal, color, position, and
				 * specular values. Reflection is a post process effect regardless and should
				 * be executed after the shadows are added.
				 */
				Input<R, Color> shadedColor; //The color after shadows have been applied
				//This includes specular lighting like glare, etc.

			};

			/**
			 * This is a class that will always be available in the node structure.
			 * It contains outputs for the most common attribute inputs to a shader
			 * and will only be pruned in the shader itself.
			 */
			template <class R>
			struct StandardInputs : public IShaderNode<R> {

				Attribute("")

			};

		}
	}
}


#endif
