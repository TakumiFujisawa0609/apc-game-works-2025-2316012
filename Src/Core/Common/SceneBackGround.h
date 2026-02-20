#pragma once
#include <memory>
#include "../CoreBase.h"
#include "../../Common/Vector2.h"

class PixelMaterial;
class PixelRenderer;

class SceneBackGround : public CoreBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	SceneBackGround();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SceneBackGround() override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

private:
	
	// 歪みの強さ
	static constexpr float DISTORTION_STRENGTH = 0.05;	
	
	// 動く速度
	static constexpr float MOVE_SPEED = 0.2;	

	// バッファーサイズ
	static constexpr int BUFFER_SIZE = 1.0f;

	// 座標
	const Vector2 POS = { -100, -100};

	// サイズ
	const Vector2 SIZE = { 1480, 920 };

	// 時間
	float step_;

	// マテリアル
	std::unique_ptr<PixelMaterial> material_;

	// レンダラー
	std::unique_ptr<PixelRenderer> renderer_;
};

