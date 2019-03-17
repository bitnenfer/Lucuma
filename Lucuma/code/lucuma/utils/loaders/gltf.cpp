#include "gltf.h"
#include "json.h"
#include "../macros.h"

#define LU_CHUNK_JSON 0x4E4F534A
#define LU_CHUNK_BIN 0x004E4942

namespace lu
{
	namespace gltf
	{
		struct Header
		{
			uint32_t magic;
			uint32_t version;
			uint32_t length;
		};

		struct Chunk
		{
			uint32_t chunkLength;
			uint32_t chunkType;
		};
	}
}

bool lu::gltf::Parse(IAllocator& allocator, const void* pBuffer, uint64_t bufferSize, GLTF& gltf)
{
	JSONTable table;
	byte_t* pByteBuffer = (byte_t*)pBuffer;
	Header* pHeader = (Header*)pByteBuffer;

	pByteBuffer += sizeof(Header);

	Chunk* pChunk = (Chunk*)pByteBuffer;

	pByteBuffer += sizeof(Chunk);

	if (pChunk->chunkType == LU_CHUNK_JSON)
	{
		if (lu::json::Parse(allocator, pByteBuffer, pChunk->chunkLength, table))
		{
			const JSONObject* pRoot = json::GetRootObject(table);

			// Accessors
			const JSONArray* pAccessors = json::GetArrayFromObject(table, *pRoot, "accessors");
			if (pAccessors != NULL)
			{
				for (uint32_t index = 0; index < json::GetArrayElementCount(*pAccessors); ++index)
				{
					const JSONObject* pAccessor = json::GetObjectFromArray(table, *pAccessors, index);
					if (pAccessor == NULL) continue;

					int32_t bufferView = json::GetIntFromObject(table, *pAccessor, "bufferView");
					int32_t bufferOffset = json::GetIntFromObject(table, *pAccessor, "bufferOffset");
					int32_t componentType = json::GetIntFromObject(table, *pAccessor, "componentType");
					int32_t count = json::GetIntFromObject(table, *pAccessor, "count");
					bool normalized = json::GetBoolFromObject(table, *pAccessor, "normalized");
					const char* pType = json::GetStringFromObject(table, *pAccessor, "type");
					const JSONArray* pMax = json::GetArrayFromObject(table, *pAccessor, "max");
					const JSONArray* pMin = json::GetArrayFromObject(table, *pAccessor, "min");

					if (!gltf.accessors.isInitialized())
					{
						gltf.accessors.setAllocator(&allocator);
					}

					GLTFAccessor accesor;
					accesor.bufferView = bufferView;
					accesor.bufferOffset = bufferOffset;
					accesor.count = count;
					accesor.normalized = normalized;
					if (componentType == 5120) accesor.componentType = GLTF_COMPONENT_BYTE;
					else if (componentType == 5121) accesor.componentType = GLTF_COMPONENT_UNSIGNED_BYTE;
					else if (componentType == 5122) accesor.componentType = GLTF_COMPONENT_SHORT;
					else if (componentType == 5123) accesor.componentType = GLTF_COMPONENT_UNSIGNED_SHORT;
					else if (componentType == 5125) accesor.componentType = GLTF_COMPONENT_UNSIGNED_INT;
					else if (componentType == 5126) accesor.componentType = GLTF_COMPONENT_FLOAT;
					
					if (pType != NULL)
					{
						if (pType[0] == 'S') accesor.type = GLTF_VALUE_SCALAR;
						else if (pType[0] == 'V')
						{
							if (pType[3] == '2') accesor.type = GLTF_VALUE_VEC2;
							else if (pType[3] == '3') accesor.type = GLTF_VALUE_VEC3;
							else if (pType[3] == '4') accesor.type = GLTF_VALUE_VEC4;
						}
						else if (pType[0] == 'M')
						{
							if (pType[3] == '2') accesor.type = GLTF_VALUE_MAT2;
							else if (pType[3] == '3') accesor.type = GLTF_VALUE_MAT3;
							else if (pType[3] == '4') accesor.type = GLTF_VALUE_MAT4;
						}
					}
					
					if (pMin != NULL)
					{
						for (uint32_t i = 0; i < json::GetArrayElementCount(*pMin); ++i)
						{
							if (componentType == 5120) accesor.maxValue.bytes[i] = (int8_t)json::GetIntFromArray(table, *pMin, i);
							else if (componentType == 5121) accesor.maxValue.ubytes[i] = (uint8_t)json::GetIntFromArray(table, *pMin, i);
							else if (componentType == 5122) accesor.maxValue.shorts[i] = (int16_t)json::GetIntFromArray(table, *pMin, i);
							else if (componentType == 5123) accesor.maxValue.ushorts[i] = (uint16_t)json::GetIntFromArray(table, *pMin, i);
							else if (componentType == 5125) accesor.maxValue.uints[i] = (uint32_t)json::GetIntFromArray(table, *pMin, i);
							else if (componentType == 5126) accesor.maxValue.floats[i] = (float32_t)json::GetFloatFromArray(table, *pMin, i);
						}
					}
					
					if (pMax != NULL)
					{
						for (uint32_t i = 0; i < json::GetArrayElementCount(*pMax); ++i)
						{
							if (componentType == 5120) accesor.maxValue.bytes[i] = (int8_t)json::GetIntFromArray(table, *pMax, i);
							else if (componentType == 5121) accesor.maxValue.ubytes[i] = (uint8_t)json::GetIntFromArray(table, *pMax, i);
							else if (componentType == 5122) accesor.maxValue.shorts[i] = (int16_t)json::GetIntFromArray(table, *pMax, i);
							else if (componentType == 5123) accesor.maxValue.ushorts[i] = (uint16_t)json::GetIntFromArray(table, *pMax, i);
							else if (componentType == 5125) accesor.maxValue.uints[i] = (uint32_t)json::GetIntFromArray(table, *pMax, i);
							else if (componentType == 5126) accesor.maxValue.floats[i] = (float32_t)json::GetFloatFromArray(table, *pMax, i);
						}
					}
				
					gltf.accessors.push(accesor);
				}

			}

			// Animations
			const JSONArray* pAnimations = json::GetArrayFromObject(table, *pRoot, "animations");
			if (pAnimations != NULL)
			{
				if (!gltf.animations.isInitialized()) gltf.animations.setAllocator(&allocator);
				
				for (uint32_t index = 0; index < json::GetArrayElementCount(*pAnimations); ++index)
				{
					const JSONObject* pAnimation = json::GetObjectFromArray(table, *pAnimations, index);
					if (pAnimation == NULL) continue;
					
					GLTFAnimation& animation = *gltf.animations.alloc();

					LU_NEW(&animation) GLTFAnimation;

					// Animation Channels
					const JSONArray* pChannels = json::GetArrayFromObject(table, *pAnimation, "channels");
					if (pChannels != NULL)
					{
						for (uint32_t i = 0; i < json::GetArrayElementCount(*pChannels); ++i)
						{
							const JSONObject* pChannel = json::GetObjectFromArray(table, *pChannels, i);
							if (pChannel == NULL) continue;
							if (!animation.channels.isInitialized()) animation.channels.setAllocator(&allocator);
							GLTFChannel channel;
							channel.sampler = json::GetIntFromObject(table, *pChannel, "sampler");
							const JSONObject* pChannelTarget = json::GetObjectFromObject(table, *pChannel, "target");
							if (pChannelTarget != NULL)
							{
								channel.target.node = json::GetIntFromObject(table, *pChannelTarget, "node");
								const char* pPath = json::GetStringFromObject(table, *pChannelTarget, "path");
								if (pPath != NULL)
								{
									if (pPath[0] == 't') channel.target.path = GLTF_PATH_TRANSLATION;
									else if (pPath[0] == 'r') channel.target.path = GLTF_PATH_ROTATION;
									else if (pPath[0] == 's') channel.target.path = GLTF_PATH_SCALE;
									else if (pPath[0] == 'w') channel.target.path = GLTF_PATH_WEIGHTS;
								}
							}
							animation.channels.push(channel);
						}
					}
				
					// Animation Samplers
					const JSONArray* pSamplers = json::GetArrayFromObject(table, *pAnimation, "samplers");
					if (pSamplers != NULL)
					{
						for (uint32_t i = 0; i < json::GetArrayElementCount(*pSamplers); ++i)
						{
							const JSONObject* pSampler = json::GetObjectFromArray(table, *pSamplers, i);
							if (pSampler == NULL) continue;
							if (!animation.samplers.isInitialized()) animation.samplers.setAllocator(&allocator);

							GLTFAnimationSampler sampler;
							sampler.input = json::GetIntFromObject(table, *pSampler, "input");
							sampler.output = json::GetIntFromObject(table, *pSampler, "output");
							const char* pInterpolation = json::GetStringFromObject(table, *pSampler, "interpolation");
							if (pInterpolation != NULL)
							{
								if (pInterpolation[0] == 'L') sampler.interpolation = GLTF_INTERPOLATION_LINEAR;
								else if (pInterpolation[0] == 'S') sampler.interpolation = GLTF_INTERPOLATION_STEP;
								else if (pInterpolation[0] == 'C') sampler.interpolation = GLTF_INTERPOLATION_CUBICSPLINE;
							}
							animation.samplers.push(sampler);
						}
					}
				}
			}

			// Asset
			const JSONObject* pAsset = json::GetObjectFromObject(table, *pRoot, "asset");
			if (pAsset != NULL)
			{
				memset(&gltf.asset, 0, sizeof(gltf.asset));

				const char* pCopyright = json::GetStringFromObject(table, *pAsset, "copyright");
				const char* pGenerator = json::GetStringFromObject(table, *pAsset, "generator");
				const char* pVersion = json::GetStringFromObject(table, *pAsset, "version");
				const char* pMinVersion = json::GetStringFromObject(table, *pAsset, "minVersion");

				if (pCopyright != NULL)
				{
					int32_t len = (int32_t)strlen(pCopyright);
					memset(gltf.asset.copyright, 0, LU_GLTF_MAX_STRING_SIZE);
					memcpy(gltf.asset.copyright, pCopyright, len < LU_GLTF_MAX_STRING_SIZE - 1 ? len : LU_GLTF_MAX_STRING_SIZE - 1);
				}
				if (pGenerator != NULL)
				{
					int32_t len = (int32_t)strlen(pGenerator);
					memset(gltf.asset.generator, 0, LU_GLTF_MAX_STRING_SIZE);
					memcpy(gltf.asset.generator, pGenerator, len < LU_GLTF_MAX_STRING_SIZE - 1 ? len : LU_GLTF_MAX_STRING_SIZE - 1);
				}
				if (pVersion != NULL)
				{
					int32_t len = (int32_t)strlen(pVersion);
					memset(gltf.asset.version, 0, LU_GLTF_MAX_STRING_SIZE);
					memcpy(gltf.asset.version, pVersion, len < LU_GLTF_MAX_STRING_SIZE - 1 ? len : LU_GLTF_MAX_STRING_SIZE - 1);
				}
				if (pMinVersion != NULL)
				{
					int32_t len = (int32_t)strlen(pMinVersion);
					memset(gltf.asset.minVersion, 0, LU_GLTF_MAX_STRING_SIZE);
					memcpy(gltf.asset.minVersion, pMinVersion, len < LU_GLTF_MAX_STRING_SIZE - 1 ? len : LU_GLTF_MAX_STRING_SIZE - 1);
				}
			}

			// Buffers
			const JSONArray* pBuffers = json::GetArrayFromObject(table, *pRoot, "buffers");
			if (pBuffers != NULL)
			{
				for (uint32_t index = 0; index < json::GetArrayElementCount(*pBuffers); ++index)
				{
					const JSONObject* pBuffer = json::GetObjectFromArray(table, *pBuffers, index);
					if (pBuffer == NULL) continue;
					if (!gltf.buffers.isInitialized()) gltf.buffers.setAllocator(&allocator);

					const char* pURI = json::GetStringFromObject(table, *pBuffer, "uri");
					int32_t byteLength = json::GetIntFromObject(table, *pBuffer, "byteLength");

					GLTFBuffer buffer;
					buffer.byteLength = byteLength;
					if (pURI != NULL)
					{
						int32_t len = (int32_t)strlen(pURI);
						memcpy(buffer.uri, pURI, len < LU_GLTF_MAX_STRING_SIZE - 1 ? len : LU_GLTF_MAX_STRING_SIZE - 1);
					}

					gltf.buffers.push(buffer);
				}
			}

			// BufferViews
			const JSONArray* pBufferViews = json::GetArrayFromObject(table, *pRoot, "bufferViews");
			if (pBufferViews != NULL)
			{
				for (uint32_t index = 0; index < json::GetArrayElementCount(*pBufferViews); ++index)
				{
					const JSONObject* pBufferView = json::GetObjectFromArray(table, *pBufferViews, index);
					if (pBufferView == NULL) continue;
					if (!gltf.bufferViews.isInitialized()) gltf.bufferViews.setAllocator(&allocator);
					GLTFBufferView bufferView;
					bufferView.buffer = json::GetIntFromObject(table, *pBufferView, "buffer");
					bufferView.byteOffset = json::GetIntFromObject(table, *pBufferView, "byteOffset");
					bufferView.byteLength = json::GetIntFromObject(table, *pBufferView, "byteLength");
					bufferView.byteStride = json::GetIntFromObject(table, *pBufferView, "byteStride");
					int32_t target = json::GetIntFromObject(table, *pBufferView, "target");

					if (target == 34962) bufferView.target = GLTF_TARGET_VERTEX_BUFFER;
					else if (target == 34963) bufferView.target = GLTF_TARGET_INDEX_BUFFER;

					gltf.bufferViews.push(bufferView);
				}
			}

			// Cameras
			const JSONArray* pCameras = json::GetArrayFromObject(table, *pRoot, "cameras");
			if (pCameras != NULL)
			{
				for (uint32_t index = 0; index < json::GetArrayElementCount(*pCameras); ++index)
				{
					const JSONObject* pCamera = json::GetObjectFromArray(table, *pCameras, index);
					if (pCamera == NULL) continue;
					if (!gltf.cameras.isInitialized()) gltf.cameras.setAllocator(&allocator);
					const char* pType = json::GetStringFromObject(table, *pCamera, "type");
					if (!pType == NULL) continue;
					GLTFCamera camera;
					const JSONObject* pPerspective = json::GetObjectFromObject(table, *pCamera, "perspective");
					const JSONObject* pOrthographic = json::GetObjectFromObject(table, *pCamera, "orthographic");
					if (pPerspective != NULL)
					{
						camera.type = GLTF_CAMERA_PERSPECTIVE;
						camera.camera.perspective.aspectRatio = json::GetFloatFromObject(table, *pPerspective, "aspectRatio");
						camera.camera.perspective.yfov = json::GetFloatFromObject(table, *pPerspective, "yfov");
						camera.camera.perspective.zfar = json::GetFloatFromObject(table, *pPerspective, "zfar");
						camera.camera.perspective.znear = json::GetFloatFromObject(table, *pPerspective, "znear");
					}
					else if (pOrthographic != NULL)
					{
						camera.type = GLTF_CAMERA_ORTHOGRAPHIC;
						camera.camera.orthographic.xmag = json::GetFloatFromObject(table, *pOrthographic, "xmag");
						camera.camera.orthographic.ymag = json::GetFloatFromObject(table, *pOrthographic, "ymag");
						camera.camera.orthographic.zfar = json::GetFloatFromObject(table, *pOrthographic, "zfar");
						camera.camera.orthographic.znear = json::GetFloatFromObject(table, *pOrthographic, "znear");
					}
					gltf.cameras.push(camera);
				}
			}

			// Images
			const JSONArray* pImages = json::GetArrayFromObject(table, *pRoot, "images");
			if (pImages != NULL)
			{
				for (uint32_t index = 0; index < json::GetArrayElementCount(*pImages); ++index)
				{
					const JSONObject* pImage = json::GetObjectFromArray(table, *pImages, index);
					if (pImage == NULL) continue;
					if (!gltf.images.isInitialized()) gltf.images.setAllocator(&allocator);
					GLTFImage image;

					memset(&image, 0, sizeof(image));

					const char* pURI = json::GetStringFromObject(table, *pImage, "uri");
					const char* pMimeType = json::GetStringFromObject(table, *pImage, "mimeType");
					image.bufferView = json::GetIntFromObject(table, *pImage, "bufferView");
					
					if (pURI != NULL)
					{
						int32_t len = (int32_t)strlen(pURI);
						memcpy(image.uri, pURI, len < LU_GLTF_MAX_STRING_SIZE - 1 ? len : LU_GLTF_MAX_STRING_SIZE - 1);
					}
					if (pMimeType != NULL)
					{
						int32_t len = (int32_t)strlen(pMimeType);
						memcpy(image.mimeType, pMimeType, len < LU_GLTF_MAX_STRING_SIZE - 1 ? len : LU_GLTF_MAX_STRING_SIZE - 1);
					}

					gltf.images.push(image);
				}
			}

			// Materials
			const JSONArray* pMaterials = json::GetArrayFromObject(table, *pRoot, "materials");
			if (pMaterials != NULL)
			{
				for (uint32_t index = 0; index < json::GetArrayElementCount(*pMaterials); ++index)
				{
					const JSONObject* pMaterial = json::GetObjectFromArray(table, *pMaterials, index);
					if (pMaterial == NULL) continue;
					if (!gltf.materials.isInitialized()) gltf.materials.setAllocator(&allocator);
					GLTFMaterial material;

					material.alphaCutoff = json::GetFloatFromObject(table, *pMaterial, "alphaCutoff");
					material.doubleSided = json::GetBoolFromObject(table, *pMaterial, "doubleSided");
					material.alphaMode = GLTF_ALPHA_OPAQUE;

					const JSONArray* pEmissiveFactor = json::GetArrayFromObject(table, *pMaterial, "emissiveFactor");
					if (pEmissiveFactor != NULL)
					{
						material.emissiveFactor[0] = json::GetFloatFromArray(table, *pEmissiveFactor, 0);
						material.emissiveFactor[1] = json::GetFloatFromArray(table, *pEmissiveFactor, 1);
						material.emissiveFactor[2] = json::GetFloatFromArray(table, *pEmissiveFactor, 2);
					}
					else
					{
						material.emissiveFactor[0] = 0.0f;
						material.emissiveFactor[1] = 0.0f;
						material.emissiveFactor[2] = 0.0f;
					}

					const JSONObject* pPBRMetallicRoughness = json::GetObjectFromObject(table, *pMaterial, "pbrMetallicRoughness");
					if (pPBRMetallicRoughness != NULL)
					{
						material.pbrMetallicRoughness.metallicFactor = json::GetFloatFromObject(table, *pPBRMetallicRoughness, "metallicFactor", 1.0f);
						material.pbrMetallicRoughness.roughnessFactor = json::GetFloatFromObject(table, *pPBRMetallicRoughness, "roughnessFactor", 1.0f);

						const JSONArray* pBaseColorFactor = json::GetArrayFromObject(table, *pPBRMetallicRoughness, "baseColorFactor");
						if (pBaseColorFactor != NULL)
						{
							material.pbrMetallicRoughness.baseColorFactor[0] = json::GetFloatFromArray(table, *pBaseColorFactor, 0, 1.0f);
							material.pbrMetallicRoughness.baseColorFactor[1] = json::GetFloatFromArray(table, *pBaseColorFactor, 1, 1.0f);
							material.pbrMetallicRoughness.baseColorFactor[2] = json::GetFloatFromArray(table, *pBaseColorFactor, 2, 1.0f);
							material.pbrMetallicRoughness.baseColorFactor[3] = json::GetFloatFromArray(table, *pBaseColorFactor, 3, 1.0f);
						}
						else
						{
							material.pbrMetallicRoughness.baseColorFactor[0] = 1.0f;
							material.pbrMetallicRoughness.baseColorFactor[1] = 1.0f;
							material.pbrMetallicRoughness.baseColorFactor[2] = 1.0f;
							material.pbrMetallicRoughness.baseColorFactor[3] = 1.0f;
						}

						const JSONObject* pBaseColorTexture = json::GetObjectFromObject(table, *pPBRMetallicRoughness, "baseColorTexture");
						if (pBaseColorTexture != NULL)
						{
							material.pbrMetallicRoughness.baseColorTexture.index = json::GetIntFromObject(table, *pBaseColorTexture, "index");
							material.pbrMetallicRoughness.baseColorTexture.texCoord = json::GetIntFromObject(table, *pBaseColorTexture, "texCoord");
						}

						const JSONObject* pMetallicRoughnessTexture = json::GetObjectFromObject(table, *pPBRMetallicRoughness, "metallicRoughnessTexture");
						if (pMetallicRoughnessTexture != NULL)
						{
							material.pbrMetallicRoughness.metallicRoughnessTexture.index = json::GetIntFromObject(table, *pMetallicRoughnessTexture, "index");
							material.pbrMetallicRoughness.metallicRoughnessTexture.texCoord = json::GetIntFromObject(table, *pMetallicRoughnessTexture, "texCoord");
						}

					}

					const JSONObject* pNormalTexture = json::GetObjectFromObject(table, *pMaterial, "normalTexture");
					if (pNormalTexture != NULL)
					{
						material.normalTexture.index = json::GetIntFromObject(table, *pNormalTexture, "index");
						material.normalTexture.texCoord = json::GetIntFromObject(table, *pNormalTexture, "texCoord");
						material.normalTexture.scale = json::GetFloatFromObject(table, *pNormalTexture, "scale");
					}
					const JSONObject* pOcclusionTexture = json::GetObjectFromObject(table, *pMaterial, "occlusionTexture");
					if (pOcclusionTexture != NULL)
					{
						material.occlusionTexture.index = json::GetIntFromObject(table, *pOcclusionTexture, "index");
						material.occlusionTexture.texCoord = json::GetIntFromObject(table, *pOcclusionTexture, "texCoord");
						material.occlusionTexture.strength = json::GetFloatFromObject(table, *pOcclusionTexture, "strength");
					}
					const JSONObject* pEmissiveTexture = json::GetObjectFromObject(table, *pMaterial, "emissiveTexture");
					if (pEmissiveTexture != NULL)
					{
						material.emissiveTexture.index = json::GetIntFromObject(table, *pEmissiveTexture, "index");
						material.emissiveTexture.texCoord = json::GetIntFromObject(table, *pEmissiveTexture, "texCoord");
					}
					
					const char* pAlphaMode = json::GetStringFromObject(table, *pMaterial, "alphaMode");
					if (pAlphaMode != NULL)
					{
						if (pAlphaMode[0] == 'M') material.alphaMode = GLTF_ALPHA_MASK;
						else if (pAlphaMode[0] == 'B') material.alphaMode = GLTF_ALPHA_BLEND;
					}

					gltf.materials.push(material);
				}
			}

			// Meshes
			const JSONArray* pMeshes = json::GetArrayFromObject(table, *pRoot, "meshes");
			if (pMeshes != NULL)
			{
				for (uint32_t index = 0; index < json::GetArrayElementCount(*pMeshes); ++index)
				{
					const JSONObject* pMesh = json::GetObjectFromArray(table, *pMeshes, index);
					if (pMesh == NULL) continue;
					if (!gltf.meshes.isInitialized()) gltf.meshes.setAllocator(&allocator);
					GLTFMesh& mesh = *gltf.meshes.alloc();

					LU_NEW(&mesh) GLTFMesh;

					const JSONArray* pPrimitives = json::GetArrayFromObject(table, *pMesh, "primitives");
					if (pPrimitives != NULL)
					{
						for (uint32_t i = 0; i < json::GetArrayElementCount(*pPrimitives); ++i)
						{
							const JSONObject* pPrimitive = json::GetObjectFromArray(table, *pPrimitives, i);
							if (pPrimitive == NULL) continue;
							if (!mesh.primitive.isInitialized()) mesh.primitive.setAllocator(&allocator);
							GLTFPrimitive& primitive = *mesh.primitive.alloc();

							LU_NEW(&primitive) GLTFPrimitive;

							if (!primitive.attributes.isInitialized()) primitive.attributes.setAllocator(&allocator);

							const JSONObject* pAttributes = json::GetObjectFromObject(table, *pPrimitive, "attributes");
							for (uint32_t j = 0, l = pAttributes->keys.getSize(); j < l; ++j)
							{
								const JSONKey& key = pAttributes->keys.constAt(j);
								GLTFAttribSemantic& attrib = *primitive.attributes.alloc();
								if (strcmp(key.key, "NORMAL") == 0) attrib = GLTF_ATTRIB_NORMAL;
								else if (strcmp(key.key, "TANGENT") == 0) attrib = GLTF_ATTRIB_TANGENT;
								else if (strcmp(key.key, "TEXCOORD_0") == 0) attrib = GLTF_ATTRIB_TEXCOORD_0;
								else if (strcmp(key.key, "TEXCOORD_1") == 0) attrib = GLTF_ATTRIB_TEXCOORD_1;
								else if (strcmp(key.key, "COLORS_0") == 0) attrib = GLTF_ATTRIB_COLORS_0;
								else if (strcmp(key.key, "JOINTS_0") == 0) attrib = GLTF_ATTRIB_JOINTS_0;
								else if (strcmp(key.key, "WEIGHTS_0") == 0) attrib = GLTF_ATTRIB_WEIGHTS_0;
							}
							primitive.indices = json::GetIntFromObject(table, *pPrimitive, "indices");
							primitive.material = json::GetIntFromObject(table, *pPrimitive, "material");
							int32_t mode = json::GetIntFromObject(table, *pPrimitive, "mode");
							if (mode == 0) primitive.mode = GLTF_PRIMITIVE_POINTS;
							else if (mode == 1) primitive.mode = GLTF_PRIMITIVE_LINES;
							else if (mode == 2) primitive.mode = GLTF_PRIMITIVE_LINE_LOOP;
							else if (mode == 3) primitive.mode = GLTF_PRIMITIVE_LINE_STRIP;
							else if (mode == 4) primitive.mode = GLTF_PRIMITIVE_TRIANGLES;
							else if (mode == 5) primitive.mode = GLTF_PRIMITIVE_TRIANGLE_STRIP;
							else if (mode == 6) primitive.mode = GLTF_PRIMITIVE_TRIANGLE_FAN;

							const JSONArray* pTargets = json::GetArrayFromObject(table, *pPrimitive, "targets");
							if (pTargets == NULL) continue;
							if (!primitive.targets.isInitialized()) primitive.targets.setAllocator(&allocator);

							for (uint32_t j = 0; j < json::GetArrayElementCount(*pTargets); ++j)
							{
								const JSONObject* pTarget = json::GetObjectFromArray(table, *pTargets, j);
								if (pTarget == NULL) continue;
								GLTFPrimitiveTarget& target = *primitive.targets.alloc();
								for (uint32_t x = 0, l = pTarget->keys.getSize(); x < l; ++x)
								{
									const JSONKey& key = pTarget->keys.constAt(x);
									if (strcmp(key.key, "POSITION")) target.targets[target.length++] = GLTF_ATTRIB_POSITION;
									else if (strcmp(key.key, "NORMAL")) target.targets[target.length++] = GLTF_ATTRIB_NORMAL;
									else if (strcmp(key.key, "TANGENT")) target.targets[target.length++] = GLTF_ATTRIB_TANGENT;
								}
							}
						}
					}

					const JSONArray* pWeights = json::GetArrayFromObject(table, *pMesh, "weights");
					if (pWeights != NULL)
					{
						if (!mesh.weights.isInitialized()) mesh.weights.setAllocator(&allocator);
						for (uint32_t i = 0; i < json::GetArrayElementCount(*pWeights); ++i)
						{
							mesh.weights.push(json::GetFloatFromArray(table, *pWeights, i));
						}
					}
				}
			}

			return true;
		}
	}

	return false;
}
