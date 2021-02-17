#pragma once

#include "Color.hpp"

#include <string>


namespace MyEngine {

	template<typename T>
	class Fog {

		std::string name;

		Color color;

		T near = 1;
		T far = 1000;

		Fog(Color color = Color(), T near = 1, T far = 1000)
				: color(color), near(near), far(far) {}

	};

}



