//
//  NV12.metal
//  MetalDemo
//
//  Created by jfdreamyang on 2020/9/27.
//

#include <metal_stdlib>
#import "MTCommon.h"
using namespace metal;


typedef struct {
    float4 renderedCoordinate [[position]];
    float2 textureCoordinate;
} TextureMappingVertex;

vertex TextureMappingVertex // 返回给片元着色器的结构体
vertexShader(uint vertex_id [[ vertex_id ]], // vertex_id是顶点shader每次处理的index，用于定位当前的顶点
             constant float4x2* textureCoordinates [[buffer(LYVertexInputIndexMatrix)]],
             constant float4x4* renderedCoordinates [[buffer(LYVertexInputIndexVertices)]]) { // buffer表明是缓存数据，0是索引
//    float4x4 renderedCoordinates = float4x4(float4( -1.0, -1.0, 0.0, 1.0 ),
//                                            float4(  1.0, -1.0, 0.0, 1.0 ),
//                                            float4( -1.0,  1.0, 0.0, 1.0 ),
//                                            float4(  1.0,  1.0, 0.0, 1.0 ));
    TextureMappingVertex outVertex;
    outVertex.renderedCoordinate = (*renderedCoordinates)[vertex_id];
    outVertex.textureCoordinate = (*textureCoordinates)[vertex_id];
    
    return outVertex;
}

float nv12_hardLight(float color) {
    if (color <= 0.5)
        color = color * color * 2.0;
    else
        color = 1.0 - ((1.0 - color)*(1.0 - color) * 2.0);
    return color;
}


