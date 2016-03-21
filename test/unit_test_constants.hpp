#include <limits>
#include "Vector2.hpp"
#include "Vector3.hpp"


float posInf = std::numeric_limits<float>::infinity();
float negInf = -std::numeric_limits<float>::infinity();

MyUPlay::MyEngine::Vector2<float> zero2();
MyUPlay::MyEngine::Vector2<float> one2(1,1);
MyUPlay::MyEngine::Vector2<float> two2(2,2);
MyUPlay::MyEngine::Vector2<float> negInf2(negInf, negInf);
MyUPlay::MyEngine::Vector2<float> posInf2(posInf, posInf);

MyUPlay::MyEngine::Vector3<float> zero3();
MyUPlay::MyEngine::Vector3<float> one3(1,1,1);
MyUPlay::MyEngine::Vector3<float> two3(2,2,2);
MyUPlay::MyEngine::Vector3<float> negInf3(negInf, negInf, negInf);
MyUPlay::MyEngine::Vector3<float> posInf3(posInf, posInf, posInf);


float x = 2;
float y = 3;
float z = 4;
float w = 5;

