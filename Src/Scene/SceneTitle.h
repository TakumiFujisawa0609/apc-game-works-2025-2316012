#pragma once
#include <unordered_map>
#include "SceneBase.h"

class TitleStateBase;

class SceneTitle : public SceneBase
{

public:

	/// <summary>
	/// 状態
	/// </summary>
	enum class STATE
	{
		NONE,
		MAIN,
		EXPLANATION,
		SELECT,
	};

	// コンストラクタ
	SceneTitle();

	// デストラクタ
	~SceneTitle();

	// 読み込み処理
	void Load() override;

	// 初期化処理
	void Init() override;

	// 状態遷移
	void ChangeState(const STATE state);

private:

	// 状態
	STATE state_;

	// 状態管理
	std::unordered_map<STATE, std::unique_ptr<TitleStateBase>> stateMap_;

	// 更新関数
	void NormalUpdate() override;

	// 描画関数
	void NormalDraw() override;

	// 処理の変更
	void ChangeNormal() override;
};
