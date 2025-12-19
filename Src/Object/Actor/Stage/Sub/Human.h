#pragma once
#include "BillboardObjectBase.h"

class SceneManager;
class ModelMaterial;
class BillboardRenderer;

class Human : public BillboardObjectBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">座標</param>
	Human(const VECTOR& pos);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Human() override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;

	/// <summary>
	/// メインの更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

private:

	// グリッチの強さ
	static constexpr float GLITCH_STRENGTH = 0.5f;

	// 最小サイズ
	static constexpr int SIZE_MIN_X = 100;
	static constexpr int SIZE_MIN_Y = 200;

	// 最大サイズ
	static constexpr int SIZE_MAX_X = 300;
	static constexpr int SIZE_MAX_Y = 200;

	// 大きさをランダムで返す
	const Vector2& GetRandSize() const;
};

