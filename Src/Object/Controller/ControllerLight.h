#pragma once
#include <memory>
#include <DxLib.h>
#include "ControllerBase.h"

class ActorBase;
class ColliderLine;

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

private:

	// コライダー用の所有者
	ActorBase& owner_;

	// ライト座標
	VECTOR lightPos_;

	// ラインのコライダー
	std::shared_ptr<ColliderLine> collider_;
};