#include "../../../Utility/UtilityCommon.h"
#include "../../../Utility/Utility3D.h"
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
	stateChangeMap_.emplace(STATE::MOVE, std::bind(&ControllerActionEnemy::ChangeStateMove, this));
	stateChangeMap_.emplace(STATE::IDLE, std::bind(&ControllerActionEnemy::ChangeStateIdle, this));
	stateChangeMap_.emplace(STATE::CHASE, std::bind(&ControllerActionEnemy::ChangeStateChase, this));
	stateChangeMap_.emplace(STATE::ACTION, std::bind(&ControllerActionEnemy::ChangeStateAction, this));

	// 各種変数初期化
	totalPoints_ = 0;
	state_ = STATE::NONE;
	timer_ = std::make_unique<Timer>();
}

ControllerActionEnemy::~ControllerActionEnemy()
{
}

void ControllerActionEnemy::Init()
{
	// ポイント数の取得
	totalPoints_ = static_cast<int>(movePosList_.size());

	// 初期ポイントの設定
	NewTargetPoint();
}

void ControllerActionEnemy::Update()
{
	// パラメーターの初期化
	owner_.SetMoveSpeed(0.0f);
	owner_.SetMoveDir(Utility3D::VECTOR_ZERO);
	owner_.SetGoalQuaRot(owner_.GetTransform().quaRot);

	// 状態別更新処理の実行
	updateFunc_();
}

void ControllerActionEnemy::ChangeState(const STATE state)
{
	// 状態遷移
	state_ = state;

	// 状態別変更処理
	stateChangeMap_[state_]();
}

void ControllerActionEnemy::RegisterChangeStateFunction(const STATE state, std::function<void()> func)
{
	// マップに登録
	stateChangeMap_[state] = func;
}

void ControllerActionEnemy::ChangeStateMove()
{
	updateFunc_ = std::bind(&ControllerActionEnemy::UpdateMove, this);

	// アニメーションの遷移
	animation_.Play(Enemy::ANIM_WALK);
}

void ControllerActionEnemy::ChangeStateIdle()
{
	updateFunc_ = std::bind(&ControllerActionEnemy::UpdateIdle, this);

	// ランダムで待機時間を設定
	timer_->SetGoalTime(IDLE_TIME_MIN + GetRand(IDLE_TIME_RANGE));

	// アニメーションの遷移
	animation_.Play(Enemy::ANIM_IDLE);
}

void ControllerActionEnemy::ChangeStateChase()
{
	updateFunc_ = std::bind(&ControllerActionEnemy::UpdateChase, this);
}

void ControllerActionEnemy::ChangeStateAction()
{
	updateFunc_ = std::bind(&ControllerActionEnemy::UpdateAction, this);
}

void ControllerActionEnemy::UpdateMove()
{	
	// 差分ベクトルの計算
	VECTOR diffVec = VSub(targetPos_, owner_.GetTransform().pos);

	// 目的地までの距離
	float distance = sqrt(diffVec.x * diffVec.x + diffVec.y * diffVec.y + diffVec.z * diffVec.z);

	// 距離が0より大きい場合
	if (distance > GOAL_REACH_DIST)
	{
		// 移動方向の取得
		VECTOR dir = VScale(diffVec, (1.0f / distance));

		// 目的地から移動方向を取得
		owner_.SetMoveDir(dir);

		// 移動速度の設定
		owner_.SetMoveSpeed(MOVE_SPEED);

		// 目標回転角度の設定
		owner_.SetGoalQuaRot(Quaternion::LookRotation(dir));
	}
	// 目的地に到達した場合
	else
	{
		// 次のポイントを設定
		NewTargetPoint();
	}
}

void ControllerActionEnemy::UpdateIdle()
{
	// 設定時間になった場合
	if (timer_->CountUp())
	{
		ChangeState(STATE::MOVE);
	}
}

void ControllerActionEnemy::UpdateChase()
{
}

void ControllerActionEnemy::UpdateAction()
{
}

void ControllerActionEnemy::NewTargetPoint()
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
		// 状態を待機に変更
		ChangeState(STATE::IDLE);
		return;
	}
	// 確率が外れた場合
	else
	{
		// 状態を移動に変更
		ChangeState(STATE::MOVE);
		return;
	}
}

int ControllerActionEnemy::GetRandGoalIndex()
{
	int index = GetRand(totalPoints_ - 1);
	return index;
}
