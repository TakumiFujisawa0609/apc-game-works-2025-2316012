// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"

// IN
#define PS_INPUT VertexToPixelLit

// PS
#include "../Common/Pixel/PixelShader3DHeader.hlsli"

// 定数バッファ
cbuffer cbParam : register(b4)
{
    float g_time;
    float g_amplitude;
    float2 dummy;
}

// main関数
float4 main(PS_INPUT PSInput) : SV_TARGET0
{
    // 揺れの強さを決定
    float waveStrength = 1.0f - PSInput.uv.y;
    
    // 揺れの値を計算
    float waveValue = sin(g_time * 5.0f + PSInput.uv.y * 10.0f);
    
    // 最終的な横移動の変位量
    float offset = waveValue * waveStrength * g_amplitude;

    // UV位置を取得
    float2 newUv = PSInput.uv;
    newUv.x += offset;
    
    // 色の取得
    float4 color = diffuseMapTexture.Sample(diffuseMapSampler, newUv);
    if (color.a < 0.01f)
    {
        discard;
    }
    
    return color;

}