fragment float4
samplingShader(TextureMappingVertex input [[stage_in]], // stage_in表示这个数据来自光栅化。（光栅化是顶点处理之后的步骤，业务层无法修改）
               texture2d<float> textureY [[ texture(LYFragmentTextureIndexTextureY) ]], // texture表明是纹理数据，LYFragmentTextureIndexTextureY是索引
               texture2d<float> textureUV [[ texture(LYFragmentTextureIndexTextureUV) ]], // texture表明是纹理数据，LYFragmentTextureIndexTextureUV是索引
               constant LYConvertMatrix *convertMatrix [[ buffer(LYFragmentInputIndexMatrix) ]],
                    //buffer表明是缓存数据，LYFragmentInputIndexMatrix是索引
               constant float *parameters[[buffer(LYVertexInputIndexBeauty)]])
{
    constexpr sampler textureSampler (mag_filter::linear,
                                      min_filter::linear); // sampler是采样器
    
    float3 yuv = float3(textureY.sample(textureSampler, input.textureCoordinate).r,
                        textureUV.sample(textureSampler, input.textureCoordinate).rg);
    float4 rgba = float4(convertMatrix->matrix * (yuv + convertMatrix->offset), 1.0);
    float beautyEnable = *(parameters + 7);
    if (fabs(beautyEnable) <= 0.00001){
        return rgba;
    }
    float2 textureCoordinate = input.textureCoordinate;
    vector_float2 singleStepOffset = *parameters;
    vector_float4 params = *(parameters + 2);
    float brightness = *(parameters + 6);
    const vector_float3 W = vector_float3(0.299, 0.587, 0.114);
    const float3x3 saturateMatrix = float3x3(
                                             float3(1.1102, -0.0598, -0.061),
                                             float3(-0.0774, 1.0826, -0.1186),
                                             float3(-0.0228, -0.0228, 1.1772)
                                             );

    vector_float2 blurCoordinates[24];
    vector_float3 centralColor = rgba.rgb;
    
    blurCoordinates[0] = textureCoordinate.xy + singleStepOffset * vector_float2(0.0, -10.0);
    blurCoordinates[1] = textureCoordinate.xy + singleStepOffset * vector_float2(0.0, 10.0);
    blurCoordinates[2] = textureCoordinate.xy + singleStepOffset * vector_float2(-10.0, 0.0);
    blurCoordinates[3] = textureCoordinate.xy + singleStepOffset * vector_float2(10.0, 0.0);
    blurCoordinates[4] = textureCoordinate.xy + singleStepOffset * vector_float2(5.0, -8.0);
    blurCoordinates[5] = textureCoordinate.xy + singleStepOffset * vector_float2(5.0, 8.0);
    blurCoordinates[6] = textureCoordinate.xy + singleStepOffset * vector_float2(-5.0, 8.0);
    blurCoordinates[7] = textureCoordinate.xy + singleStepOffset * vector_float2(-5.0, -8.0);
    blurCoordinates[8] = textureCoordinate.xy + singleStepOffset * vector_float2(8.0, -5.0);
    blurCoordinates[9] = textureCoordinate.xy + singleStepOffset * vector_float2(8.0, 5.0);
    blurCoordinates[10] = textureCoordinate.xy + singleStepOffset * vector_float2(-8.0, 5.0);
    blurCoordinates[11] = textureCoordinate.xy + singleStepOffset * vector_float2(-8.0, -5.0);
    blurCoordinates[12] = textureCoordinate.xy + singleStepOffset * vector_float2(0.0, -6.0);
    blurCoordinates[13] = textureCoordinate.xy + singleStepOffset * vector_float2(0.0, 6.0);
    blurCoordinates[14] = textureCoordinate.xy + singleStepOffset * vector_float2(6.0, 0.0);
    blurCoordinates[15] = textureCoordinate.xy + singleStepOffset * vector_float2(-6.0, 0.0);
    blurCoordinates[16] = textureCoordinate.xy + singleStepOffset * vector_float2(-4.0, -4.0);
    blurCoordinates[17] = textureCoordinate.xy + singleStepOffset * vector_float2(-4.0, 4.0);
    blurCoordinates[18] = textureCoordinate.xy + singleStepOffset * vector_float2(4.0, -4.0);
    blurCoordinates[19] = textureCoordinate.xy + singleStepOffset * vector_float2(4.0, 4.0);
    blurCoordinates[20] = textureCoordinate.xy + singleStepOffset * vector_float2(-2.0, -2.0);
    blurCoordinates[21] = textureCoordinate.xy + singleStepOffset * vector_float2(-2.0, 2.0);
    blurCoordinates[22] = textureCoordinate.xy + singleStepOffset * vector_float2(2.0, -2.0);
    blurCoordinates[23] = textureCoordinate.xy + singleStepOffset * vector_float2(2.0, 2.0);
    
    float sampleColor = centralColor.g * 22.0;
    for (int i=0; i < 24; i++){
        yuv = float3(textureY.sample(textureSampler, blurCoordinates[i]).r,
                     textureUV.sample(textureSampler, blurCoordinates[i]).rg);
        rgba = float4(convertMatrix->matrix * (yuv + convertMatrix->offset), 1.0);
        float multiplier = 1.0;
        if(i >= 12 && i <= 19){
            multiplier = 2.0;
        }
        else if (i >= 20){
            multiplier = 3.0;
        }
        sampleColor += rgba.g * multiplier;
    }
    sampleColor = sampleColor / 62.0;
    float highPass = centralColor.g - sampleColor + 0.5;
    for (int i = 0; i < 5; i++) {
        highPass = nv12_hardLight(highPass);
    }
    float lumance = dot(centralColor, W);
    float alpha = pow(lumance, params.r);
    vector_float3 smoothColor = centralColor + (centralColor-vector_float3(highPass))*alpha*0.1;
    smoothColor.r = clamp(pow(smoothColor.r, params.g), 0.0, 1.0);
    smoothColor.g = clamp(pow(smoothColor.g, params.g), 0.0, 1.0);
    smoothColor.b = clamp(pow(smoothColor.b, params.g), 0.0, 1.0);

    vector_float3 lvse = vector_float3(1.0)-(vector_float3(1.0)-smoothColor)*(vector_float3(1.0)-centralColor);
    vector_float3 bianliang = max(smoothColor, centralColor);
    vector_float3 rouguang = 2.0*centralColor*smoothColor + centralColor*centralColor - 2.0*centralColor*centralColor*smoothColor;

    float4 gl_FragColor = vector_float4(mix(centralColor, lvse, alpha), 1.0);
    gl_FragColor.rgb = mix(gl_FragColor.rgb, bianliang, alpha);
    gl_FragColor.rgb = mix(gl_FragColor.rgb, rouguang, params.b);

    float3 satcolor = gl_FragColor.rgb * saturateMatrix;
    gl_FragColor.rgb = mix(gl_FragColor.rgb, satcolor, params.a);
    gl_FragColor.rgb = vector_float3(gl_FragColor.rgb + vector_float3(brightness));
    return gl_FragColor;
}
