/*
 * TaskEngine.cpp
 *
 *  Created on: Aug 27, 2014
 *      Author: tyler
 */

#include "TaskEngine.hpp"

using std;

void MyEngine::Task::start(){

	if (isCanceled()){
		return;
	}

	run();

}

void MyEngine::Task::cancel(){
	canceled = true;
}

