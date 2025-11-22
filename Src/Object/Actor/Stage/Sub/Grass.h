#pragma once
#include <DxLib.h>
#include <memory>
#include "../Common/Vector2.h"

class ModelMaterial;
class BillboardRenderer;
class GrassRoom;

class Grass
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Grass(const VECTOR& pos, GrassRoom& parent);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Grass();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// メインの更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:

	// 親インスタンス
	GrassRoom& parent_;

	// 活動状態
	bool isActive_;

	// アルファ値
	float alpha_;

	// 座標
	VECTOR pos_;

	// 開始位置
	VECTOR startPos_;

	// サイズ
	Vector2 size_;

	// マテリアル
	std::unique_ptr<ModelMaterial> material_;

	// レンダラー
	std::unique_ptr<BillboardRenderer> renderer_;
};