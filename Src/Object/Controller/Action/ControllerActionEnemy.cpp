#include "../../../Manager/Generic/CharacterManager.h"
#include "../../../Utility/UtilityCommon.h"
#include "../../../Utility/Utility3D.h"
#include "../../../Core/Common/Timer.h"
#include "../../Actor/Character/Player.h"
#include "../../Actor/Character/Enemy.h"
#include "../ControllerPathFinder.h"
#include "../ControllerAnimation.h"
#include "ControllerActionEnemy.h"

ControllerActionEnemy::ControllerActionEnemy(Enemy& owner) :
	ControllerActionBase(owner),
	owner_(owner),
	targetTransform_(CharacterManager::GetInstance().GetCharacter(CharacterManager::TYPE::PLAYER).GetTransform()),
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
	goalIndex_ = 0;
	targetPos_ = Utility3D::VECTOR_ZERO;
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

void ControllerActionEnemy::DebugDraw()
{
	VECTOR centerPos = owner_.GetTransform().pos;
	VECTOR rot = owner_.GetTransform().rot;

	//ラジアンに変換
	float viewRad = UtilityCommon::Deg2RadF(VIEW_ANGLE);

	//前方方向を求める
	float forwardX = sinf(rot.y);
	float forwardZ = cosf(rot.y);

	//前方方向の座標
	VECTOR forwardPos = centerPos;
	forwardPos.x += forwardX * VIEW_RANGE;
	forwardPos.z += forwardZ * VIEW_RANGE;

	//右斜め30度の方向を求める
	float rightX = sinf(rot.y + viewRad);
	float rightZ = cosf(rot.y + viewRad);

	//右斜め30度の座標
	VECTOR rightPos = centerPos;
	rightPos.x += rightX * VIEW_RANGE;
	rightPos.z += rightZ * VIEW_RANGE;

	//左斜め30度の方向を求める
	float leftX = sinf(rot.y - viewRad);
	float leftZ = cosf(rot.y - viewRad);

	//右斜め30度の座標
	VECTOR leftPos = centerPos;
	leftPos.x += leftX * VIEW_RANGE;
	leftPos.z += leftZ * VIEW_RANGE;

	VECTOR value = { VIEW_ANGLE,VIEW_ANGLE,VIEW_ANGLE };
	centerPos = VAdd(centerPos, value);

	// 状態別色分け
	int color = state_ == STATE::CHASE ? UtilityCommon::RED : UtilityCommon::YELLOW;

	// 視野の描画
	DrawTriangle3D(centerPos, forwardPos, rightPos, color, TRUE);
	DrawTriangle3D(leftPos, forwardPos, centerPos, color, TRUE);
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

	// アニメーションの遷移
	animation_.Play(Enemy::ANIM_RUN);
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

	// ターゲットの探索
	SearchTarget();
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
	// 探索処理
	SearchTarget();

	// ターゲット位置を取得
	targetPos_ = targetTransform_.pos;

	// 自身の位置を取得
	VECTOR myPos = owner_.GetTransform().pos;

	// 差分ベクトルの計算
	VECTOR diffVec = VSub(targetPos_, myPos);

	// 距離を計算
	float distance = Utility3D::Distance(targetPos_, myPos);

	// 移動方向の取得
	VECTOR dir = VScale(diffVec, (1.0f / distance));

	if (!Utility3D::EqualsVZero(dir))
	{
		// 目的地から移動方向を取得
		owner_.SetMoveDir(dir);

		// 移動速度の設定
		owner_.SetMoveSpeed(DASH_SPEED);

		// 目標回転角度の設定
		owner_.SetGoalQuaRot(Quaternion::LookRotation(dir));
	}
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

void ControllerActionEnemy::SearchTarget()
{
	// ターゲット位置を取得
	VECTOR targetPos = targetTransform_.pos;

	// 自身の位置を取得
	VECTOR myPos = owner_.GetTransform().pos;

	// 自身からターゲットまでのベクトル
	VECTOR toTarget = VSub(targetPos, myPos);

	// ターゲットまでの距離を計算
	float distance = Utility3D::Distance(targetPos, myPos);

	// 視野内に入っている場合
	if (distance <= VIEW_RANGE * VIEW_RANGE)
	{
		//自分から見たプレイヤーの角度を求める
		float rad = atan2(targetPos.x - myPos.x, targetPos.z - myPos.z);
		float viewRad = rad - owner_.GetTransform().rot.y;
		float viewDeg = static_cast<float>(UtilityCommon::DegIn360(UtilityCommon::Rad2DegF(viewRad)));

		//視野内に入ってるか判断
		if (viewDeg <= VIEW_ANGLE || viewDeg >= (360.0f - VIEW_ANGLE))
		{
			ChangeState(STATE::CHASE);
		}
		// 視野外の場合
		else
		{
			ChangeState(STATE::MOVE);
		}
	}
}

int ControllerActionEnemy::GetRandGoalIndex()
{
	int index = GetRand(totalPoints_ - 1);
	return index;
}
