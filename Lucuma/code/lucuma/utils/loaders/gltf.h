#pragma once

#include "../../types.h"
#include "../allocator.h"
#include "../../math.h"
#include "../array.h"

#define LU_GLTF_MAX_STRING_SIZE 64

namespace lu
{
	enum GLTFAttribSemantic
	{
		GLTF_ATTRIB_POSITION = 0x0001,
		GLTF_ATTRIB_NORMAL = 0x0002,
		GLTF_ATTRIB_TANGENT = 0x0004,
		GLTF_ATTRIB_TEXCOORD_0 = 0x0008,
		GLTF_ATTRIB_TEXCOORD_1 = 0x0010,
		GLTF_ATTRIB_COLORS_0 = 0x0020,
		GLTF_ATTRIB_JOINTS_0 = 0x0040,
		GLTF_ATTRIB_WEIGHTS_0 = 0x0080
	};

	enum GLTFTextureFilter
	{
		GLTF_FILTER_NEAREST,
		GLTF_FILTER_LINEAR,
		GLTF_FILTER_NEAREST_MIPMAP_NEAREST,
		GLTF_FILTER_LINEAR_MIPMAP_NEAREST,
		GLTF_FILTER_NEAREST_MIPMAP_LINEAR,
		GLTF_FILTER_LINEAR_MIPMAP_LINEAR
	};

	enum GLTFTextureWrap
	{
		GLTF_WRAP_CLAMP_TO_EDGE,
		GLTF_WRAP_MIRRORED_REPEAT,
		GLTF_WRAP_REPEAT
	};

	enum GLTFPrimitiveMode
	{
		GLTF_PRIMITIVE_POINTS,
		GLTF_PRIMITIVE_LINES,
		GLTF_PRIMITIVE_LINE_LOOP,
		GLTF_PRIMITIVE_LINE_STRIP,
		GLTF_PRIMITIVE_TRIANGLES,
		GLTF_PRIMITIVE_TRIANGLE_STRIP,
		GLTF_PRIMITIVE_TRIANGLE_FAN
	};

	enum GLTFComponentType
	{
		GLTF_COMPONENT_BYTE,
		GLTF_COMPONENT_UNSIGNED_BYTE,
		GLTF_COMPONENT_SHORT,
		GLTF_COMPONENT_UNSIGNED_SHORT,
		GLTF_COMPONENT_UNSIGNED_INT,
		GLTF_COMPONENT_FLOAT,
	};

	enum GLTFValueType
	{
		GLTF_VALUE_SCALAR,
		GLTF_VALUE_VEC2,
		GLTF_VALUE_VEC3,
		GLTF_VALUE_VEC4,
		GLTF_VALUE_MAT2,
		GLTF_VALUE_MAT3,
		GLTF_VALUE_MAT4
	};

	enum GLTFBufferTarget
	{
		GLTF_TARGET_VERTEX_BUFFER,
		GLTF_TARGET_INDEX_BUFFER
	};

	enum GLTFCameraType
	{
		GLTF_CAMERA_PERSPECTIVE,
		GLTF_CAMERA_ORTHOGRAPHIC
	};

	enum GLTFAlphaMode
	{
		GLTF_ALPHA_OPAQUE,
		GLTF_ALPHA_MASK,
		GLTF_ALPHA_BLEND
	};

	enum GLTFTargetPath
	{
		GLTF_PATH_TRANSLATION,
		GLTF_PATH_ROTATION,
		GLTF_PATH_SCALE,
		GLTF_PATH_WEIGHTS
	};

	enum GLTFInterpolation
	{
		GLTF_INTERPOLATION_LINEAR,
		GLTF_INTERPOLATION_STEP,
		GLTF_INTERPOLATION_CUBICSPLINE
	};

	union GLTFValueComponent
	{
		float32_t floats[16];
		uint32_t uints[16];
		uint16_t ushorts[16];
		int16_t shorts[16];
		uint8_t ubytes[16];
		int8_t bytes[16];
	};

	struct GLTFTextureInfo
	{
		int32_t index;
		int32_t texCoord;
	};

	struct GLTFNormalTextureInfo
	{
		int32_t index;
		int32_t texCoord;
		float32_t scale;
	};

	struct GLTFOcclusionTextureInfo
	{
		int32_t index;
		int32_t texCoord;
		float32_t strength;
	};

	struct GLTFAccessor
	{
		GLTFValueComponent maxValue;
		GLTFValueComponent minValue;
		
		int32_t bufferView;
		int32_t bufferOffset;
		int32_t count;
		GLTFValueType type;
		GLTFComponentType componentType;
		bool normalized;
	};

