// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"
#include "../Common/Pixel/PixelShaderCommonFunction.hlsli"
// IN
#define PS_INPUT VertexToPixelLit

// シャドウマップの有効フラグ
#define SHADOWMAP 1

// PS
#include "../Common/Pixel/PixelShader3DHeader.hlsli"

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
    float g_time;       // 時間
    float g_strength;   // 強さ
    float2 dummy;
}

// 乱数生成関数
float random(float2 st)
{
    return frac(sin(dot(st.xy, float2(12.9898, 78.233))) * 43758.5453123);
}

// main関数
float4 main(PS_INPUT PSInput) : SV_TARGET0
{
    // UV値の取得
    float2 uv = PSInput.uv;
    float4 color = diffuseMapTexture.Sample(diffuseMapSampler, uv);
    
    // ラインずれ
    float glitchLine = step(0.9, random(float2(floor(uv.y * 20.0), g_time)));
    float offset = glitchLine * (random(float2(g_time, g_time)) - 0.5) * 0.1 * g_strength;

    // 2. RGBずらし（色収差）
    float shift = 0.02 * g_strength;
    float r = diffuseMapTexture.Sample(diffuseMapSampler, uv + float2(offset + shift, 0)).r;
    float g = diffuseMapTexture.Sample(diffuseMapSampler, uv + float2(offset, 0)).g;
    float b = diffuseMapTexture.Sample(diffuseMapSampler, uv + float2(offset - shift, 0)).b;

    // 色の出力
    color.rgb = float3(r, g, b);
    return color;
}
