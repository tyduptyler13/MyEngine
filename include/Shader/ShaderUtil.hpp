/*
 * ShaderUtil.hpp
 *
 *  Created on: Jul 20, 2016
 *      Author: tyler
 */

#ifndef MYUPLAY_MYENGINE_SHADERUTIL
#define MYUPLAY_MYENGINE_SHADERUTIL

#include "Shader/ShaderNode.hpp"
#include "Shader/Attribute.hpp"

#include "Matrix4.hpp"
#include "Vector3.hpp"
#include "Color.hpp"
#include "DrawableObject3D.hpp"
#include "Light.hpp"

#include <string>

namespace MyUPlay {
	namespace MyEngine {
		namespace Shader {

			/**
			 * R - Renderer
			 * T - Type for conversion to some shading language.
			 * @return A renderer specific type related to a c type.
			 */
			template <typename R, typename T>
			struct Utility {

				static const char* type;
				static const char* getType(){
					return type;
				}

			};

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

		}
	}
}


#endif
