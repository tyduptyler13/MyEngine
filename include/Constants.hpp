#ifndef MYUPLAYMYENGINECONSTANTS
#define MYUPLAYMYENGINECONSTANTS

namespace MyUPlay {

	namespace MyEngine {

		//Renderer sort order
		enum SortOrder {
			FrontToBack, //Default option, sorts all objects by distance from camera
			MaterialSort, //Sorts first by material, then by distance from camera (minimize state changes)
			NoSort //Disables sorting, objects will be rendered in an implementation defined way
		};

		//GL State constants
		enum CullConstant {
			CullFaceNone = 0,
			CullFaceBack = 1,
			CullFaceFront = 2,
			CullFaceFrontBack = 3
		};

		enum CullDirection {
			FrontFaceDirectionCW = 0,
			FrontFaceDirectionCCW = 1
		};

		//Shadow types.
		enum ShadowMapType {
			BasicShadowMap = 0,
			PCFShadowMap = 1,
			PCFSoftShadowMap = 2
		};

		//side
		enum SideConstant {
			FrontSide = 0,
			BackSide = 1,
			DoubleSide = 2
		};

		//shading
		enum ShadeConstant {
			FlatShading = 1,
			SmoothShading = 2
		};

		//colors
		enum ColorConstant {
			NoColors = 0,
			FaceColors = 1,
			VertexColors = 2
		};

		//blending modes
		enum BlendingMode {
			NoBlending = 0,
			NormalBlending = 1,
			AdditiveBlending = 2,
			SubtractiveBlending = 3,
			MultiplyBlending = 4,
			CustomBlending = 5
		};

		//custom blending equations
		enum BlendingEquation {
			AddEquation = 100,
			SubtractEquation = 101,
			ReverseSubtractEquation = 102,
			MinEquation = 103,
			MaxEquation = 104
		};

		//custom blending destination factors
		enum BlendingDestination {
			ZeroFactor = 200,
			OneFactor = 201,
			SrcColorFactor = 202,
			OneMinusSrcColorFactor = 203,
			SrcAlphaFactor = 204,
			OneMinusSrcAlphaFactor = 205,
			DstAlphaFactor = 206,
			OneMinusDstAlphaFactor = 207
		};

		//Custom blending source factors
		enum BlendingSource {
			DstColorFactor = 208,
			OneMinusDstColorFactor = 209,
			SrcAlphaSaturateFactor = 210
		};

		//depth modes
		enum DepthMode {
			NeverDepth = 0,
			AlwaysDepth = 1,
			LessDepth = 2,
			LessEqualDepth = 3,
			EqualDepth = 4,
			GreaterEqualDepth = 5,
			GreaterDepth = 6,
			NotEqualDepth = 7
		};

		//texture constants
		enum TextureConstant {
			MultiplyOperation = 0,
			MixOperation = 1,
			AddOperation = 2
		};

		//mapping modes
		enum MappingMode {
			UVMapping = 300,
			CubeReflectionMapping = 301,
			CubeRefractionMapping = 302,
			EquirectangularReflectionMapping = 303,
			EquirectangularRefractionMapping = 304,
			SphericalReflectionMapping = 305
		};

		//Wrapping modes
		enum WrappingMode {
			RepeatWrapping = 1000,
			ClampToEdgeWrapping = 1001,
			MirroredRepeatWrapping = 1002
		};

		//Filters
		enum FilterConstant {
			NearestFilter = 1003,
			NearestMipMapNearestFilter = 10004,
			NearestMipMapLinearFilter = 10005,
			LinearFilter = 1006,
			LinearMipMapNearestFilter = 1007,
			LinearMipMapLinearFilter = 1008
		};

		//Data types
		enum DataType {
			UnsignedByteType = 1009,
			ByteType = 1010,
			ShortType = 1011,
			UnsignedShortType = 1012,
			IntType = 1013,
			UnsignedIntType = 1014,
			FloatType = 1015,
			HalfFloatType = 1025
		};

		//Pixel types
		enum PixelType {
			UnsignedShort4444Type = 1016,
			UnsignedShort5551Type = 1017,
			UnsignedShort565type = 1018
		};

		//Pixel formats
		enum PixelFormat {
			AlphaFormat = 1019,
			RGBFormat = 1020,
			RGBAFormat = 1021,
			LuminanceFormat = 1022,
			LuminanceAlphaFormat = 1023,
			RGBEFormat = RGBAFormat
		};

		//DDS/ST3C Compressed texture formats
		enum CompressedTextureFormat {
			RGBS3TCDXT1Format = 2001,
			RGBAS3TCDXT1Format = 2002,
			RGBAS3TCDXT3Format = 2003,
			RGBAS3TCDXT5Format = 2004
		};

		//PVRTC compressed texture formats
		enum { //TODO Might not use these, might remove.
			RGBPVRTC4BPPV1Format = 2100,
			RGBPVRTC2BPPV1Format = 2101,
			RGBAPVRTC4BPPV1Format = 2102,
			RGBAPVRTC2BPPV1Format = 2103
		};

		//Loop styles for AnimationAction
		enum AnimationStyle {
			LoopOnce = 2200,
			LoopRepeat = 2201,
			LoopPingPong = 2202
		};

	}

}

#endif
