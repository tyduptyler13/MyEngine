#ifndef MYUPLAY_MYENGINE_OVERLAYSYSTEM
#define MYUPLAY_MYENGINE_OVERLAYSYSTEM

namespace MyUPlay {
	namespace MyEngine {

		template <class R>
		struct OverlaySystem {

			void updatePixelData(unsigned char* data, unsigned length){
				static_assert(false, "This must be implemented by your rendering system!");
			}

			void attach(R&) {
				static_assert(false, "This must be implemented by your rendering system!");
			}

		};

	}
}

#endif
