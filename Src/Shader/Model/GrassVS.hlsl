// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"

// IN
#include "../Common/Vertex/VertexInputType.hlsli"
#define VERTEX_INPUT DX_VERTEX3DSHADER

// OUT
#define VS_OUTPUT VertexToPixelLit
#include "../Common/Vertex/VertexShader3DHeader.hlsli"

// 定数バッファ：スロット8番目
cbuffer cbParamShadow : register(b8)
{
    float4x4 g_light_viewmatrix;
    float4x4 g_light_projectionMatrix;
};

VS_OUTPUT main(VS_INPUT VSInput)
{

    VS_OUTPUT ret;

    // 頂点座標変換 +++++++++++++++++++++++++++++++++++++( 開始 )
    float4 lLocalPosition;
    float4 lWorldPosition;
    float4 lViewPosition;

    // float3 → float4
    lLocalPosition.xyz = VSInput.pos;
    lLocalPosition.w = 1.0f;

    // ローカル座標をワールド座標に変換(剛体)
    lWorldPosition.w = 1.0f;
    lWorldPosition.xyz = mul(lLocalPosition, g_base.localWorldMatrix);

    // ワールド座標をビュー座標に変換
    lViewPosition.w = 1.0f;
    lViewPosition.xyz = mul(lWorldPosition, g_base.viewMatrix);

    // ビュー座標を射影座標に変換
    ret.svPos = mul(lViewPosition, g_base.projectionMatrix);

    // ライトのビュー座標をライトの射影座標に変換
    float4 lLViewPosition = mul(g_light_viewmatrix, lWorldPosition);
 
	// ライトのビュー座標をライトの射影座標に変換
    ret.lightAtPos = mul(g_light_projectionMatrix, lLViewPosition).xyz;
    
    // 頂点座標変換 +++++++++++++++++++++++++++++++++++++( 終了 )

    // その他、ピクセルシェーダへ引継&初期化 ++++++++++++( 開始 )
    // UV座標
    ret.uv.x = VSInput.TexCoords0.x;
    ret.uv.y = VSInput.TexCoords0.y;
    // ディフューズカラー
    ret.diffuse = VSInput.diffuse;
    // その他、ピクセルシェーダへ引継&初期化 ++++++++++++( 終了 )

    // 出力パラメータを返す
    return ret;

}
