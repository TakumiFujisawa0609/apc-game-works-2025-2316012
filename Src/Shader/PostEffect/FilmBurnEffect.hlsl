#include "../Common/Pixel/PixelShader2DHeader.hlsli"

static const float PI = 3.14159;

// スロット4番目(b4)に登録
cbuffer cbParam : register(b4)
{
    float g_time;
    float g_total_time;
    float2 dummy;
};

// サブテクスチャ
Texture2D subTex : register(t1);

// サブサンプラー
SamplerState subTexSampler : register(s1);

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // UV値取得
    float2 uv = PSInput.uv;
    
    // 進行度を計算
    float progress = saturate(g_time / g_total_time);
    
    // 境界線を計算(初期位置は少し下げる)
    float th = smoothstep(0.0f, 1.0f, progress);

    // メインテクスチャのサンプリング
    float texValue = ((subTex.Sample(subTexSampler, uv).r - 0.5f) + 2.0f * uv.x) / 3.0f;
    
    float mask = smoothstep(th - 0.3f, th + 0.1f, texValue);
    float dist = smoothstep(th - 0.3f, th + 0.05f, texValue);
    float col = pow(smoothstep(th - 0.2f, th + 0.15f, texValue), 3.0f);

    float3 color = tex.Sample(texSampler, uv * (0.7f + pow(dist, 2.0f) * 0.3f)).rgb;
    float3 discolor = color * float3(0.8, 0.4, 0.2);
    
    return float4(lerp(discolor, color, col) * mask, 1.0);
}