#include "../Common/Pixel/PixelShader2DHeader.hlsli"

// スロット4番目(b4)に登録
cbuffer cbParam : register(b4)
{
    float g_ripplis_step;       // リップル更新用
    float g_wave_distance;      // 波紋が発生している中心からの距離
    float g_aspect_ratio;       // 画面アスペクト比
    float g_glitch_step;        // グリッチ更新用
    
    float g_glitch_strength;    // グリッチの強さ (0.0～1.0)
    float g_time_factor;        // 波の速度
    float g_distacne_factor;    // 波の密度
    float g_total_factor;       // 歪み全体の強さ
    
    float g_wave_width;         // 波紋幅
    float2 g_start_uv;          // 波紋開始位置
    float g_block_size_y;       // ブロック分割数Y
};
    

// ランダム関数
float Random(float2 uv, float time)
{
    float t = time * 0.05f;
    float value = dot(uv + t, float2(12.9898, 78.233));
    return frac(sin(value) * 43758.5453123);
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // 準備
    float2 uv = PSInput.uv;
    float glitchIntensity = g_glitch_strength;
    
    // 波紋エフェクトの計算
    float2 uvToVec = g_start_uv.xy - uv;
    uvToVec.x = uvToVec.x * g_aspect_ratio;
    float dis = length(uvToVec);
    float sinFactor = sin(dis * g_distacne_factor + g_ripplis_step * g_time_factor) * g_total_factor * 0.01f;
    float discardFactor = clamp(g_wave_width - abs(g_wave_distance - dis), 0.0f, 1.0f) / g_wave_width;
    float2 vec = (dis < 0.0001f) ? float2(0.0f, 0.0f) : normalize(uvToVec);
    float2 rippleOffset = vec * sinFactor * discardFactor;
    
    // 波紋適用後のUV
    float2 warpedUv = uv + rippleOffset;

    // グリッチの計算
    float blockId = floor(warpedUv.y * g_block_size_y);
    float lineNoise = Random(float2(blockId, g_glitch_step * 0.1f), 0.0f);
    
    float glitchOffset = 0.0f;
    if (lineNoise < glitchIntensity * 0.3f)
    {
        glitchOffset = (Random(float2(blockId, g_glitch_step), 1.0f) - 0.5f) * glitchIntensity * 0.2f;
    }
    
    float splitAmount = glitchIntensity * 0.02f * Random(float2(g_glitch_step, 0.0f), 0.0f);
    float2 finalUv = warpedUv + float2(glitchOffset, 0.0f);

    // UV座標が 0.0～1.0 の範囲を超えないように固定する
    float2 uvR = clamp(finalUv + float2(splitAmount, 0.0f), 0.0f, 1.0f);
    float2 uvG = clamp(finalUv, 0.0f, 1.0f);
    float2 uvB = clamp(finalUv - float2(splitAmount, 0.0f), 0.0f, 1.0f);

    // 範囲制限したUVでサンプリング
    float4 colR = tex.Sample(texSampler, uvR);
    float4 colG = tex.Sample(texSampler, uvG);
    float4 colB = tex.Sample(texSampler, uvB);

    float4 finalColor = float4(colR.r, colG.g, colB.b, colG.a);

    return finalColor;
}