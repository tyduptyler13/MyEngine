#include "SoundObject.hpp"

#include "nbind/nbind.h"

using namespace MyEngine;

NBIND_CLASS(SoundObject) {

	getset(getRepeat, setRepeat);

	method(play);
	method(setAudio);
	method(isPlaying);
	method(setVelocity);
	method(setPosition);
	method(setDirection);

}

NBIND_CLASS(Listener) {

	method(getInstance);

	method(setVelocity);
	method(setPosition);
	method(setDirection);

}
