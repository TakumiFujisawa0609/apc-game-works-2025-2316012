#pragma once
#include <memory>
#include <DxLib.h>
#include "../../../../Common/Vector2.h"

class SceneManager;
class GrassRoom;
class ModelMaterial;
class BillboardRenderer;

class Grass
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">生成位置</param>
	/// <param name="parent">部屋のインスタンス</param>
	Grass(const VECTOR& pos, const GrassRoom& grassRoom);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Grass();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

private:

	// 揺れの強さ
	static constexpr float AMPLITUDE = 0.01f;

	// 草のサイズ
	const Vector2 SIZE = { 30, 40 };	
	
	// 発生する部屋の参照
	const GrassRoom& grassRoom_;

	// シーン管理クラス
	SceneManager& scnMng_;

	// 活動判定
	bool isActive_;

	// ステップ
	float step_;

	// 位置
	VECTOR pos_;

	// 大きさ
	Vector2 size_;

	// マテリアル
	std::unique_ptr<ModelMaterial> material_;

	// レンダラー
	std::unique_ptr<BillboardRenderer> renderer_;
};

