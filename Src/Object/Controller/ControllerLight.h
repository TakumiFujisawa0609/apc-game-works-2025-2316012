#pragma once
#include <memory>
#include <DxLib.h>
#include "ControllerBase.h"

class ActorBase;
class ColliderLine;
class InputManager;
class SoundManager;

class ControllerLight : public ControllerBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ControllerLight(ActorBase& owner);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ControllerLight() override;

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// ライト座標を返す
	/// </summary>
	/// <returns>ライト座標</returns>
	const VECTOR& GetLightPos() const { return lightPos_; }

	/// <summary>
	/// ライト座標の設定
	/// </summary>
	/// <param name="pos">ライト座標</param>
	void SetLightPos(const VECTOR& pos) { lightPos_ = pos; }

	/// <summary>
	/// 電源の判定
	/// </summary>
	/// <returns>trueの場合電源がついている,falseの場合電源がついていない</returns>
	const bool IsLight() const { return isLight_; }

private:

	// 入力判定管理クラスの参照
	InputManager& input_;

	// 音管理クラスの参照
	SoundManager& sndMng_;

	// コライダー用の所有者
	ActorBase& owner_;

	// ライト座標
	VECTOR lightPos_;

	// 電源の判定
	bool isLight_;

	// ラインのコライダー
	std::shared_ptr<ColliderLine> collider_;
};