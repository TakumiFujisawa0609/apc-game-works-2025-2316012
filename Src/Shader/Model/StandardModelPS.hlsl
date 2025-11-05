//VS・PS共通
#include "../Common/VertexToPixelHeader.hlsli"

//IN
#define PS_INPUT VertexToPixelLit

// 法線マップ有効フラグ
#define BUMPMAP 1

//PS
#include "../Common/Pixel/PixelShader3DHeader.hlsli"

static float3 POINT_LIGHT_COLOR = { 0.8f, 0.8f, 0.6f };
static float3 FOG_COLOR = { 1.0f, 0.0f, 0.0f };

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
    const float tiling = 0.01f;
    const float amount = 20.0f;
    
    // UV座標を取得
    float2 uv = PSInput.uv;

    // 色の取得     
    float4 color = diffuseMapTexture.Sample(diffuseMapSampler, uv);
    float3 material = color.rgb; // マテリアル
    
    //法線マップから色を取得し、0～1を-1～1に変換
    const float3 tanNormal = normalize(normalMapTexture.Sample(normalMapSampler, uv).xyz * 2 - 1);
    
    //ビュー座標系に変換する逆行列を取得
    const float3x3 tangentViewMat = transpose(float3x3(normalize(PSInput.tan), normalize(PSInput.bin), normalize(PSInput.normal)));

    //ベクトルをビュー座標系に変換
    const float3 normal = normalize(mul(tangentViewMat, tanNormal));

    // デバッグ修正 ノーマルマップの影響を無効化し、頂点の法線（ビュー座標系）をそのまま使用する
    //const float3 normal = normalize(PSInput.normal); // 既にビュー空間に変換されていると仮定
    
    // ライトベクトルを正規化
    float3 lightDir = normalize(g_light_dir.xyz);
    
    // ライティング計算
    float lighting = max(0.0f, dot(normal, -lightDir));
    
    // 環境光とディフューズ光の加算
    float3 ambient = material * g_ambient_color.rgb; // 環境光
    float3 diffuse = material * lighting; // 平行光源のディフューズ
  
    // ポイントライト
    float3 lightIntensity = diffuse + (PSInput.lightPower - ambient);
    
    // 最終的なライティング結果
    float3 finalLightColor = material * lightIntensity;
    finalLightColor = saturate(ambient + finalLightColor);
    
    //return float4(saturate(finalLightColor), color.a);
    
    // フォグの効果
    float fog = PSInput.fogFactor;
    float3 foggedColor = finalLightColor + FOG_COLOR * fog;

    return float4(saturate(foggedColor), color.a);
}