#pragma once
#include "Vector2.h"
class Sprite
{

public:

	// テクスチャハンドルID
	int handleId;	

	// テクスチャの位置
	Vector2 pos;	

	// テクスチャサイズ
	Vector2 size;	

	// 分割数
	Vector2 div;	

	// 中心座標
	Vector2 center;

	// 回転角度
	float angle;	

	// 拡大率
	float scale;	

	// 透過
	bool isTrans;

	// X軸反転
	bool isFlipX;	

	// Y軸反転
	bool isFlipY;	

	// デフォルトコンストラクタ
	Sprite();

	//デストラクタ
	~Sprite();

	/// <summary>
	/// 中心座標を返す(DrawGraphで描画しているもの用)
	/// </summary>
	/// <returns></returns>
	const Vector2 GetCenterPos() const;

	/// <summary>
	/// 通常描画
	/// </summary>
	/// <returns></returns>
	const void Draw() const;

	/// <summary>
	/// 回転描画
	/// </summary>
	/// <returns></returns>
	const void DrawRota() const;
};

