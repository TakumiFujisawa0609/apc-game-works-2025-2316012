#include "../Common/Pixel/PixelShader2DHeader.hlsli"

//-----------------------------------------------------------------------------
// 定数バッファ (C++側から渡すパラメータ)
//-----------------------------------------------------------------------------
// スロット4番目(b4)に登録
cbuffer cbParam : register(b4)
{
    float Time; // 経過時間 (アニメーション用)
    float Amplitude; // 波の振幅 (強さ)
    float Frequency; // 波の波数 (密度)
    float2 Centeruv; // 波紋の中心座標 (0.0～1.0)
    float3 LightDirection; // 光源の方向ベクトル (正規化済み)
};

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // -----------------------------------------------------
    // 1. 波紋の高さ (Height) と変位量 (Displacement) の計算
    // -----------------------------------------------------
    float2 diff = PSInput.uv - Centeruv;
    float dist = length(diff);
    
    // 定数として定義
    const float Speed = 5.0f;
    // 位相 = (波数 * 距離) - (速度 * 時間)
    float phase = Frequency * dist - Speed * Time;
    
    // 波の高さ（感の元になる）
    float height = Amplitude * sin(phase);
    
    // テクスチャ座標の歪み量
    float displacement = Amplitude * sin(phase);

    // -----------------------------------------------------
    // 2. 新しいuv座標 (Newuv) の計算 (テクスチャ歪み)
    // -----------------------------------------------------
    float2 direction = normalize(diff);
    // 中心点でのゼロ除算を防ぐ
    if (dist < 0.0001f)
    {
        direction = float2(0.0f, 0.0f);
    }
    float2 newuv = PSInput.uv + direction * displacement;

    // -----------------------------------------------------
    // 3. 法線ベクトル (Normal Vector) の動的計算 (簡易的な有限差分)
    // -----------------------------------------------------
    const float epsilon = 0.005f; // 微小なuvオフセット

    // 周囲の点の高さを仮想的に計算
    float h_left = Amplitude * sin(Frequency * length((PSInput.uv + float2(-epsilon, 0.0f)) - Centeruv) - Speed * Time);
    float h_right = Amplitude * sin(Frequency * length((PSInput.uv + float2(epsilon, 0.0f)) - Centeruv) - Speed * Time);
    float h_up = Amplitude * sin(Frequency * length((PSInput.uv + float2(0.0f, epsilon)) - Centeruv) - Speed * Time);
    float h_down = Amplitude * sin(Frequency * length((PSInput.uv + float2(0.0f, -epsilon)) - Centeruv) - Speed * Time);

    // X, Y方向の傾きを求める
    float dx = (h_right - h_left) / (2.0f * epsilon);
    float dy = (h_up - h_down) / (2.0f * epsilon);

    // 法線ベクトル (N = (-dx, -dy, 1.0) を正規化)
    float3 normal = normalize(float3(-dx, -dy, 1.0f));

    // -----------------------------------------------------
    // 4. ライティングの計算（陰影のみ）
    // -----------------------------------------------------
    // 画面全体を明るく保つための調整
    const float AmbientStrength = 0.95f; // 全体の明るさの底上げ（元の色の95%）
    const float ShadowStrength = 0.3f; // 影の濃さ（この値が大きいほど影が濃くなる）
    const float3 LightColor = { 1.0f, 1.0f, 1.0f }; // 光源色

    // 環境光 (Ambient)
    float3 ambient = AmbientStrength * LightColor;

    // 法線と光源の内積
    // 1.0 (正面) ～ -1.0 (裏面)
    float dotFactor = dot(normal, -normalize(LightDirection));

    // 陰影（Shadow）の計算
    // dotFactorが 1.0 のときは 0.0 になる (影なし)
    // dotFactorが 0.0 のときは 1.0 * ShadowStrength になる (影最大)
    float ShadowEffect = (1.0f - dotFactor) * ShadowStrength;

    // 最終的な光の強さ: Ambient (基本明るさ) から ShadowEffect を減算
    float3 totalLight = ambient - ShadowEffect;
    
    // 0.0f～1.0f にクランプ（これ以上暗くも明るくもしない）
    totalLight = clamp(totalLight, 0.0f, 1.0f);

    // -----------------------------------------------------
    // 5. 最終的なピクセル色の決定
    // -----------------------------------------------------
    // 新しいuv座標でテクスチャをサンプリング
    float4 baseColor = tex.Sample(texSampler, newuv);

    // ベース色に乗算
    float4 finalColor = float4(baseColor.rgb * totalLight, baseColor.a);

    return finalColor;
}