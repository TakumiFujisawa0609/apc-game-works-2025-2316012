#include "../common/Pixel/PixelShader2DHeader.hlsli"

// 半径
static const int RADIUS = 2;

// 定数バッファ
cbuffer ConstantBuffer : register(b4)
{
    float2 g_textureSize;   // テクスチャサイズ
};

// 擬似ランダムノイズ（紙の質感を出すため）
float GetPaperGrain(float2 uv)
{
    return frac(sin(dot(uv, float2(12.9898, 78.233))) * 43758.5453);
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // UV値を取得
    float2 srcPos = PSInput.uv;
    
    // 画像サイズ分で分割
    float2 invSize = 1.0 / g_textureSize;
    
    // 各領域の合計色
    float3 mean[4];
    
    // 二乗和を格納する配列
    float3 stdDev[4];

    // 配列の初期化処理
    [unroll]
    for (int i = 0; i < 4; i++)
    {
        mean[i] = float3(0, 0, 0);
        stdDev[i] = float3(0, 0, 0);
    }

    // 周辺ピクセルを走査して4つの領域の統計情報を取得
    [unroll]
    for (int y = -RADIUS; y <= RADIUS; y++)
    {
        [unroll]
        for (int x = -RADIUS; x <= RADIUS; x++)
        {
            float3 color = tex.Sample(texSampler, srcPos + float2(x, y) * invSize).rgb;
            float3 colorSq = color * color;

            // 境界条件を判定して各領域に加算
            if (x <= 0)
            {
                if (y <= 0)
                {
                    mean[0] += color;
                    stdDev[0] += colorSq;
                }
                if (y >= 0)
                {
                    mean[2] += color;
                    stdDev[2] += colorSq;
                }
            }
            if (x >= 0)
            {
                if (y <= 0)
                {
                    mean[1] += color;
                    stdDev[1] += colorSq;
                }
                if (y >= 0)
                {
                    mean[3] += color;
                    stdDev[3] += colorSq;
                }
            }
        }
    }

    float invN = 1.0 / 9.0;
    float minVar = 100.0;
    float3 finalColor = float3(0, 0, 0);

    // 最も色の分散が小さい領域を選択することでエッジを保持した塗りつぶしを実現
    [unroll]
    for (int j = 0; j < 4; j++)
    {
        float3 m = mean[j] * invN;
        float3 v = abs(stdDev[j] * invN - m * m);
        float var = dot(v, float3(1, 1, 1));

        if (var < minVar)
        {
            minVar = var;
            finalColor = m;
        }
    }

    // クレヨン風の質感を出すための後処理
    // 紙の粒状感を合成
    float grain = GetPaperGrain(srcPos) * 0.15;
    finalColor += grain;

    // 彩度を調整してクレヨンらしい鮮やかさを強調
    float luma = dot(finalColor, float3(0.299, 0.587, 0.114));
    finalColor = lerp(float3(luma, luma, luma), finalColor, 1.2);

    // 最終的な明るさとコントラストの補正
    finalColor = saturate(finalColor * 1.05);

    return float4(finalColor, 1.0);
}