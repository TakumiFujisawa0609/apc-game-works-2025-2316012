#include "../../Manager/Generic/SceneManager.h"
#include "Timer.h"

Timer::Timer(const float goalTime) :
	goalTime_(goalTime),
	scnMng_(SceneManager::GetInstance())
{
	step_ = 0.0f;
}

Timer::~Timer()
{
}

void Timer::Init()
{
	//ステップ初期化
	step_ = goalTime_;
}

bool Timer::CountDown()
{
	// ステップの更新
	step_ -= scnMng_.GetDeltaTime();

	//目標時間に達しているか
	if (0 > step_)
	{
		return true;
	}

	return false;
}