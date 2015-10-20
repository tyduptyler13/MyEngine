#ifndef MYUPLAY_MYENGINE_LINE3
#define MYUPLAY_MYENGINE_LINE3

#include "Vector3.hpp"
#include "Math.hpp"
#include "Matrix4.hpp"

namespace MyUPlay {

	namespace MyEngine {

		template <typename T>
		class Line3 {

		public:

			Vector3<T> start, end;

			Line3(Vector3<T> start = Vector3<T>(), Vector3<T> end = Vector3<T>()) : start(start), end(end) {}

			Line3(const Line3& line) : start(line.start), end(line.end) {}

			Line3& set(const Vector3<T>& start, const Vector3<T>& end) {
				this->start = start;
				this->end = end;
				return *this;
			}

			Line3& copy(const Line3& line){
				start = line.start;
				end = line.end;
				return *this;
			}

			Line3& operator=(const Line3& line){
				return copy(line);
			}

			Vector3<T> center() const {
				return Vector3<T>().addVectors(start, end).multiply(0.5);
			}

			Vector3<T> delta() const {
				return Vector3<T>().subVectors(end, start);
			}

			T distanceSq() const {
				return start.distanceToSquared(end);
			}

			T distance() const {
				return start.distanceTo(end);
			}

			Vector3<T> at(T t) const {
				return delta().multiply(t).add(start);
			}

			Vector3<T> closestPointParameter(const Vector3<T>& point, bool clampToLine) const {
				
				Vector3<T> startP;
				Vector3<T> startEnd;

				startP.subVectors(point, start);
				startEnd.subVectors(end, start);

				T startEnd2 = startEnd.dot(startEnd);
				T startEndStartP = startEnd.dot(startP);

				T t = startEndStartP / startEnd2;

				if (clampToLine) {

					return Math::clamp<T>(t, 0, 1);

				}

				return t;

			}

			Vector3<T> closestPointToPoint(const Vector3<T>& point, bool clampToLine) const {

				T t = closestPointParameter(point, clampToLine);

				return delta().multiply(t).add(start);

			}

			Line3& applyMatrix4(const Matrix4<T>& matrix) {

				start.applyMatrix4(matrix);
				end.applyMatrix4(matrix);

				return *this;

			}

			bool equals(const Line3& line) const {

				return start == line.start && end == line.end;

			}

			bool operator==(const Line3& line) const {
				return equals(line);
			}


		};

	}

}

#endif

