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

#include "Camera.hpp"
#include "DrawableObject3D.hpp"

#include "Shader/ShaderUtil.hpp"
#include "Shader/Attribute.hpp"

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
					if (node){ //Remove from previous node.
						unset(self);
					}
					node = outNode;
					output = out;
					out->inputs.push_back(std::weak_ptr<IShaderNode<R>>(self));
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

				virtual ~Shader(){}

				bool dirty = true;

				virtual void compile() = 0;

				virtual void render(std::shared_ptr<Camera<float>> camera, std::shared_ptr<DrawableObject3D<float>> o) = 0;

				//A list of attributes to check if they are dirty before rendering.
				std::vector<std::weak_ptr<IAttribute<R>>> attributes;

			};

			/**
			 * This shader is the final component in the first stage of shading.
			 * It will collect
			 */
			template <class R>
			struct DeferredShaderS1 : public Shader<R> {

				Input<R, Vector4<float> > position;
				Input<R, Vector3<float> > normal;
				Input<R, Color> color; //Vec3
				Input<R, float> alpha;
				//Color and alpha are combined in the last step with gl_fragColor = vec4(color, alpha); -- GLES2Renderer

			};

			template <class R>
			struct DeferredShaderS2 : public Shader<R> {

				/**
				 * This is the output node after all shading is complete
				 */
				Input<R, Color> finalColor;

			};

			template <class R>
			struct ShaderStages {
				DeferredShaderS1<R> stage1;
				DeferredShaderS2<R> stage2;
			};

			/**
			 * This is a class that will always be available in the node structure.
			 * It contains outputs for the most common attribute inputs to a shader
			 * and will only be pruned in the shader code itself.
			 */
			template <class R>
			struct StandardInputs : public IShaderNode<R> {

				Attribute<R, Vector3f> normal;
				Attribute<R, Vector3f> position;
				Attribute<R, Matrix4f> modelViewMatrix; //Camera.matrixWorldInverse * object.matrixWorld
				Attribute<R, Matrix4f> projectionMatrix; //camera.projectionMatrix;
				Attribute<R, Matrix4f> modelMatrix; //object.matrixWorld
				Attribute<R, Matrix4f> viewMatrix; //Camera.matrixWorldInverse
				Attribute<R, Vector3f> cameraPosition;

				StandardInputs() :
					normal("normal", ShaderScope::PerVertex, Vector3f()),
					position("position", ShaderScope::PerVertex, Vector3f()),
					modelViewMatrix("modelViewMatrix", ShaderScope::PerPrimitive, Matrix4f()),
					projectionMatrix("projectionMatrix", ShaderScope::PerPrimitive, Matrix4f()),
					modelMatrix("modelMatrix", ShaderScope::PerPrimitive, Matrix4f()),
					viewMatrix("viewMatrix", ShaderScope::PerPrimitive, Matrix4f()),
					cameraPosition("cameraPosition", ShaderScope::PerPrimitive, Vector3f())
				{}

			};

		}
	}
}


#endif
