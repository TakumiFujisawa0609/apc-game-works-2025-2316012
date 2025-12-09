#include "../Common/Pixel/PixelShader2DHeader.hlsli"

// スロット4番目(b4)に登録
cbuffer cbParam : register(b4)
{
    float2 offset;
    float2 dummy;
};

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // UV値取得
    float2 uv = PSInput.uv;
    
    // 出力カラー
    float4 color = { 0.0f, 0.0f, 0.0f, 1.0f };
    
    // 赤成分を少し左上から取得
    color.r = tex.Sample(texSampler, uv - offset).r;

    // 緑成分はそのまま中心から取得
    color.g = tex.Sample(texSampler, uv).g;

    // 青成分を少し右下から取得
    color.b = tex.Sample(texSampler, uv + offset).b;

    // 結果の色を出力
    return color;
}