
#include "Renderer.hpp"

#include "nbind/nbind.h"

using namespace MyUPlay::MyEngine;

#ifdef BUILDING_NODE_EXTENSION

#include "nbind/api.h"

/**
 * The following is an override for the default behavior,
 * but only when building a nodejs addon.
 */
class LambdaWorker : public Nan::AsyncWorker {

	std::function<void()> exec;

public:

	LambdaWorker(Nan::Callback* callback, std::function<void()> exec) :
		Nan::AsyncWorker(callback), exec(exec) {}

	void Execute() {
		exec();
	}

};

template <>
template <>
void Renderer<float>::renderAsync<nbind::cbFunction>(
		std::shared_ptr<Scene<float>> scene, std::shared_ptr<Camera<float>> camera, nbind::cbFunction& cb){

	Nan::Callback* callback = new Nan::Callback(cb.getJsFunction());

	Nan::AsyncQueueWorker(new LambdaWorker(callback, [this, scene, camera]{
		this->render(scene, camera);
	}));

}

#endif

NBIND_CLASS(Renderer<float>, Renderer) {

	method(getSizeArray, "getSize");
	method(setSize);
	method(setViewport);
	method(getViewportArray, "getViewport");

	multimethod(render, args(std::shared_ptr<Scene<float>>, std::shared_ptr<Camera<float>>));
	method(template renderAsync<nbind::cbFunction>, "renderAsync");

	method(setFullScreen);
	method(setWindowed);

	multimethod(template onResize<nbind::cbFunction>, args(nbind::cbFunction&), "onResize");
	//Note: Loop is not recommended to be exposed to javascript as it is a long term blocking call.
	//If you wish to use loop behavior, duplicate its code in javascript using a callback and settimeout.


	method(needsToClose);

}
