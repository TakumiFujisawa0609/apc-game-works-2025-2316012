#pragma once
#include "../../Template/Singleton.h"

class ResultManager : public Singleton<ResultManager>
{
	//シングルトンにだけ共有する
	friend class Singleton<ResultManager>;

public:

	/// <summary>
	/// 終了状態
	/// </summary>
	enum class END_STATE
	{
		CLEAR,	// クリア
		DEAD,	// 死亡
		MAX
	};

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();

	/// <summary>
	/// 終了状態の設定
	/// </summary>
	/// <param name="state">終了状態</param>
	void SetEndState(const END_STATE state);

	/// <summary>
	/// スコアの追加
	/// </summary>
	/// <param name="score">スコア</param>
	void AddScore(const int score);

private:

	// 終了状態
	END_STATE state_;

	// ゲームスコア
	int score_;

	// コンストラクタ
	ResultManager();

	// デストラクタ
	~ResultManager();
};