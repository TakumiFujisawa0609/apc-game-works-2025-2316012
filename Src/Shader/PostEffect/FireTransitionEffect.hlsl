#include "../Common/Pixel/PixelShader2DHeader.hlsli"
#include "../Common/Pixel/PixelShaderCommonFunction.hlsli"

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
    float g_progress;
    float g_bokeh_level;
    float dummy;
};

// サブテクスチャ
Texture2D subTex : register(t1);

// サブサンプラー
SamplerState subTexSampler : register(s1);

// ノイズテクスチャ
Texture2D noiseTex : register(t2);

// ノイズサンプラー
SamplerState noiseTexSampler : register(s2);

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
    return f;
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    float2 uv = PSInput.uv;
    float3 p = 4.0 * float3(uv, 0.0) + g_time * float3(0, 0, 1);
    
    // ノイズの値を 0.0 ~ 1.0 に収まりやすく調整
    float x = myfbm(p);
    
    // ノイズに基づいたマスクを生成
    float mask = smoothstep(g_progress - g_bokeh_level, g_progress + g_bokeh_level, x);
    
    // 変化後のテクスチャを取得
    float3 mainColor = tex.Sample(texSampler, uv).rgb;
    
    // 変化前のテクスチャを取得
    float3 subColor = subTex.Sample(subTexSampler, uv).rgb;
    
    // テクスチャの合成
    float3 color = lerp(mainColor, subColor, mask);
    
    // 境界線部分を抽出して炎の色を加える
    float edge = 1.0 - abs(mask - 0.5) * 2.0;
    
    // 境界を光らせる
    edge = pow(max(0.0, edge), 3.0);
    color += FIRE_COLOR * edge * 2.0; 

    return float4(color, 1.0);
}