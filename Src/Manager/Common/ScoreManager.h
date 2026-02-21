#pragma once
#include "../../Template/Singleton.h"

class ScoreManager : public Singleton<ScoreManager>
{
	//シングルトンにだけ共有する
	friend class Singleton<ScoreManager>;

public:	
	
	// 通常スコア（基本給）
	static constexpr int DEFAULT_SCORE = 10000;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();

	/// <summary>
	/// ボーナスの加算
	/// </summary>
	/// <param name="score">追加ボーナス量</param>
	void AddBonusScore(const int score) { bonusScore_ += score; }

	/// <summary>
	/// 減点の加算
	/// </summary>
	/// <param name="score">追加減点量</param>
	void AddCutScore(const int score) { cutScore_ += score; }

	/// <summary>
	/// ボーナスを返す
	/// </summary>
	/// <returns>ボーナス量</returns>
	const int GetBonusScore() const { return bonusScore_; }

	/// <summary>
	/// 減点量を返す
	/// </summary>
	/// <returns>減点量</returns>
	const int GetCutScore() const { return cutScore_; } 

	/// <summary>
	/// 合計スコアを返す
	/// </summary>
	/// <returns>合計スコア</returns>
	const int GetTotalScore() const;

	/// <summary>
	/// 死亡時のスコア処理
	/// </summary>
	void DeadScore();

	// 合計スコアを計算する

private:

	// ゲームスコア
	int bonusScore_;	// 報告時の加算スコア（ボーナス）
	int cutScore_;		// 誤報告時の減点スコア

	// コンストラクタ
	ScoreManager();

	// デストラクタ
	~ScoreManager() = default;
};