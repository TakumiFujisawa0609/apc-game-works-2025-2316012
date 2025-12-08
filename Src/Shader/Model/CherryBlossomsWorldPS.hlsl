//VS・PS共通
#include "../Common/VertexToPixelHeader.hlsli"
#include "../Common/Pixel/PixelShaderCommonFunction.hlsli"

//IN
#define PS_INPUT VertexToPixelLit

// 法線マップ有効フラグ
#define BUMPMAP 1

// サブテクスチャの有効フラグ
#define SUBTEXTUREMODE 7

//PS
#include "../Common/Pixel/PixelShader3DHeader.hlsli"

// スポットライトの角度
static float SPOT_LIGHT_ANGLE_COS = cos(radians(30.0f));

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
    float3 g_light_dir; // ライト方向
    float g_fog_start;
    
    float3 g_ambient_color; // 環境光
    float g_fog_end;
    
    float3 g_spot_light_pos; // スポットライト位置
    float g_is_light; // ライトの電源(0オフ、1オン)
    
    float3 g_spot_light_dir; // スポットライト方向
    float g_sakura_boost_amount; // 桜の強調量 (例: 1.0f ～ 5.0f)
    
    float3 g_camera_pos;
    float g_sakura_target_hue; // 桜の目標色相 (0.0f～1.0f)
}

// RGB (0.0～1.0) を HSV (0.0～1.0) に変換する関数
float3 RgbToHsv(float3 c)
{
    float4 K = float4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    
    // RGBの最小値と最大値を求める
    float M = max(c.r, max(c.g, c.b));
    float m = min(c.r, min(c.g, c.b));
    float d = M - m;
    
    float3 s_v; 
    s_v.yz = M;
    s_v.x = (M > 0.0) ? (d / M) : 0.0;
    
    float3 P = (c.g < c.b) ? float3(K.z, K.w, K.y) : float3(K.y, K.w, K.z);
    
    float3 h_temp = c - M;
    h_temp.y += M;
    h_temp.z += M;
    
    float3 h_calc = h_temp * P.x + P.y; 
    h_calc = (h_calc * d) / (d + 1e-6); 
    
    float h = step(M, c.r) * h_calc.x +
              step(M, c.g) * h_calc.y +
              step(M, c.b) * h_calc.z;
    
    h = (h < 0.0) ? (h + 1.0) : h;
    h = (d == 0.0) ? 0.0 : h;
    
    return float3(h, s_v.x, s_v.y);
}

float4 main(PS_INPUT PSInput) : SV_TARGET0
{
    float2 uv = PSInput.uv;
 
    // ベースカラー
    float4 texColor = diffuseMapTexture.Sample(diffuseMapSampler, uv);
    float3 material = texColor.rgb;
    
    // 法線計算
    float3 tanNormal = normalize(normalMapTexture.Sample(normalMapSampler, uv).xyz * 2 - 1);
    float3 normal = CalculateNormal(PSInput.tan, PSInput.bin, PSInput.normal, tanNormal);
 
    // 光の方向
    float3 lightDir = normalize(g_light_dir.xyz);
    float NdotL = max(0.0f, dot(normal, -lightDir));
    
    // 明部減衰（明るい色ほど環境光を弱める）
    float3 ambientAttenuated = CalculateAmbientAttenuation(material, g_ambient_color);
    
    // ディフューズ
    float3 diffuse = material * NdotL;
 
    // 環境光を合わせた色 
    float3 litColor = saturate(ambientAttenuated + diffuse);
    
    float3 hsv = RgbToHsv(material); // 仮の変換関数
    float currentHue = hsv.r; // Rチャンネルに色相(Hue)が格納されているとする
    
    // 現在の色相が目標の色相に近いか調べる
    float hueDiff = abs(currentHue - g_sakura_target_hue);
    hueDiff = min(hueDiff, 1.0f - hueDiff);
    
    // 検出した色相から強調マスクの生成
    float tolerance = 0.05f; // 許容する色相差の範囲
    float detectionMask = saturate(1.0f - (hueDiff / tolerance));
    
    // 暗い色などを除外
    float satValueMask = saturate(hsv.g * 2.0f) * saturate(hsv.b * 1.5f);
    
    // 最終的な強調マスク
    float sakuraMask = detectionMask * satValueMask;

    // 強調色
    float3 targetEmissiveColor = float3(1.0f, 0.7f, 0.8f); // 強調したい明るいピンク
    
    float3 sakuraBoost = targetEmissiveColor * sakuraMask * g_sakura_boost_amount;
    
    // 色の加算
    float3 boostColor = litColor + sakuraBoost;
    
    // フォグ適用
    float foglength = length(PSInput.world - g_camera_pos);
    float fog = (g_fog_end - foglength) / (g_fog_end - g_fog_start);
    fog = saturate(fog);
    float3 foggedColor = ApplyFog(boostColor, fog);
    
    // 電源がオンの場合
    // スポットライトの色計算
    float3 spotLight = CalculateSpotLite(PSInput.world, g_spot_light_pos, g_spot_light_dir, PSInput.normal);
    
    // 色の加算(電源がオフの場合0乗算で追加値なし)
    foggedColor += spotLight * g_is_light;
    
    // 色の出力
    return float4(foggedColor, texColor.a);
}