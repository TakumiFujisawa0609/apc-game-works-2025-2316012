#pragma once
#include "BillboardObjectBase.h"
#include "../Common/Vector2.h"

class SceneManager;
class ModelMaterial;
class BillboardRenderer;
class GrassRoom;

class Grass : public BillboardObjectBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Grass(const VECTOR& pos, GrassRoom& parent);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Grass() override;

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

	// 揺れの強さ
	static constexpr float WIND_STRENGTH = 0.05f;

	// 揺れの周期
	static constexpr float WIND_FREQUENCY = 10.0f;

	// 親インスタンス
	GrassRoom& parent_;

	// アルファ値
	float alpha_;

	// 開始位置
	VECTOR startPos_;
};