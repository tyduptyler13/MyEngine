//
// Created by tyler on 3/13/21.
//
#pragma once

namespace MyEngine {
	struct Plugin {
		virtual ~Plugin() = default;

		virtual void onLoad() = 0;

		virtual void onUnload() = 0;
	};
}
