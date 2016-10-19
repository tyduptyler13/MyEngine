#include <limits>

#include "constants.hpp"

const float Test::posInf = std::numeric_limits<float>::infinity();
const float Test::negInf = -std::numeric_limits<float>::infinity();

const MyUPlay::MyEngine::Vector2<float> Test::zero2(0,0);
const MyUPlay::MyEngine::Vector2<float> Test::one2(1,1);
const MyUPlay::MyEngine::Vector2<float> Test::two2(2,2);
const MyUPlay::MyEngine::Vector2<float> Test::negInf2(negInf, negInf);
const MyUPlay::MyEngine::Vector2<float> Test::posInf2(posInf, posInf);

const MyUPlay::MyEngine::Vector3<float> Test::zero3(0,0);
const MyUPlay::MyEngine::Vector3<float> Test::one3(1,1,1);
const MyUPlay::MyEngine::Vector3<float> Test::two3(2,2,2);
const MyUPlay::MyEngine::Vector3<float> Test::negInf3(negInf, negInf, negInf);
const MyUPlay::MyEngine::Vector3<float> Test::posInf3(posInf, posInf, posInf);

const float Test::x = 2;
const float Test::y = 3;
const float Test::z = 4;
const float Test::w = 5;
