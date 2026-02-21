#include "../Game/GameSystemManager.h"
#include "ScoreManager.h"

void ScoreManager::Init()
{
	bonusScore_ = 0;
	cutScore_ = 0;
}

const int ScoreManager::GetTotalScore() const
{
	int score = DEFAULT_SCORE + bonusScore_ - cutScore_;
	if (score < 0)
	{
		return 0;
	}
	return score;
}

void ScoreManager::DeadScore()
{
	// ボーナスはゼロにする
	bonusScore_ = 0;

	// 減点は基本給量分減らす
	cutScore_ = DEFAULT_SCORE;
}

ScoreManager::ScoreManager()
{
	bonusScore_ = 0;
	cutScore_ = 0;
}
