#ifndef MYUPLAY_MYENGINE_VECTOR2
#define MYUPLAY_MYENGINE_VECTOR2



namespace MyUPlay {

	namespace MyEngine {

		/**
		 * This class supports any numeric type or class that supports
		 * all of the standard operator for floats or doubles
		 */
		template <typename T = float>
		class Vector2 {

		public:
			T x, y;

			Vector2(T x = 0, T y = 0) : x(x), y(y) {}

			Vector2& set(T x, T y){
				this->x = x;
				this->y = y;
				return *this;
			}

			inline Vector2& operator=(const Vector2& v){
				return copy(v);
			}

			Vector2& setX(T x){
				this->x = x;
				return *this;
			}

			Vector2& setY(T y){
				this->y = y;
				return *this;
			}

			Vector2& copy(const Vector2& v){
				x = v.x;
				y = v.y;
				return *this;
			}

			Vector2& add(const Vector2& v){
				x += v.x;
				y += v.y;
				return *this;
			}

			inline Vector2& operator+=(const Vector2& v){
				return add(v);
			}

			Vector2& addVectors(const Vector2& a, const Vector2& b){
				x = a.x + b.x;
				y = a.y + a.y;
				return *this;
			}

			Vector2& add(T scalar){
				x += scalar;
				y += scalar;
				return *this;
			}

			Vector2& sub(const Vector2& v){
				x -= v.x;
				y -= v.y;
				return *this;
			}

			inline Vector2& operator -=(const Vector2& v){
				return sub(v);
			}

			Vector2 subVectors(const Vector2& a, const Vector2& b){
				x = a.x - b.x;
				y = a.y - b.y;
				return *this;
			}

			Vector2& sub(T scalar){
				x -= scalar;
				y -= scalar;
				return *this;
			}

			Vector2& multiplyScalar(T scalar){
				x *= scalar;
				y *= scalar;
				return *this;
			}

			Vector2& divideScalar(T scalar){
				if (scalar == 0.0){
					x = 0;
					y = 0;
				}
				else {
					x /= scalar;
					y /= scalar;
				}
				return *this;
			}

			Vector2& negate(){
				x = -x;
				y = -y;
				return *this;
			}

            Vector2& floor(){

                x = floor(x);
                y = floor(y);

                return *this;

            }

            Vector2& ceil(){

                x = ceil(x);
                y = ceil(y);

                return *this;

            }

            Vector2& round(){

                x = round(x);
                y = round(y);

                return *this;

            }

            Vector2& roundToZero() {

                x = x < 0 ? ceil(x) : floor(x);
                y = y < 0 ? ceil(y) : floor(y);

                return *this;

            }

			Vector2& min(const Vector2&);
			Vector2& max(const Vector2&);
			Vector2& clamp(const Vector2& min, const Vector2& max);
			Vector2& clampScalar(T min, T max);

		};
	}
}




#endif
