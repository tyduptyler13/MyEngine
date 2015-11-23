#ifndef MYUPLAY_MYENGINE_CONSTANTS
#define MYUPLAY_MYENGINE_CONSTANTS

namespace MyUPlay {

	namespace MyEngine {

		//GL State constants
		const short CullFaceNone = 0,
			    CullFaceBack = 1,
			    CullFaceFront = 2,
			    CullFaceFrontBack = 3;

		const short FrontFaceDirectionCW = 0,
			    FrontFaceDirectionCCW = 1;

		//Shadow types.
		const short BasicShadowMap = 0,
			    PCFShadowMap = 1,
			    PCFSoftShadowMap = 2;

		//side
		const short FrontSide = 0,
			    BackSide = 1,
			    DoubleSide = 2;

		//shading
		const short FlatShading = 1,
			    SmoothShading = 2;

		//colors
		const short NoColors = 0,
			    FaceColors = 1,
			    VertexColors = 2;

		//blending modes
		const short NoBlending = 0,
			    NormalBlending = 1,
			    AdditiveBlending = 2,
			    SubtractiveBlending = 3,
			    MultiplyBlending = 4,
			    CustomBlending = 5;

		//custom blending equations
		const short AddEquation = 100,
			    SubtractEquation = 101,
			    ReverseSubtractEquation = 102,
			    MinEquation = 103,
			    MaxEquation = 104;

		//custom blending destination factors
		const short ZeroFactor = 200,
			    OneFactor = 201,
			    SrcColorFactor = 202,
			    OneMinusSrcColorFactor = 203,
			    SrcAlphaFactor = 204,
			    OneMinusSrcAlphaFactor = 205,
			    DstAlphaFactor = 206,
			    OneMinusDstAlphaFactor = 207;

		//Custom blending source factors
		const short DstColorFactor = 208,
			    OneMinusDstColorFactor = 209,
			    SrcAlphaSaturateFactor = 210;

		//depth modes
		const short NeverDepth = 0,
			    AlwaysDepth = 1,
			    LessDepth = 2,
			    LessEqualDepth = 3,
			    EqualDepth = 4,
			    GreaterEqualDeath = 5,
			    GreaterDepth = 6,
			    NotEqualDepth = 7;

		//texture constants
		const short MultiplyOperation = 0,
			    MixOperation = 1,
			    AddOperation = 2;

		//mapping modes
		const short UVMapping = 300,
			    CubeReflectionMapping = 301,
			    CubeRefractionMapping = 302,
			    EquirectangularReflectionMapping = 303,
			    EquirectangularRefractionMapping = 304,
			    SphericalReflectionMapping = 305;

		//Wrapping modes
		const short RepeatWrapping = 1000,
			    ClampToEdgeWrapping = 1001,
			    MirroredRepeatWrapping = 1002;

		//Filters
		const short NearestFilter = 1003,
			    NearestMipMapNearestFilter = 10004,
			    NearestMipMapLinearFilter = 10005,
			    LinearFilter = 1006,
			    LinearMipMapNearestFilter = 1007,
			    LinearMipMapLinearFilter = 1008;

		//Data types
		const short UnsignedByteType = 1009,
			    ByteType = 1010,
			    ShortType = 1011,
			    UnsignedShortType = 1012,
			    IntType = 1013,
			    UnsignedIntType = 1014,
			    FloatType = 1015,
			    HalfFloatType = 1025;

		//Pixel types
		const short UnsignedShort4444Type = 1016,
			    UnsignedShort5551Type = 1017,
			    UnsignedShort565type = 1018;

		//Pixel formats
		const short AlphaFormat = 1019,
			    RGBFormat = 1020,
			    RGBAFormat = 1021,
			    LuminanceFormat = 1022,
			    LuminanceAlphaFormat = 1023,
			    RGBEFormat = RGBAFormat;

		//DDS/ST3C Compressed texture formats
		const short RGB_S3TC_DXT1_Format = 2001,
			    RGBA_S3TC_DXT1_Format = 2002,
			    RGBA_S3TC_DXT3_Format = 2003,
			    RGBA_S3TC_DXT5_Format = 2004;

		//PVRTC compressed texture formats
		const short RGB_PVRTC_4BPPV1_Format = 2100,
			    RGB_PVRTC_2BPPV1_Format = 2101,
			    RGBA_PVRTC_4BPPV1_Format = 2102,
			    RGBA_PVRTC_2BPPV1_Format = 2103;

		//Loop styles for AnimationAction
		const short LoopOnce = 2200,
			    LoopRepeat = 2201,
			    LoopPingPong = 2202;

	}

}

#endif

