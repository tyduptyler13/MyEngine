#include "Texture.hpp"

using namespace MyUPlay::MyEngine;

static int Texture::load = IMG_INIT(IMG_INIT_JPG | IMG_INIT_PNG);
short Texture::DEFAULT_MAPPING = UVMapping;

void Texture::loadImage() {

	image = IMG_Load(sourceFile.c_str());

}
