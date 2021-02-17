#pragma once

#include <vector>
#include <functional>

#include "Math.hpp"
#include "Vector3.hpp"
#include "Vector2.hpp"

namespace MyEngine {

	template<typename T>
	class BufferAttribute {

		std::vector<T> buffer;

		std::vector<std::function<void(const BufferAttribute& self, const std::vector<T>&)>> onUpdateList;
		std::vector<std::function<void(const BufferAttribute& self)>> onDeleteList;

		bool isUpdating = false;

		void update() {
			if (isUpdating) {
				throw std::runtime_error("Reentry into buffer attribute from update event!!! Undefined behavior!");
			}
			isUpdating = true;
			for (auto& func : onUpdateList) {
				func(*this, buffer);
			}
			isUpdating = false;
		}

	public:

		const Math::UUID uuid = Math::generateUUID();

		//If true, the data could change often and some graphics apis treat dynamic data differently.
		bool dynamic = false; //Used to hint if the data will change often if at all.

		unsigned char length;

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

		unsigned size() const {
			return buffer.size();
		}

		const Math::UUID getUUID() const {
			return uuid;
		}

		operator const std::vector<T>&() const {
			return buffer;
		}

		BufferAttribute& getBuffer(std::function<void(std::vector<T>&)> func) {
			func(buffer);
			update();
			return *this;
		}

		T& operator[](unsigned index) {
			return buffer[index];
		}

		const T operator[](unsigned index) const {
			return buffer[index];
		}

		BufferAttribute& onUpdate(std::function<void(const BufferAttribute&, const std::vector<T>&)> func) {
			onUpdateList.push_back(func);
			func(*this, buffer);
			return *this;
		}

		BufferAttribute& onDelete(std::function<void(const BufferAttribute&)> func) {
			onDeleteList.push_back(func);
			return *this;
		}

		/**
		 * The length is the size of internal objects. Aka a vector3 is 3, vector2 is 2, etc.
		 */
		BufferAttribute(unsigned char length = 1) : length(length) {}

		~BufferAttribute() {
			for (auto& func : onDeleteList) {
				func(*this);
			}
		}

	};

}
