#include "../Common/Pixel/PixelShader2DHeader.hlsli"

// 円周率
static const float PI = 3.14159;

// 炎の色
static const float3 FIRE_COLOR = float3(0.6f, 0.0f, 0.0f);

// ノイズ用マトリックス
static const float3x3 NOISE_MATRIX = float3x3
(
    0.00, 0.80, 0.60,
    -0.80, 0.36, -0.48,
    -0.60, -0.48, 0.64
);

// スロット4番目(b4)に登録
cbuffer cbParam : register(b4)
{
    float g_time;
    //float g_resolution;
    float dummy;
    
    float2 g_mouse_pos; // マウス位置
};

// サブテクスチャ
Texture2D subTex : register(t1);

// サブサンプラー
SamplerState subTexSampler : register(s1);

float hash(float n)
{
    return frac(sin(n) * 43758.5453);
}

float noise(float3 x)
{
    float3 p = floor(x);
    float3 f = frac(x);
    f = f * f * (3.0 - 2.0 * f);

    float n = p.x + p.y * 57.0 + 113.0 * p.z;

    return lerp(
        lerp(lerp(hash(n + 0.0), hash(n + 1.0), f.x),
             lerp(hash(n + 57.0), hash(n + 58.0), f.x), f.y),
        lerp(lerp(hash(n + 113.0), hash(n + 114.0), f.x),
             lerp(hash(n + 170.0), hash(n + 171.0), f.x), f.y),
        f.z
    );
}

float myfbm(float3 p)
{
    float f = 0.0;
    f = 0.5000 * noise(p);
    p = mul(NOISE_MATRIX, p) * 2.02;
    f += 0.2500 * noise(p);
    p = mul(NOISE_MATRIX, p) * 2.03;
    f += 0.1250 * noise(p);
    p = mul(NOISE_MATRIX, p) * 2.01;
    f += 0.0625 * noise(p);
    p = mul(NOISE_MATRIX, p) * 2.04;
    f += 0.03125 * noise(p);
    p = mul(NOISE_MATRIX, p) * 2.03;
    f += 0.015625 * noise(p);
    return f;
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // UV値取得
    float2 uv = PSInput.uv;
    
    // ノイズ計算
    float3 dir = float3(0.0, 0.0, 1.0);
    float3 p = 4.0 * float3(uv, 0.0) + g_time * dir;
    float x = myfbm(p);

    // パラメータ取得
    float2 val = g_mouse_pos.xy;
    
    // 境界のシェーピング
    x = smoothstep(-val.y + val.x, 1.0 - val.y + 1.0 - val.x, x);
    
    // テクスチャのサンプリング
    float3 mainColor = tex.Sample(texSampler, uv).rgb;
    float3 subColor = subTex.Sample(subTexSampler, uv).rgb;

    // 合成
    float3 color = lerp(mainColor, subColor + FIRE_COLOR, x);
    float k = (1.0 - pow(0.5 + 0.5 * cos(PI * (x - 0.5)), 3.0));
    color += lerp(FIRE_COLOR, color, k);
 
    return float4(color, 1.0);
}