
#include "GLES2Renderer.hpp"

namespace MyUPlay {
  namespace MyEngine {

    struct GL32Renderer : public GLES2Renderer {

      /**
       * Antialias should be set to a value 0 (off), or 2-4 for varying samples (on).
       */
      GL32Renderer(unsigned antialias = 0, GLFWmonitor* monitor = nullptr, GL32Renderer* share = nullptr);
      virtual ~GL32Renderer();

    };

  }
}
