#include "../Common/Pixel/PixelShader2DHeader.hlsli"
#include "../Common/Pixel/PixelShaderCommonFunction.hlsli"

// 画面中心
static float2 CENTER = { 0.5f, 0.5f };

// 方向数
static float DIRECTIONS = 16.0f;

// 各方向でのサンプル数
static float QUALITY = 3.0f;

// 2π
static float TWO_PI = 6.28318530718f;

// フラッシュの色
static float3 FLASH_COLOR = { 0.0f, 0.0f, 0.0f };

// グレースケール
static float3 GRAY_SCALE = { 0.299, 0.587, 0.114 };

// 定数バッファ：スロット4番目
cbuffer cbParam : register(b4)
{
    float g_time;
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // UV値取得
    float2 uv = PSInput.uv;
    
    // UV座標とテクスチャを参照して、最適な色を取得する
    float4 color = tex.Sample(texSampler, uv);
    
    // シンプルな砂嵐のノイズ生成
    float2 noiseUv = uv * 1000.0 + g_time * 50.0;
    float noise = Rand(floor(noiseUv)); // 各ノイズブロックで同じ乱数を生成
    
    // 色の出力
    return float4(noise, noise, noise, 1.0f);

}