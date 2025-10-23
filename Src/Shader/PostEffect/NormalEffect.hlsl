#include "../Common/Pixel/PixelShader2DHeader.hlsli"

// 定数バッファ：スロット4番目
cbuffer cbParam : register(b4)
{
    float4 g_color;
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // UV値取得
    float2 uv = PSInput.uv;

    // こ画面の中心座標取得
    float2 center = { 0.5, 0.5 };

    // 中心からのベクトル
    float2 offset = uv - center;

    // 分割領域の大きさで正規化
    offset /= center;

    // 距離を計算
    float r = length(offset);

    // 魚眼効果（樽型歪曲）
    float k = 0.4; // 歪みの強さ（大きくすると歪む）
    float scale = 1.0 + k * r * r;

    offset /= scale;

    // UV座標に戻す
    uv = center + offset * center;

    // テクスチャの色を取得
    return tex.Sample(texSampler, uv);

}