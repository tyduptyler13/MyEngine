#include <limits>

#include "constants.hpp"

float Test::posInf = std::numeric_limits<float>::infinity();
float Test::negInf = -std::numeric_limits<float>::infinity();

MyUPlay::MyEngine::Vector2<float> Test::zero2(0,0);
MyUPlay::MyEngine::Vector2<float> Test::one2(1,1);
MyUPlay::MyEngine::Vector2<float> Test::two2(2,2);
MyUPlay::MyEngine::Vector2<float> Test::negInf2(negInf, negInf);
MyUPlay::MyEngine::Vector2<float> Test::posInf2(posInf, posInf);

MyUPlay::MyEngine::Vector3<float> Test::zero3(0,0);
MyUPlay::MyEngine::Vector3<float> Test::one3(1,1,1);
MyUPlay::MyEngine::Vector3<float> Test::two3(2,2,2);
MyUPlay::MyEngine::Vector3<float> Test::negInf3(negInf, negInf, negInf);
MyUPlay::MyEngine::Vector3<float> Test::posInf3(posInf, posInf, posInf);

float Test::x = 2;
float Test::y = 3;
float Test::z = 4;
float Test::w = 5;

