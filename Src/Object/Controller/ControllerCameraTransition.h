#pragma once
#include <DxLib.h>
#include "ControllerBase.h"

class Camera;

class ControllerCameraTransition : public ControllerBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ControllerCameraTransition();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ControllerCameraTransition() override;
	
	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();
	
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 設定
	/// </summary>
	/// <param name="goalCameraPos">カメラの目標位置</param>
	/// <param name="goalTargetPos">注視点の目標位置</param>
	/// <param name="transitionTime">トランジション時間</param>
	void Set(const VECTOR& goalCameraPos, const VECTOR& goalTargetPos, const float transitionTime);

	/// <summary>
	/// 処理の終了判定
	/// </summary>
	/// <returns>trueの場合終了済み,falseの場合実行中</returns>
	const bool IsEnd() const { return isEnd_; }

private:

	// カメラの参照
	Camera& camera_;

	// 計算用ステップ
	float step_;

	// トランジション時間
	float transitionTime_;

	// 開始位置
	VECTOR startCameraPos_;		// 位置
	VECTOR startTargetPos_;		// 注視点

	// 終了位置
	VECTOR goalCameraPos_;
	VECTOR goalTargetPos_;

	// 終了判定
	bool isEnd_;
};

