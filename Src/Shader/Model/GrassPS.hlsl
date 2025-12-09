// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"

// IN
#define PS_INPUT VertexToPixelLit

// PS
#include "../Common/Pixel/PixelShader3DHeader.hlsli"

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{  
    float g_time;           // 時間
    float g_wind_strength;  // 揺れの強さ
    float g_frequency;      // 揺れの周期
    float g_alpha;          // 透過値
}

// main関数
float4 main(PS_INPUT PSInput) : SV_TARGET0
{
    // V座標（縦方向）に基づいた揺れ係数
    float h_factor = 1.0f - PSInput.uv.y;
    
    // X座標（U）によって位相をずらすことで、隣接する草が同じタイミングで揺れるのを防ぐ
    float offset = sin(g_time * 3.0f + PSInput.uv.x * g_frequency) * g_wind_strength;

    // 係数を乗算して、先端ほど強く揺れるオフセットを計算
    float windOffsetU = offset * h_factor;

    // 新しいUV座標の計算
    float2 newUV = PSInput.uv;
    newUV.x += windOffsetU;

    // 新しいUVを使ってテクスチャをサンプリング
    float4 color = diffuseMapTexture.Sample(diffuseMapSampler, newUV);

    // アルファテスト（ディスカード）
    if (color.a < 0.01f)
    {
        discard;
    }
    
    // 透過値の適用
    color.a = g_alpha;

    return color;
}
