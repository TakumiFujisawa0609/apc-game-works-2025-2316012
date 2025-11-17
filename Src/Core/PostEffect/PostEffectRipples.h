#pragma once
#include "PostEffectBase.h"
#include "../Common/Vector2F.h"

class InputManager;

class PostEffectRipples : public PostEffectBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PostEffectRipples();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PostEffectRipples() override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

private:

	// 入力管理クラス
	InputManager& input_;

	// バッファーサイズ
	static constexpr int CONST_BUFFER_SIZE = 2;

	// 時間
	float time_;

	float amplitude_;
	float frequency_;
	Vector2F centerUv_;
};