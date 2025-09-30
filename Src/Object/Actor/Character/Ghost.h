#pragma once
#include "CharacterBase.h"

class Ghost : public CharacterBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="param">パラメーター</param>
	Ghost(const Json& param);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Ghost();

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load() override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init() override;

private:

	// メインの描画処理
	void DrawMain() override;

	// アニメーションの初期化
	void InitAnimation() override;

	// トランスフォームの初期化
	void InitTransform() override;
};