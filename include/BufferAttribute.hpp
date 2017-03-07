#ifndef MYUPLAY_MYENGINE_BUFFERATTRIBUTE
#define MYUPLAY_MYENGINE_BUFFERATTRIBUTE

#include <vector>
#include <functional>

#include "Math.hpp"
#include "Vector3.hpp"
#include "Vector2.hpp"

namespace MyUPlay {

	namespace MyEngine {

		template <typename T, typename key = unsigned>
		class BufferAttribute {

			std::vector<T> buffer;

			std::vector<std::function<void(const BufferAttribute& self, const std::vector<T>&)>> onUpdateList;
			std::vector<std::function<void(const BufferAttribute& self)>> onDeleteList;

			void update() {
				for (auto& func : onUpdateList) {
					func(buffer);
				}
			}

		public:

			Math::UUID uuid = Math::generateUUID();
			std::string name;

			//If true, the data could change often and some graphics apis treat dynamic data differently.
			bool dynamic = false; //Used to hint if the data will change often if at all.

			BufferAttribute& operator=(const std::vector<T>& buf) {
				buffer = buf;

				update();

				return *this;
			}

			BufferAttribute& operator=(const std::vector<Vector3<T>>& buf) {
				buffer.clear();
				buffer.resize(buf.size() * 3);

				for (unsigned i = 0, o = 0; i < buf.size(); ++i, o += 3) {
					buf[i].toArray(buffer, o);
				}

				update();

				return *this;
			}

			BufferAttribute& operator=(const std::vector<Vector2<T>>& buf) {
				buffer.clear();
				buffer.resize(buf.size() * 2);

				for (unsigned i = 0, o = 0; i < buf.size(); ++i, o += 2) {
					buf[i].toArray(buffer, o);
				}

				update();

				return *this;
			}

			BufferAttribute& onUpdate(std::function<void(const std::vector<T>&)> func) {
				onUpdateList.push_back(func);
				func(*this, buffer);
				return *this;
			}

			BufferAttribute(std::string name) : name(name) {}
			~BufferAttribute() {
				for (auto& func : onDeleteList) {
					func(*this);
				}
			}

		};

	}

}

#endif
