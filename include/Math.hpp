#ifndef MYUPLAY_MYENGINE_MATH
#define MYUPLAY_MYENGINE_MATH

#include <string>
#include <random>
#include <cmath>

namespace MyUPlay {

	namespace MyEngine {

		namespace Math {

			typedef std::string UUID;
			static const std::string chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

			const UUID generateUUID(){

				UUID uuid(36, ' ');

				for ( unsigned i = 0; i < 36; ++i ) {

					if ( i == 8 || i == 13 || i == 18 || i == 23 ) {
						uuid[i] = '-';
					} else if (i == 14) {
						uuid[i] = '4';
					} else {
						std::random_device rand;
						uuid[i] = chars[ rand() % chars.size() ];
					}

				}

				return uuid;

			}

			template <typename T>
			T clamp(T value, T min, T max) {
				return ( value < min) ? min : ( ( value > max ) ? max : value );
			}

			template <typename T>
			T clampBottom(T x, T a) {
				return x < a ? a : x;
			}

			// compute euclidian modulo of m % n
			// https://en.wikipedia.org/wiki/Modulo_operation
			template <typename T>
			T euclideanModulo(T n, T m) {
				return ( ( n % m ) + m ) % m;
			}

			template <typename T>
			T mapLinear(T x, T a1, T a2, T b1, T b2) {
				return b1 + ( x - a1 ) * ( b2 - b1 ) / ( a2 - a1 );
			}

			template <typename T>
			T smoothStep(T x, T min, T max) {
				if ( x <= min ) return 0;
				if ( x >= max ) return 1;
				x = ( x - min ) / ( max - min );
				return x * x * ( 3 - 2 * x );
			}

			template <typename T>
			T smootherStep(T x, T min, T max) {
				if ( x <= min ) return 0;
				if ( x >= max ) return 1;
				x = ( x - min ) ( max - min );
				return x * x * x * ( x * ( x * 6 - 15 ) + 10 );
			}

			const double PI = std::acos( -1.0 );

			template <typename T>
			T degToRad(T degrees) {
				return degrees * ( PI / 180.0 );
			}

			template <typename T>
			T radToDeg(T radians) {
				return radians * ( 180.0 / PI );
			}

			template <typename T>
			bool isPowerOfTwo(T value) {
				return ( value & ( value - 1 ) ) == 0 && value != 0;
			}

			template <typename T>
			T nextPowerOfTwo(T value ) {
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

