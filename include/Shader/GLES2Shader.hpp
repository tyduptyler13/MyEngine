#ifndef MYUPLAY_MYENGINE_SHADERGLES2
#define MYUPLAY_MYENGINE_SHADERGLES2

#include "Shader/ShaderNode.hpp"
#include "Shader/Shader.hpp"

#include "Shader/ShaderLib.hpp"

#include "Vector2.hpp"

#include "GLES2/gl2.h"

#include <unordered_map>

namespace MyUPlay {
	namespace MyEngine {
		namespace Shader {

			struct GLES2Shader : public virtual Shader {

				std::unordered_map<std::string, GLint> pos;
				GLuint program = 0;

				virtual ~GLES2Shader(){
					if (program != 0){
						glDeleteProgram(program);
					}
				}

				GLES2Shader(){}

				void compile() override;

				inline void bind() override {
					glUseProgram(program);
				}

				GLint getAttribLoc(std::string name) {
					auto it = pos.find(name);
					if (it == pos.end()){
						GLint i = glGetAttribLocation(program, name.c_str());
						pos[name] = i;
						return i;
					} else {
						return it->second;
					}
				}

				GLint getUniformLoc(std::string name) {
					auto it = pos.find(name);
					if (it == pos.end()){
						GLint i = glGetUniformLocation(program, name.c_str());
						pos[name] = i;
						return i;
					} else {
						return it->second;
					}
				}

			};

			struct GLES2FlatShader : public GLES2Shader {

				IShaderNode* getVertexRoot() {
					return &vert;
				}

				IShaderNode* getFragmentRoot() {
					return &frag;
				}

				void prepare(Camera<float>*, Mesh<float>*, const std::vector<Light<float>*>&){
					if (this->dirty) compile();
				}
				void prepare(GLuint tex);

				GLES2FlatShader(){}

			private:

				struct : public IShaderNode {
					std::string getStatic() const {
						return "const vec2 madd=vec2(0.5,0.5);\n"
								"attribute vec2 vertex;\n"
								"varying vec2 texCoord;\n";
					}
					std::string getInstance() const {
						return "texCoord = vertex*madd+madd;\n"
								"texCoord.t = 1.0 - texCoord.t;\n"
								"gl_Position = vec4(vertex,0.0,1.0);\n";
					}
				} vert;

				struct : public IShaderNode {
					std::string getStatic() const {
						return "varying vec2 texCoord;\n"
								"uniform sampler2D tex;\n";
					}
					std::string getInstance() const {
						return "gl_FragColor = texture2D(tex, texCoord);\n";
								//"gl_FragColor = vec4(1.0,0.0,0.0,0.5);\n";
					}
				} frag;

			};

			/**
			 * This is a GLES2 Specialization of the VertexBase class.
			 */
			struct GLES2Vertex : public VertexBase {

				GLES2Shader* const shader;

				std::unordered_map<std::string, GLint> attribLocs;

				Output<Vector3f> position; //Vertex position (local)
				Output<Vector3f> normal; //Vertex normal (local normalized)

				Input<Vector4f> gl_Position; //Window coords

				void prepare(Camera<float>* camera, Mesh<float>* object, const std::vector<Light<float>*>& lights) override;

				std::string getStatic() const override {
					return "attribute vec3 position;\n"
							"attribute vec3 normal;\n\n"
							"uniform mat4 modelView;\n"
							"uniform mat4 projectionMatrix;\n"
							"uniform mat4 modelMatrix;\n"
							"uniform mat4 viewMatrix;\n"
							"uniform vec3 cameraPosition;\n\n"
							"varying vec3 fPosition;\n"
							"varying vec3 fNormal;\n\n";
				}

				std::string getInstance() const override {
					return "fPosition = " + fPosition.output->name + ";\n"
							"fNormal = " + fNormal.output->name + ";\n"
							"gl_Position = " + gl_Position.output->name + ";\n";
				}

				void traverseChildren(ShaderTraverser s) const override {
					s(fPosition.node);
					s(fNormal.node);
					s(gl_Position.node);
				}

				GLES2Vertex(GLES2Shader* const shader) : VertexBase(), shader(shader), position("position"), normal("normal") {

					//Default nodes. Using set again will just erase these nodes.

					std::shared_ptr<TransformDirection> transformDirection = std::make_shared<TransformDirection>();

					transformDirection->dir.set(&normal);
					transformDirection->matrix.set(&modelView);

					fNormal.set(transformDirection, &transformDirection->ret);

					std::shared_ptr<TransformLocation> transformLocation = std::make_shared<TransformLocation>();

					transformLocation->loc.set(&position);
					transformLocation->matrix.set(&modelView);

					fPosition.set(transformLocation, &transformLocation->ret);

					std::shared_ptr<Combine<Vector3f, float, Vector4f>> combined = std::make_shared<Combine<Vector3f, float, Vector4f>>();
					std::shared_ptr<InputVariable<float>> one = std::make_shared<InputVariable<float>>(1); //Const value of 1

					combined->a.set(transformLocation, &transformLocation->ret);
					combined->b.set(one, &one->out);

					std::shared_ptr<Multiply<Matrix4f, Vector4f, Vector4f>> multiply = std::make_shared<Multiply<Matrix4f, Vector4f, Vector4f>>();

					multiply->a.set(&projectionMatrix);
					multiply->b.set(combined, &combined->ret);

					gl_Position.set(multiply, &multiply->ret);

				}

			};

			struct GLES2Fragment : public FragmentBase {

				GLES2Shader* const shader;

				Output<Vector4f> gl_FragCoord;
				Output<bool> gl_FrontFacing;

				void prepare(Camera<float>* camera, Mesh<float>* object, const std::vector<Light<float>*>& lights) override;

				std::string getStatic() const override {
					return "varying vec3 fPosition;\n"
							"varying vec3 fNormal;\n\n";
				}

				std::string getInstance() const override {
					return "gl_FragColor = vec4(" + color.output->name + ", " + alpha.output->name + ");";
				}

				/**
				 * By default we output the normal shader
				 */
				GLES2Fragment(GLES2Shader* const shader) : FragmentBase(), shader(shader), gl_FragCoord("gl_FragCoord"), gl_FrontFacing("gl_FrontFacing") {
					std::shared_ptr<InputVariable<float>> a = std::make_shared<InputVariable<float>>(1);
					alpha.set(a, &a->out);

					color.set(&normal);
				}

				void traverseChildren(ShaderTraverser s) const override {
					s(alpha.node);
					s(color.node);
				}

			};

			struct GLES2ForwardShader : public ForwardShader, public GLES2Shader {

				GLES2ForwardShader() : GLES2Shader() {
					vertexShaderRoot = std::make_unique<GLES2Vertex>(this);
					fragmentShaderRoot = std::make_unique<GLES2Fragment>(this);
				}

				void prepare(Camera<float>* camera, Mesh<float>* object, const std::vector<Light<float>*>& lights) override;

			};

		}
	}
}

#endif
