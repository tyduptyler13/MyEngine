#ifndef MYUPLAY_MYENGINE_SHADERGLES2
#define MYUPLAY_MYENGINE_SHADERGLES2

#include "Shader/ShaderNode.hpp"
#include "Shader/Shader.hpp"

#include "Shader/ShaderLib.hpp"

#include "GLES2/gl2.h"

namespace MyUPlay {
	namespace MyEngine {
		namespace Shader {

			/**
			 * This is a GLES2 Specialization of the VertexBase class.
			 */
			struct GLES2Vertex : public VertexBase {

				Output<Vector3f> position; //Vertex position (local)
				Output<Vector3f> normal; //Vertex normal (local normalized)

				Input<Vector4f> gl_Position; //Window coords

				void prepare(std::shared_ptr<Camera<float>> camera, std::shared_ptr<DrawableObject3D<float>> object, std::vector<Light<float>>& lights) override;

				std::string getStatic() const override {
					return "attribute vec3 position\n"
							"attribute vec3 normal\n\n"
							"uniform mat4 modelView;\n"
							"uniform mat4 projectionMatrix;\n"
							"uniform mat4 modelMatrix;\n"
							"uniform mat4 viewMatrix;\n"
							"uniform vec3 cameraPosition;\n\n"
							"varying vec3 fPosition;\n";
							"varying vec3 fNormal;\n\n";
				}

				std::string getInstance() const override {
					return "fPosition = " + fPosition.output->name + ";\n"
							"fNormal = " + fNormal.output->name + ";\n"
							"gl_Position = " + gl_Position.output->name + ";\n";
				}

				void traverseChildren(ShaderTraverser s) override {
					s(fPosition.node);
					s(fNormal.node);
					s(gl_Position);
				}

				GLES2Vertex() : VertexBase(), position("position"), normal("normal") {

					//Default nodes. Using set again will just erase these nodes.

					std::shared_ptr<TransformDirection> transformDirection = new TransformDirection();

					transformDirection->dir.set(&normal);
					transformDirection->matrix.set(&modelView);

					fNormal.set(transformDirection, &transformDirection->ret);

					std::shared_ptr<TransformLocation> transformLocation = new TransformLocation();

					transformLocation->loc.set(&position);
					transformLocation->matrix.set(&modelView);

					fPosition.set(transformLocation, &transformLocation->ret);

					std::shared_ptr<Combine<Vector3f, float, Vector4f>> combined = new Combine<Vector3f, float, Vector4f>();
					std::shared_ptr<InputVariable<float>> zero = new InputVariable<float>(0); //A float value of zero.

					combined->a.set(transformLocation, &transformLocation->ret);
					combined->b.set(zero, &zero->out);

					std::shared_ptr<Multiply<Matrix4f, Vector4f, Vector4f>> multiply = new Multiply<Matrix4f, Vector4f, Vector4f>();

					multiply->a.set(&projectionMatrix);
					multiply->b.set(combined, &combined->ret);

					gl_Position.set(multiply, &multiply->ret);

				}

			};

			struct GLES2Fragment : public FragmentBase {

				Output<Vector4f> gl_FragCoord;
				Output<bool> gl_FrontFacing;

				void prepare(std::shared_ptr<Camera<float>> camera, std::shared_ptr<DrawableObject3D<float>> object, std::vector<Light<float>>& lights) override;

				std::string getStatic() const override {
					return "varying vec3 fPosition;\n"
							"varying vec3 fNormal;\n\n";
				}

				std::string getInstance() const override {
					return "gl_FragColor = vec4(" + color.output->name + ", " + alpha.output->name + ");";
				}

				GLES2Fragment() : FragmentBase(), gl_FragCoord("gl_FragCoord"), gl_FrontFacing("gl_FrontFacing") {
					//TODO setup default nodes
				}

			};

			struct GLES2Shader : public virtual Shader {

				GLuint program = 0;

				~GLES2Shader(){
					glDeleteProgram(program);
				}

				GLES2Shader() {
					vertexShaderRoot = new GLES2Vertex();
				}

				void bind() override {
					glUseProgram(program);
				}
			};

			struct ForwardShaderGLES2 : public ForwardShader, public GLES2Shader {

				ForwardShaderGLES2(){
					fragmentShaderRoot = new GLES2Fragment();
				}
				~ForwardShaderGLES2();

				void compile() override;

				void render(std::shared_ptr<Camera<float>> camera,
						std::shared_ptr<DrawableObject3D<float>> object,
						const std::vector<Light<float>>& lights) override;

			};

		}
	}
}

#endif
