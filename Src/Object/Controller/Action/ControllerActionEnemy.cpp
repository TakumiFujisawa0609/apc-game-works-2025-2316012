#include "../../../Core/Common/Timer.h"
#include "../../Actor/Character/Enemy.h"
#include "../ControllerPathFinder.h"
#include "../ControllerAnimation.h"
#include "ControllerActionEnemy.h"

ControllerActionEnemy::ControllerActionEnemy(Enemy& owner) :
	ControllerActionBase(owner),
	owner_(owner),
	animation_(owner.GetControllerAnimation()),
	pathFinder_(owner.GetPathFinder()),
	movePosList_(owner.GetMovePosList()),
	MOVE_SPEED(owner.GetSpeedMove()),
	DASH_SPEED(owner.GetSpeedRun())
{
	// 状態更新関数登録
	RegisterUpdateFunction(STATE::MOVE, std::bind(&ControllerActionEnemy::UpdateMove, this));
	RegisterUpdateFunction(STATE::POINT_NEW, std::bind(&ControllerActionEnemy::UpdatePointNew, this));
	RegisterUpdateFunction(STATE::IDLE, std::bind(&ControllerActionEnemy::UpdateIdle, this));
	RegisterUpdateFunction(STATE::CHASE, std::bind(&ControllerActionEnemy::UpdateChase, this));
	RegisterUpdateFunction(STATE::ACTION, std::bind(&ControllerActionEnemy::UpdateAction, this));

	// 各種変数初期化
	totalPointList_ = 0;
	state_ = STATE::NONE;
	timer_ = std::make_unique<Timer>();
}

ControllerActionEnemy::~ControllerActionEnemy()
{
}

void ControllerActionEnemy::Init()
{
	totalPointList_ = static_cast<int>(movePosList_.size());

	// 目的地のインデックスを取得
	goalIndex_ = GetRandGoalIndex();

	// 最初のポイントを設定
	pathFinder_.FindPath(owner_.GetFirstPosIndex(), goalIndex_, ADJACENT_NODE_DIST, points_);

	// 初期状態
	state_ = STATE::MOVE;
}

void ControllerActionEnemy::Update()
{
	stateUpdateFuncMap_[state_]();
}

void ControllerActionEnemy::RegisterUpdateFunction(const STATE state, std::function<void()> func)
{
	// マップに登録
	stateUpdateFuncMap_[state] = func;
}

void ControllerActionEnemy::UpdateMove()
{	
	// 目的地から移動方向を取得
	VECTOR dir = VSub(targetPos_, owner_.GetTransform().pos);

	// 移動速度の設定
	owner_.SetMoveSpeed(MOVE_SPEED);
}

void ControllerActionEnemy::UpdatePointNew()
{	
	//ポイントが無くなった場合
	if (points_.empty())
	{
		// 現在地のインデックスを設定
		int startIndex = goalIndex_;

		// 新しい目的地のインデックスを取得
		goalIndex_ = GetRandGoalIndex();

		// 新しいルートの探索
		pathFinder_.FindPath(startIndex, goalIndex_, ADJACENT_NODE_DIST, points_);
	}

	// 次のポイントを目的地に設定
	targetPos_ = movePosList_[points_.front()];

	// 先頭ポイントを削除
	points_.erase(points_.begin());

	// ランダム確率で待機状態に変更
	if (GetRand(IDLE_RAND) == 0)
	{
		// タイマーセット
		timer_->SetGoalTime(IDLE_TIME_MIN + GetRand(IDLE_TIME_RANGE));

		// アニメーションの遷移
		animation_.Play(Enemy::ANIM_IDLE);

		// 状態を待機に変更
		state_ = STATE::IDLE;
		return;
	}
	// 確率が外れた場合
	else
	{
		//状態を移動に変更
		state_ = STATE::MOVE;
		return;
	}
}

void ControllerActionEnemy::UpdateIdle()
{
	// 設定時間になった場合
	if (timer_->CountDown())
	{
		// アニメーションの遷移
		animation_.Play(Enemy::ANIM_WALK);

		//状態を移動に変更
		state_ = STATE::MOVE;
		return;
	}
}

void ControllerActionEnemy::UpdateChase()
{
}

void ControllerActionEnemy::UpdateAction()
{
}

int ControllerActionEnemy::GetRandGoalIndex()
{
	int index = GetRand(totalPointList_ - 1);
	return index;
}
