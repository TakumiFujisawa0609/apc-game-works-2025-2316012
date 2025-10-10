#include "../common/Pixel/PixelShader2DHeader.hlsli"

// 定数バッファ（DXライブラリで設定可能）
cbuffer ConstantBuffer : register(b4)
{
    float4 g_color;         // 通常は未使用（DXライブラリの仕様）
    float g_time;           // 経過時間（DXライブラリでSetShaderConstantFで渡す）
    float g_glitch_strength;// グリッチの強度（0.0～1.0くらい）
    float g_glitch_speed;   // グリッチの変化速度
    float g_none;
};

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    //float2 uv = PSInput.uv;
    
    //float noiseSeed = floor(uv,.y)
    
    // テクスチャの色を返す
    return tex.Sample(texSampler, PSInput.uv);
}