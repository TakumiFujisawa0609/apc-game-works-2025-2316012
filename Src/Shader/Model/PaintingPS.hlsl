//VS・PS共通
#include "../Common/VertexToPixelHeader.hlsli"

//IN
#define PS_INPUT VertexToPixelLit

// 法線マップ有効フラグ
#define BUMPMAP 1

//PS
#include "../Common/Pixel/PixelShader3DHeader.hlsli"

static float3 FOG_COLOR = { 0.0f, 0.0f, 0.0f };

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
    float4 g_color; // 色
    float3 g_light_dir; // ライト方向
    float dummy;
    float3 g_ambient_color; // 環境光
    float dummy2;
}

float4 main(PS_INPUT PSInput) : SV_TARGET0
{
    float2 uv = PSInput.uv;
 
    // ベースカラー
    float4 texColor = diffuseMapTexture.Sample(diffuseMapSampler, uv);
    float3 material = texColor.rgb;
 
    // 法線計算
    const float3 tanNormal = normalize(normalMapTexture.Sample(normalMapSampler, uv).xyz * 2 - 1);
    const float3x3 tangentViewMat = transpose(float3x3(normalize(PSInput.tan), normalize(PSInput.bin), normalize(PSInput.normal)));
    const float3 normal = normalize(mul(tangentViewMat, tanNormal));
 
    // 光の方向
    float3 lightDir = normalize(g_light_dir.xyz);
    float NdotL = max(0.0f, dot(normal, -lightDir));
 
    // 環境光（暗いマテリアルを持ち上げる）
    float3 ambientBase = material * g_ambient_color.rgb;
    
    // ディフューズ
    float3 diffuse = material * NdotL;
 
    // 最終カラー 
    float3 litColor = saturate(ambientBase + diffuse);
 
    // フォグ適用
    float fogFactor = saturate(1.0f - PSInput.fogFactor); // 0=カメラ近, 1=遠
    float3 foggedColor = lerp(litColor, FOG_COLOR, fogFactor);

    return float4(foggedColor, texColor.a);
}