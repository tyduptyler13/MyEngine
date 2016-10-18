#ifndef MYUPLAY_MYENGINE_SHADERNODE
#define MYUPLAY_MYENGINE_SHADERNODE

#include <vector>
#include <string>
#include <functional>
#include <memory>
#include <unordered_map>

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


			template <class R, typename T, int size>
			struct ArrayBuffer : IShaderNode<R> {

				ArrayBuffer<R, T, size> buffer;
				Attribute<R, int> index;


				ArrayBuffer(std::string bufferName, std::string indexName)
				: buffer(bufferName), index(indexName) {}

			};

			/**
			 * This node is where the core vertex shader components attach.
			 */
			template <class R>
			struct VertexBase : public IShaderNode<R> {

				Output<R, Matrix4f> modelView;
				Output<R, Matrix4f> projectionMatrix;
				Output<R, Matrix4f> modelMatrix; //Matrix world
				Output<R, Matrix4f> viewMatrix; //Inverse matrix world
				Output<R, Vector3f> cameraPosition;

				Input<R, Vector3f> position;
				Input<R, Vector3f> normal; //Output the normalized normal vector for use in the fragment shader.

			};

			template <class R>
			struct FragmentBase : public IShaderNode<R> {
				Output<R, Vector3f> position; //Interpolated position from vertex shader
				Output<R, Vector3f> normal; //Interpolated normal from vertex shader (normalized)

				Input<R, Color> color; //gl_color;
			};

			template <class R>
			struct DeferredFragment : public IShaderNode<R> {
				Output<R, Vector3f> vposition; // Position from vertex shader
				Output<R, Vector3f> vnormal; //Normal from vertex shader.

				Input<R, Vector3f> position; //Camera space position
				Input<R, Vector3f> normal; //Camera space normal
				Input<R, Color> color;
				Input<R, Color> emission;
			};

			template <class R>
			struct DeferredLightingPass : public IShaderNode<R> {
				Output<R, Vector3f> position;
				Output<R, Vector3f> normal;
				Output<R, Color> objColor;
				Output<R, Color> objEmission;

				Input<R, Color> finalColor;
			};

			template <class R>
			struct Shader {

				virtual ~Shader(){}

				bool dirty = true;

				virtual void compile();

				virtual void bind();

				virtual void render(std::shared_ptr<Camera<float>> camera, std::shared_ptr<DrawableObject3D<float>> o, const std::vector<Light<float>>& lights);

				VertexBase<R> vertexShader;

			};

			template <class R>
			struct ForwardShader : public Shader<R> {

				FragmentBase<R> fragmentShader;

			};

			template <class R>
			struct DeferredShader : public Shader<R> {
				DeferredFragment<R> frag;
				DeferredLightingPass<R> lighting;
			};



		}
	}
}


#endif
