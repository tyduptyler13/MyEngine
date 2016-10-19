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

namespace MyUPlay {
	namespace MyEngine {

		namespace Shader {

			/**
			 * All of the following are templated to do static type restriction.
			 * This keeps all shaders specific to their renderer.
			 */

			//Forward declared.
			template <typename T>
			struct Output;
			template <typename T>
			struct Input;

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

				/**
				 * This needs to be implemented for every shader node in order to dynamically build the shader list.
				 */

				typedef std::function<void(std::shared_ptr<IShaderNode>)> ShaderTraverser;

				virtual void traverseChildren(ShaderTraverser){
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

			struct IAttribute;

			/**
			 * Root shaders are special shader nodes that can provide both inputs and outputs.
			 * These nodes are where the compiler will start and usually end.
			 */
			struct IRootShaderNode : public IShaderNode {

				std::vector<std::unique_ptr<IAttribute>> customAttributes;

				/**
				 * This function must be overriden by each root node and by every renderer to supply
				 * the shader with the required attributes that will ALWAYS be part of the code.
				 * Should the attribute get optimized out because it wasn't used, only then can
				 * it be skipped. In opengl this is done when the position is not present in the shader.
				 */
				virtual void prepare(std::shared_ptr<Camera<float>> camera, std::shared_ptr<DrawableObject3D<float>> object, std::vector<Light<float>>& lights) = 0;

			};

			/**
			 * This node is where the core vertex shader components attach.
			 */
			struct VertexBase : public IRootShaderNode {

				Output<Matrix4f> modelView;
				Output<Matrix4f> projectionMatrix;
				Output<Matrix4f> modelMatrix; //Matrix world
				Output<Matrix4f> viewMatrix; //Inverse matrix world
				Output<Vector3f> cameraPosition;

				Input<Vector3f> position;
				Input<Vector3f> normal; //Output the normalized normal vector for use in the fragment shader.

				void traverseChildren(ShaderTraverser s) override {
					s(position.node);
					s(normal.node);
				}

			};

			struct FragmentBase : public IRootShaderNode {
				Output<Vector3f> position; //Interpolated position from vertex shader
				Output<Vector3f> normal; //Interpolated normal from vertex shader (normalized)

				Input<Color> color; //gl_color;

				void traverseChildren(ShaderTraverser s) override {
					s(color.node);
				}
			};

			struct DeferredFragment : public IRootShaderNode {
				Output<Vector3f> vposition; // Position from vertex shader
				Output<Vector3f> vnormal; //Normal from vertex shader.

				Input<Vector3f> position; //Camera space position
				Input<Vector3f> normal; //Camera space normal
				Input<Color> color;
				Input<Color> emission;

				void traverseChildren(ShaderTraverser s) override {
					s(position.node);
					s(normal.node);
					s(color.node);
					s(emission.node);
				}
			};

			struct DeferredLightingPass : public IRootShaderNode {
				Output<Vector3f> position;
				Output<Vector3f> normal;
				Output<Color> objColor;
				Output<Color> objEmission;

				Input<Color> finalColor;

				void traverseChildren(ShaderTraverser s) override {
					s(finalColor.node);
				}
			};

			struct Shader {

				virtual ~Shader(){}

				bool dirty = true;

				virtual void compile() = 0;

				virtual void bind() = 0;

				virtual void render(std::shared_ptr<Camera<float>> camera, std::shared_ptr<DrawableObject3D<float>> o, const std::vector<Light<float>>& lights) = 0;

				std::unique_ptr<VertexBase> vertexShader;

			};

			struct ForwardShader : public Shader {

				std::unique_ptr<FragmentBase> fragmentShader;

			};

			struct DeferredShader : public Shader {
				std::unique_ptr<DeferredFragment> frag;
				std::unique_ptr<DeferredLightingPass> lighting;
			};

		}
	}
}


#endif
