
#include <random>

#include "Shader/ShaderNode.hpp"
#include "Shader/Attribute.hpp"

using namespace MyUPlay::MyEngine::Shader;
using namespace std;

string IAttribute::uniqueName = IAttribute::generateUniqueName();

static default_random_engine gen;
static uniform_int_distribution<unsigned short> dist;

string IAttribute::generateUniqueName(){
    return "var" + std::to_string(dist(random));
}
