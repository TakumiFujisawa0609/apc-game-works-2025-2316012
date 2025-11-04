#include "../Common/Pixel/PixelShader2DHeader.hlsli"

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

// 定数バッファ：スロット4番目
cbuffer cbParam : register(b4)
{
    float4 g_color;         // 色
    float g_distance;       // 画素の距離
    float2 g_screen_size;   // スクリーンサイズ
    float g_flash_power;    // フラッシュの強さ
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{   
    // UV値取得
    float2 uv = PSInput.uv;
    
    // 色の取得
    float4 color = tex.Sample(texSampler, uv);

    // 魚眼
    //-------------------------------------------------------------------------
    // 中心からのベクトル
    float2 offset = uv - CENTER;

    // 分割領域の大きさで正規化
    offset /= CENTER;

    // 距離を計算
    float r = length(offset);

    // 魚眼効果樽型歪曲）
    float k = 0.1; // 歪みの強さ（大きくすると歪む）
    float scale = 1.0 + k * r * r;

    offset /= scale;
    //-------------------------------------------------------------------------
    
    // ブラー
    // UV座標に戻す
    uv = CENTER + offset * CENTER;
    
    // 画面にブラーを掛ける
    float2 radius = float2(g_distance / g_screen_size.x, g_distance / g_screen_size.y); // ブラー半径（UVスケール）

    // 全方向に向けてぼかす
    for (float d = 0.0f; d < TWO_PI; d += TWO_PI / DIRECTIONS)
    {
        float2 dir = float2(cos(d), sin(d)); // 単位方向ベクトル

        for (float i = 1.0f; i <= QUALITY; i += 1.0f)
        {
            float2 offset = dir * radius * (i / QUALITY); // ブラーの段階的距離
            color += tex.Sample(texSampler, uv + offset);
        }
    }

    // 平均化
    color /= (1.0f + DIRECTIONS * QUALITY);
    //-------------------------------------------------------------------------
    
    // フラッシュ
    // UVから中心までの距離を計算
    float dist = distance(uv, CENTER);

    // 色の強さを距離に応じて調整（端に行くほど強い）
    float factor = saturate(dist * 2.0) * g_flash_power;
    
    //色を追加
    float3 rgb = lerp(color.rgb, FLASH_COLOR, factor);
    
    // 色を返す
    return float4(rgb, color.a);
}