	struct GLTFAsset
	{
		char copyright[LU_GLTF_MAX_STRING_SIZE];
		char generator[LU_GLTF_MAX_STRING_SIZE];
		char version[LU_GLTF_MAX_STRING_SIZE];
		char minVersion[LU_GLTF_MAX_STRING_SIZE];
	};

	struct GLTFBuffer
	{
		char uri[LU_GLTF_MAX_STRING_SIZE];
		
		int32_t byteLength;
	};

	struct GLTFBufferView
	{
		
		int32_t buffer;
		int32_t byteOffset;
		int32_t byteLength;
		int32_t byteStride;
		GLTFBufferTarget target;
	};

	struct GLTFPerspectiveCamera
	{
		float32_t aspectRatio;
		float32_t yfov;
		float32_t zfar;
		float32_t znear;
	};

	struct GLTFOrthographicCamera
	{
		float32_t xmag;
		float32_t ymag;
		float32_t zfar;
		float32_t znear;
	};

	struct GLTFCamera
	{
		union CameraData
		{
			GLTFPerspectiveCamera perspective;
			GLTFOrthographicCamera orthographic;
		} camera;
		GLTFCameraType type;
	};

	struct GLTFImage
	{
		char uri[LU_GLTF_MAX_STRING_SIZE];
		char mimeType[LU_GLTF_MAX_STRING_SIZE];
		int32_t bufferView;
	};

	struct GLTFIndices
	{
		int32_t bufferView;
		int32_t byteOffset;
		GLTFComponentType componentType;
	};

	struct GLTFPBRMetallicRoughness
	{
		float32_t baseColor[4];
		float32_t metallicFactor;
		float32_t roughnessFactor;
		GLTFTextureInfo baseColorTexture;
		GLTFTextureInfo metallicRoughnessTexture;
	};

	struct GLTFMaterial
	{
		GLTFPBRMetallicRoughness pbrMetallicRoughness;
		GLTFNormalTextureInfo normalTexture;
		GLTFOcclusionTextureInfo occlusionTexture;
		GLTFTextureInfo emissiveTexture;
		float32_t emissiveFactor[3];
		float32_t alphaCutoff;
		GLTFAlphaMode alphaMode;
		bool doubleSided;
	};

	struct GLTFPrimitiveTarget
	{
		GLTFAttribSemantic targets[8];
		uint32_t length;
	};

	struct GLTFPrimitive
	{
		Array<GLTFAttribSemantic> attributes;
		Array<GLTFPrimitiveTarget> targets;
		int32_t indices;
		int32_t material;
		GLTFPrimitiveMode mode;
	};

	struct GLTFMesh
	{
		Array<GLTFPrimitive> primitive;
		Array<float32_t> weights;
	};

	struct GLTFNode
	{
		float32_t matrix[16];
		float32_t rotation[4];
		float32_t scale[3];
		float32_t translation[3];
		Array<int32_t> children;
		Array<int32_t> weights;
		int32_t camera;
		int32_t skin;
		int32_t mesh;
	};

	struct GLTFSampler
	{
		GLTFTextureFilter magFilter;
		GLTFTextureFilter minFilter;
		GLTFTextureWrap wrapS;
		GLTFTextureWrap wrapT;
	};

	struct GLTFScene
	{
		Array<int32_t> nodes;
	};

	struct GLTFSkin
	{
		Array<int32_t> joints;
		int32_t inverseBindMatrices;
		int32_t skeleton;
	};

	struct GLTFTarget
	{
		GLTFTargetPath path;
		int32_t node;
	};

	struct GLTFTexture
	{
		int32_t sampler;
		int32_t source;
	};

	struct GLTFChannel
	{
		int32_t sampler;
		GLTFTarget target;
	};

	struct GLTFAnimationSampler
	{
		int32_t input;
		int32_t output;
		GLTFInterpolation interpolation;
	};

	struct GLTFAnimation
	{
		Array<GLTFChannel> channels;
		Array<GLTFAnimationSampler> samplers;
	};

	struct GLTF
	{
		GLTFAsset asset;
		Array<GLTFAccessor> accessors;
		Array<GLTFAnimation> animations;
		Array<GLTFBuffer> buffers;
		Array<GLTFBufferView> bufferViews;
		Array<GLTFCamera> cameras;
		Array<GLTFImage> images;
		Array<GLTFMaterial> materials;
		Array<GLTFMesh> meshes;
		Array<GLTFNode> nodes;
		Array<GLTFSampler> samplers;
		Array<GLTFScene> scenes;
		Array<GLTFSkin> skins;
		Array<GLTFTexture> textures;
		int32_t scene;
	};

	namespace gltf
	{
		bool Parse(IAllocator& allocator, const void* pBuffer, uint64_t bufferSize, GLTF& gltf);
	}
}
