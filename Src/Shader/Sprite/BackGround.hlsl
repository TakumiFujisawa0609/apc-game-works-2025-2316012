#include "../common/Pixel/PixelShader2DHeader.hlsli"

// 定数バッファ
cbuffer ConstantBuffer : register(b4)
{
    float g_time;           // 時間
    float g_strength;       // 歪み強さ
    float g_spead;          // 速度
    float dummy;
};

// 簡易的な擬似ノイズ関数
float2 GetRandomDir(float2 p)
{
    // 座標からランダムな方向ベクトルを生成
    float x = frac(sin(dot(p, float2(127.1, 311.7))) * 43758.5453);
    float y = frac(sin(dot(p, float2(269.5, 183.3))) * 43758.5453);
    return float2(x, y) * 2.0 - 1.0;
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // UV値を取得
    float2 uv = PSInput.uv;
    
    // 時間経過によって変化する歪み成分を計算
    float2 distortion;
    distortion.x = sin(uv.y * 10.0 + g_time * g_spead) * cos(uv.x * 8.0 + g_time * g_spead * 0.5);
    distortion.y = cos(uv.x * 12.0 + g_time * g_spead * 0.8) * sin(uv.y * 7.0 + g_time * g_spead);
    
    // 計算した歪みを元のUV座標に加算
    float2 distortedUv = uv + distortion * g_strength;
    
    // 歪ませた座標でテクスチャをサンプリング
    float4 color = tex.Sample(texSampler, distortedUv);
    
    // 色の出力
    return color;
}