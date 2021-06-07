//
//  MTCommon.h
//  MetalDemo
//
//  Created by jfdreamyang on 2020/9/27.
//

#ifndef MTCommon_h
#define MTCommon_h

#include <simd/simd.h>

typedef struct
{
    vector_float4 position;
    vector_float2 textureCoordinate;
} LYVertex;


typedef struct {
    matrix_float3x3 matrix;
    vector_float3 offset;
} LYConvertMatrix;



typedef enum LYVertexInputIndex
{
    LYVertexInputIndexVertices     = 0,
    LYVertexInputIndexMatrix       = 1,
    LYVertexInputIndexRotation     = 2,
    LYVertexInputIndexBeauty       = 3
} LYVertexInputIndex;


typedef enum LYFragmentBufferIndex
{
    LYFragmentInputIndexMatrix     = 0,
} LYFragmentBufferIndex;


typedef enum LYFragmentTextureIndex
{
    LYFragmentTextureIndexTextureY     = 0,
    LYFragmentTextureIndexTextureUV     = 1,
} LYFragmentTextureIndex;




#endif /* MTCommon_h */
