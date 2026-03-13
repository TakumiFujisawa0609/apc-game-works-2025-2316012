#include "../Common/Pixel/PixelShader2DHeader.hlsli"

// 円周率
static const float PI = 3.14159;

// セピアカラー
static const float3 SEPIA_COLOR = float3(0.8, 0.4, 0.2);

// スロット4番目(b4)に登録
cbuffer cbParam : register(b4)
{
    float g_time;
    float g_total_time;
    float3 g_sepia_color;
    float3 dummy;
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
    float rate = saturate(g_time / g_total_time);
    
    // しきい値を計算(初期位置は少し下げる)
    float th = smoothstep(0.0f, 1.0f, rate);

    // サブテクスチャの赤成分とUV座標を組み合わせてノイズ状の値を生成
    float texValue = ((subTex.Sample(subTexSampler, uv).r - 0.5f) + 2.0f * uv.x) / 3.0f;
    
    // 各種マスク・カラー値を計算
    float mask = smoothstep(th - 0.3f, th + 0.1f, texValue);    // 透明度を制御するマスク
    float dist = smoothstep(th - 0.3f, th + 0.05f, texValue);   // 歪み量を制御する値
    float col = pow(smoothstep(th - 0.2f, th + 0.15f, texValue), 3.0f); // 色の線形補間用の係数

    // メインテクスチャをサンプリングしつつUV座標を歪ます
    float3 color = tex.Sample(texSampler, uv * (0.7f + pow(dist, 2.0f) * 0.3f)).rgb;
    
    // 色調整（セピア調）
    float3 sepia = color * g_sepia_color;
    
    // 最終的な色味とマスクを組み合わせて出力
    return float4(lerp(sepia, color, col) * mask, 1.0);
}