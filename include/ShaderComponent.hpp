#ifndef MYUPLAY_MYENGINE_SHADERCOMPONENT
#define MYUPLAY_MYENGINE_SHADERCOMPONENT

#include <vector>
#include <map>
#include <string>
#include <functional>
#include <memory>

namespace MyUPlay {

    namespace MyEngine {

        template <typename T>
        struct Input {
            std::string name;
            T value;
            int position = -1; //For setting the value.
            std::unique_ptr<std::function<T()> > update = NULL; //Override this if you want the value updated every frame
            //If this is null, it will always use whatever is in value.
        }

        class ShaderComponent {

            public:

                std::string name;
                ~ShaderComponent(){}

                bool optimize = true;
                bool debug = false;

                std::vector<std::string> extensions; //Required extensions
                /**
                 * Only add an input here if code MUST set its value before the shader is run.
                 * If it is a varying type variable then don't put it here, glsl will handle it.
                 */
                std::vector<Input> inputs; //Input variables.
                std::vector<Input> outputs; //Output of this function. (only used in node graph)

                std::string code;

                virtual void compile();

        };

    }

}


#endif
