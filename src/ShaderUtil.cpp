
#include <random>

#include "Shader/ShaderUtil.hpp"

using namespace MyUPlay::MyEngine::Shader;
using namespace std;

static default_random_engine gen;
static uniform_int_distribution<unsigned> dist;

string generateUniqueName(){
    return "var" + std::to_string(dist(gen));
}
