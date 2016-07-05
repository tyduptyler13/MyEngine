#ifndef MYUPLAY_MYENGINE_SHADER
#define MYUPLAY_MYENGINE_SHADER

#include <vector>
#include <memory>

#include "ShaderComponent.hpp"

namespace MyUPlay {

    namespace MyEngine {

        class Shader {

            private:

                bool dirty = true;
                std::shared_ptr<ShaderComponent> vertexShader;
                std::shared_ptr<ShaderComponent> fragmentShader;

                void onChangeFunc(){
                    dirty = true;
                }

            public:

                void setVertexShader(std::shared_ptr<ShaderComponent> shader){
                    vertexShader = shader;
                    shader->setOnChange(onChangeFunc);
                }

                void setFragmentShader(std::shared_ptr<ShaderComponent> shader){
                    fragmentShader = shader;
                    shader->setOnChange(onChangeFunc);
                }

                bool isDirty(){
                    return dirty;
                }

                virtual void bind() = 0;

        };

    }

}


#endif
