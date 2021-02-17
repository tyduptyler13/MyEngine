#include "SoundObject.hpp"

using namespace MyEngine;
using namespace std;

Listener* Listener::singleton = new Listener();
static auto soundLog = getLogger("Sound");

Listener::Listener(){
	//TODO
}

Listener& Listener::setVelocity(float){
	//TODO
	return *this;
}

Listener& Listener::setPosition(const Vector3<float>&){
	//TODO
	return *this;
}

Listener& Listener::setDirection(const Vector3<float>&){
	//TODO
	return *this;
}

SoundObject& SoundObject::play(){
	//TODO
	return *this;
}

SoundObject& SoundObject::setAudio(std::string filename){
	//TODO
	return *this;
}

bool SoundObject::isPlaying() const {
	//TODO
	return false;
}

float SoundObject::getLength() const {
	//TODO
	return 0;
}

SoundObject& SoundObject::setVelocity(float){
	//TODO
	return *this;
}

SoundObject& SoundObject::setPosition(const Vector3<float>&){
	//TODO
	return *this;
}

SoundObject& SoundObject::setDirection(const Vector3<float>&){
	//TODO
	return *this;
}

//TODO
