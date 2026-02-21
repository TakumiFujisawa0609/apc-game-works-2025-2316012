#include "../common/Pixel/PixelShader2DHeader.hlsli"

// ノイズの大きさ
static const float NOISE_SCALE = 3758.5453f;

// 炎の色
static const float3 FIRE_COLOR = float3(1.5f, 0.5f, 0.0f);

// 定数バッファ
cbuffer ConstantBuffer : register(b4)
{
    float g_time;           // 時間
    float2 g_texture_size;  // テクスチャサイズ
    float dummy; 
};

// 乱数
float Hash(float2 p)
{
    return frac(sin(dot(p, float2(12.9898f, 78.233f))) * NOISE_SCALE);
}

// ノイズ
float Noise(float2 p)
{
    float2 i = floor(p);
    float2 f = frac(p);
    f = f * f * (3.0f - 2.0f * f);

    float a = Hash(i);
    float b = Hash(i + float2(1.0f, 0.0f));
    float c = Hash(i + float2(0.0f, 1.0f));
    float d = Hash(i + float2(1.0f, 1.0f));

    return lerp(lerp(a, b, f.x), lerp(c, d, f.x), f.y);
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{   
    // テクスチャサイズを考慮したUV計算
    float2 uv = PSInput.uv;
    float aspect = g_texture_size.x / g_texture_size.y;
    float2 centeredUv = (uv - 0.5f) * float2(aspect, 1.0f);
    
    // 時間による進行度
    float time = g_time;
    
    // 表示したい画像
    float4 mainColor = tex.Sample(texSampler, uv);

    // 透過している個所は破棄
    if (mainColor.a <= 0.0f)
    {
        discard;
    }
    
    // 燃焼エフェクトの境界計算
    float burnCoord = (centeredUv.x + centeredUv.y * 0.3f) + 4.0f - time;
    float noiseEffect = Noise(centeredUv * 8.0f) * 0.4f;
    float d = 1.0f - (burnCoord + noiseEffect);
    
    // 光が到達されていない場所も無視
    if (d < 0.15f)
    {
        discard;
    }
    
    // 境界を数値化（0.45以下何もなし, 0.45から0.5炎, 0.5以上画像）
    float burnEdge = smoothstep(0.4f, 0.5f, d);
    float fireEdge = smoothstep(0.05f, 0.58f, d) * (1.0f - burnEdge);
    
    // 動的な火花の計算
    float spark = Noise(centeredUv * 50.0f - float2(time * 4.0f, 0.0f));
    float3 fireEffect = fireEdge * FIRE_COLOR * (0.8f + spark * 0.5f);
    
    // 色の合成
    float4 finalColor;
    finalColor.rgb = ((mainColor.rgb * burnEdge) + fireEffect) * mainColor.a;
    finalColor.a = max(mainColor.a * burnEdge, fireEdge);
    
    // 色の出力
    return finalColor;
}