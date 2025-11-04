//VS・PS共通
#include "../Common/VertexToPixelHeader.hlsli"

//IN
#define PS_INPUT VertexToPixelLit

// 法線マップ有効フラグ
#define BUMPMAP 1

//PS
#include "../Common/Pixel/PixelShader3DHeader.hlsli"

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
    // UV座標を取得
    float2 uv = PSInput.uv;
    
    // 色の取得
    float4 color = diffuseMapTexture.Sample(diffuseMapSampler, uv);
    
    // 法線マップから色を取得し、0～1を-1～1に変換
    const float3 tanNormal = normalize(normalMapTexture.Sample(normalMapSampler, uv).xyz * 2 - 1);
    
    // ビュー座標系に変換する逆行列を取得
    const float3x3 tangentViewMat = transpose(float3x3(normalize(PSInput.tan), normalize(PSInput.bin), normalize(PSInput.normal)));

	// ベクトルをビュー座標系に変換
    const float3 normal = normalize(mul(tangentViewMat, tanNormal));
    
    // ライトベクトルを正規化
    float3 lightDir = normalize(g_light_dir.xyz);
    
    // ライティング計算
    float lighting = max(0.0f, dot(normal, -lightDir));
    
    // 環境光とディフューズ光の加算
    float3 ambient = color.rgb * g_ambient_color.rgb;
    float3 diffuse = color.rgb * lighting;
    color.xyz = saturate(ambient + diffuse);
    
    return color;
}