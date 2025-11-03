//VS・PS共通
#include "../Common/VertexToPixelHeader.hlsli"

//IN
#define PS_INPUT VertexToPixelLit

// 法線マップ有効フラグ
#define BUMPMAP 1

// サブテクスチャの有効フラグ
//#define SUBTEXTUREMODE 7

//PS
#include "../Common/Pixel/PixelShader3DHeader.hlsli"

SamplerState subSampler : register(s3); // サブテクスチャ
Texture2D subTexture : register(t3); // サブテクスチャ

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
    float4 g_color;         // 色
    float3 g_light_dir;     // ライト方向
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
    
    return float4(1.0f, 0.0f, 0.0f, 1.0f);
    
    // 色の取得
    float4 color = diffuseMapTexture.Sample(diffuseMapSampler, uv);
    
    // TBN行列の構築
    //float3x3 TBN = float3x3(
    //    PSInput.tan.xyz,        // 接線ベクトル
    //    PSInput.bin.xyz,        // 従法線ベクトル
    //    PSInput.normal.xyz      // 法線ベクトル
    //);
    
    //// 法線マップのサンプリングと変換
    //float4 normalMapColor = normalMapTexture.Sample(normalMapSampler, uv);
    
    //// タンジェント空間での法線ベクトル
    //float3 tanVec = normalMapColor.xyz * 2.0f - 1.0f;
    
    //// ワールド空間へ変換
    //float3 world = normalize(mul(tanVec, TBN));
    
    //// ライトベクトルを正規化
    //float3 lightDir = normalize(g_light_dir.xyz);
    
    //// ライティング計算
    //float lighting = max(0.0f, dot(world, -lightDir));
    
    //// 環境光とディフューズ光の加算
    //float3 ambient = color.rgb * g_color.rgb * g_ambient_color.rgb;
    //float3 diffuse = color.rgb * g_color.rgb * lighting;
    //color.xyz = saturate(ambient + diffuse);
    
    // テクスチャのUV座標を生成
    float2 texUv = uv * tiling;
    
    // テクスチャ色の取得
    float4 texColor = subTexture.Sample(subSampler, texUv);
    
    float3 rgb = color.rgb;
    
    rgb.r += texColor.r;
    
    return float4(rgb, color.a);
    ///////////////////////////////////////////////////////////////////////////////////
    
    // 血の手のテクスチャのアルファ値を強度として利用
    float intensity = texColor.a;
    
    if (intensity > 0.01f)
    {
        return texColor;
    }
    return color;
    
    // 加算する血の色を計算
        float3 bloodEffect = texColor.rgb * intensity;
    
    // 元の色に血の色を足し合わせ、彩度をクランプ（1.0以下に収める）
    color.xyz = saturate(color.xyz + bloodEffect);
    
    // αブレンドでテクスチャを重ねる
    //color.rgb = lerp(color.rgb, texColor.rgb, texColor.a);
    
    // 最終的な色を返す
    return color;
}