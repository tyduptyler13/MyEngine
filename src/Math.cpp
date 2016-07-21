#include "Math.hpp"

#include <random>
#include <cmath>
#include <cstdarg>

namespace MyUPlay {
	namespace MyEngine {
		namespace Math {

			static const char characters[63] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

			const UUID generateUUID(){

				UUID uuid(36, ' ');
				std::random_device rand; //Trueish random (slow)
				std::default_random_engine randG(rand()); //Fake random (fast) with good rand input seed
				std::uniform_int_distribution<int> dist(0, 62); //Range for UUID

				for ( unsigned i = 0; i < 36; ++i ) {

					if ( i == 8 || i == 13 || i == 18 || i == 23 ) {
						uuid[i] = '-';
					} else if (i == 14) {
						uuid[i] = '4';
					} else {
						uuid[i] = characters[ dist(randG) ];
					}

				}

				return uuid;

			}

			template <>
			float euclideanModulo(float n, float m) {
				return fmod(fmod(n,m) + m, m); //Needed specialization for float.
			}

			template <>
			double euclideanModulo(double n, double m) {
				return fmod(fmod(n,m) + m, m); //Needed specialization for double.
			}

			const double PI = std::acos( -1.0 );

		}
	}
}
