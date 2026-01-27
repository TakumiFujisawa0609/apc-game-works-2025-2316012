#include "../Common/Pixel/PixelShader2DHeader.hlsli"

static const float SLICES = 16.0f; // ŠK’i‚Ì”

// ƒXƒƒbƒg4”Ô–Ú(b4)‚É“o˜^
cbuffer cbParam : register(b4)
{
    float g_step; // XV—p
    float g_strength; // ‹­‚³
    float2 dummy;
};

// ‹[Ž——”¶¬
float Random(float2 st)
{
    return frac(sin(dot(st.xy, float2(12.9898, 78.233))) * 43758.5453123);
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    float2 uv = PSInput.uv;

    // YÀ•W‚ðŠK’ió‚É‚·‚é
    float ySplit = floor(uv.y * SLICES) / SLICES;

    // ŽžŠÔ‚ÆyÀ•W‚ÉŠî‚Ã‚¢‚Ä‰¡•ûŒü‚Ö‚Ì‚¸‚ê‚ðŒvŽZ
    float drift = Random(float2(ySplit, g_step)) * g_strength;

    // ˆê’è‚ÌŠm—¦‚Å‚¸‚ç‚·
    if (Random(float2(g_step, g_step)) > 0.5)
    {
        uv.x += drift;
    }

    return tex.Sample(texSampler, uv);
}