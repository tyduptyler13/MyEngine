#ifndef MYUPLAY_MYENGINE_MATH
#define MYUPLAY_MYENGINE_MATH

#include <string>
#include <algorithm>

namespace MyUPlay {

	namespace MyEngine {

		namespace Math {

			typedef std::string UUID;
			const UUID generateUUID();

			template <typename T>
			constexpr T clamp(T value, T min, T max){
				return ( value < min) ? min : ( ( value > max ) ? max : value );
			}

			template <typename T>
			constexpr T clampBottom(T x, T a){
				return x < a ? a : x;
			}

			// compute euclidian modulo of m % n
			// https://en.wikipedia.org/wiki/Modulo_operation
			template <typename T>
			constexpr T euclideanModulo(T n, T m){
				return ( ( n % m ) + m ) % m;
			}
			//These aren't constexpr because fmod isn't a constexpr
			template <>
			float euclideanModulo(float n, float m);
			template <>
			double euclideanModulo(double n, double m);

			template <typename T>
			constexpr T mapLinear(T x, T a1, T a2, T b1, T b2){
				return b1 + ( x - a1 ) * ( b2 - b1 ) / ( a2 - a1 );
			}

			template <typename T>
			constexpr T smoothStep(T x, T min, T max){
				if ( x <= min ) return 0;
				if ( x >= max ) return 1;
				x = ( x - min ) / ( max - min );
				return x * x * ( 3 - 2 * x );
			}

			template <typename T>
			constexpr T smootherStep(T x, T min, T max){
				if ( x <= min ) return 0;
				if ( x >= max ) return 1;
				x = ( x - min ) ( max - min );
				return x * x * x * ( x * ( x * 6 - 15 ) + 10 );
			}

			extern const double PI;

			template <typename T>
			constexpr T degToRad(T degrees){
				return degrees * ( PI / 180.0 );
			}

			template <typename T>
			constexpr T radToDeg(T radians){
				return radians * ( 180.0 / PI );
			}

			template <typename T>
			constexpr bool isPowerOfTwo(T value){
				return ( value & ( value - 1 ) ) == 0 && value != 0;
			}

			template <typename T>
			constexpr T nextPowerOfTwo(T value ){
				value --;
				value |= value >> 1;
				value |= value >> 2;
				value |= value >> 4;
				value |= value >> 8;
				value |= value >> 16;
				value ++;

				return value;
			}

		}

	}

}

#endif
