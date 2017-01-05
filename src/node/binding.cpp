#include "nbind/nbind.h"

#define NBINDING_MODE

//Breaks std::function
#undef function

#include "Vector3.hpp"
#include "Object3D.hpp"
#include "Scene.hpp"
#include "Camera.hpp"
#include "PerspectiveCamera.hpp"
#include "Renderer.hpp"
#include "GLES2Renderer.hpp"
#include "Geometries/BoxGeometry.hpp"
#include "GLES2MaterialLib.hpp"
#include "Mesh.hpp"
#include "GeometryImporter.hpp"
#include "GLES2OverlaySystem.hpp"
