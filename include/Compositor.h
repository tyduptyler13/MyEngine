#pragma once

struct Compositor {
	// TODO Conceptual layer system/post effect system

	// Need groups of layers to render and any effects that should be rendered on the result of those layers
	// Need to combine all the layers rendered this way
	// Maybe layers will work in more of a graph sort of fashion
	// long term we could add a visual programming interface for this

	// Example uses to consider
	// (deferred renderer) -> [[depth map], [albedo map, normal map]] -> combine using lighting
	// (post passes) -> [depth of field, hdr/eye adaptation, etc]
	// (UI pass) -> Overlay UI buffer on top of whatever result
};
