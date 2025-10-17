#include "../../../Manager/Generic/CharacterManager.h"
#include "../../../Manager/Generic/CollisionManager.h"
#include "../../../Utility/UtilityCommon.h"
#include "../../../Utility/Utility3D.h"
#include "../../../Core/Common/Timer.h"
#include "../../Actor/Character/Player.h"
#include "../../Actor/Character/Enemy.h"
#include "../../Collider/ColliderLine.h"
#include "../../Collider/ColliderSphere.h"
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
	DASH_SPEED(owner.GetSpeedRun()),
	VIEW_RANGE(owner.GetViewRange()),
	VIEW_ANGLE(owner.GetViewAngle())
{
	// 状態更新関数登録
	stateChangeMap_.emplace(STATE::SEARCH, std::bind(&ControllerActionEnemy::ChangeStateSearch, this));
	stateChangeMap_.emplace(STATE::IDLE, std::bind(&ControllerActionEnemy::ChangeStateIdle, this));
	stateChangeMap_.emplace(STATE::CHASE, std::bind(&ControllerActionEnemy::ChangeStateChase, this));
	stateChangeMap_.emplace(STATE::CHASE_NEAR, std::bind(&ControllerActionEnemy::ChangeStateChaseNear, this));
	stateChangeMap_.emplace(STATE::ACTION, std::bind(&ControllerActionEnemy::ChangeStateAction, this));

	// 各種変数初期化
	colliderSphere_ = nullptr;
	totalPoints_ = 0;
	goalIndex_ = 0;
	nextPointPos_ = Utility3D::VECTOR_ZERO;
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

	// 初期状態
	ChangeState(STATE::SEARCH);
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

	// 範囲の描画
	DrawSphere3D(owner_.GetTransform().pos, NEAR_RANGE, 16, UtilityCommon::YELLOW, UtilityCommon::YELLOW, false);
	DrawSphere3D(owner_.GetTransform().pos, CHASE_RANGE, 16, UtilityCommon::YELLOW, UtilityCommon::YELLOW, false);

	DrawFormatString(0, 240, UtilityCommon::WHITE, L"自身の状態 : %d", static_cast<int>(state_));
}

void ControllerActionEnemy::ChangeState(const STATE state)
{
	// 同じ状態の場合無視
	if (state_ == state) { return; }

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

void ControllerActionEnemy::ChangeStateSearch()
{
	updateFunc_ = std::bind(&ControllerActionEnemy::UpdateSearch, this);

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

void ControllerActionEnemy::ChangeStateChaseNear()
{
	updateFunc_ = std::bind(&ControllerActionEnemy::UpdateChaseNear, this);

	// アニメーションの遷移
	animation_.Play(Enemy::ANIM_RUN);
}

void ControllerActionEnemy::ChangeStateAction()
{
	updateFunc_ = std::bind(&ControllerActionEnemy::UpdateAction, this);
}

void ControllerActionEnemy::UpdateSearch()
{	
	// ターゲットの探索
	SearchTarget();

	// 差分ベクトルの計算
	VECTOR diffVec = VSub(nextPointPos_, owner_.GetTransform().pos);

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
			ChangeState(STATE::SEARCH);
			return;
		}
	}
}

void ControllerActionEnemy::UpdateIdle()
{
	// 設定時間になった場合
	if (timer_->CountUp())
	{
		ChangeState(STATE::SEARCH);
	}
}

void ControllerActionEnemy::UpdateChase()
{
	// ターゲットが範囲外の場合
	if (CheckRangeToTarget(CHASE_RANGE))
	{
		// 範囲外のため探索に戻る
		ChangeState(STATE::SEARCH);

		// 現在地が最も近い位置番号を習得
		int startIndex = pathFinder_.GetNearNodeIndex(owner_.GetTransform().pos);

		// ランダムでゴールを設定
		goalIndex_ = GetRandGoalIndex();

		// 新しい経路を探索する
		pathFinder_.FindPath(startIndex, goalIndex_, ADJACENT_NODE_DIST, points_);

		// 処理終了
		return;
	}
	// ターゲットが至近距離内の場合
	else if (!CheckRangeToTarget(NEAR_RANGE))
	{
		// 範囲外のため探索に戻る
		ChangeState(STATE::CHASE_NEAR);

		// 処理終了
		return;
	}

	// 差分ベクトルの計算
	VECTOR diffVec = VSub(nextPointPos_, owner_.GetTransform().pos);

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

	// 移動ポイントの更新
	if (points_.empty() || timer_->CountUp())
	{
		// ターゲットまでの経路を探索
		FindPathToTarget();
	}
}

void ControllerActionEnemy::UpdateChaseNear()
{
	// ターゲットが至近距離外の場合
	if (CheckRangeToTarget(NEAR_RANGE))
	{
		// 通常の追跡に戻る
		ChangeState(STATE::CHASE);

		// 時間の設定
		timer_->SetGoalTime(CHANGE_POINT_TIME);

		// 現在地が最も近い位置番号を習得
		int startIndex = pathFinder_.GetNearNodeIndex(owner_.GetTransform().pos);

		// ターゲット位置が最も近い位置番号を習得
		goalIndex_ = pathFinder_.GetNearNodeIndex(targetTransform_.pos);

		// 新しい経路を探索する
		pathFinder_.FindPath(startIndex, goalIndex_, ADJACENT_NODE_DIST, points_);

		// 処理終了
		return;
	}

	// ターゲット位置を取得
	nextPointPos_ = targetTransform_.pos;

	// 自身の位置を取得
	VECTOR myPos = owner_.GetTransform().pos;

	// 差分ベクトルの計算
	VECTOR diffVec = VSub(nextPointPos_, myPos);

	// 距離を計算
	float distance = Utility3D::Distance(nextPointPos_, myPos);

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
	nextPointPos_ = movePosList_[points_.front()];

	// 先頭ポイントを削除
	points_.erase(points_.begin());
}

void ControllerActionEnemy::FindPathToTarget()
{
	// ターゲットが最も近い場所番号を取得
	goalIndex_ = pathFinder_.GetNearNodeIndex(targetTransform_.pos);

	// 現在地の更新
	int startIndex = points_.front();

	// 経路探索
	pathFinder_.FindPath(startIndex, goalIndex_, ADJACENT_NODE_DIST, points_);
	
	// 次のポイントを目的地に設定
	nextPointPos_ = movePosList_[points_.front()];

	// 先頭ポイントを削除
	points_.erase(points_.begin());
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
	float distanceSq = VDot(toTarget, toTarget);

	// 視野内に入っている場合
	if (distanceSq <= VIEW_RANGE * VIEW_RANGE)
	{
		//自分から見たプレイヤーの角度を求める
		float rad = atan2(targetPos.x - myPos.x, targetPos.z - myPos.z);
		float viewRad = rad - owner_.GetTransform().rot.y;
		float viewDeg = static_cast<float>(UtilityCommon::DegIn360(UtilityCommon::Rad2DegF(viewRad)));

		//視野内に入ってるか判断
		if (viewDeg <= VIEW_ANGLE || viewDeg >= (360.0f - VIEW_ANGLE))
		{
			// 障害物との判定用のコライダーを生成
			CreateLineCollider(myPos, targetPos);

			// 追跡状態に変更
			ChangeState(STATE::CHASE_NEAR);
			return;
		}
	}
}

bool ControllerActionEnemy::CheckRangeToTarget(const float range)
{
	// 距離を求める
	float distance = Utility3D::Distance(owner_.GetTransform().pos, targetTransform_.pos);

	// 距離が大きい場合
	if (distance > range)
	{
		// 成功
		return true;
	}

	// 失敗
	return false;
}

//void ControllerActionEnemy::CreateSphereCollider()
//{
//	// 球体の生成
//	colliderSphere_ = std::make_shared<ColliderSphere>(owner_, CollisionTags::TAG::ENEMY_SPHERE_RANGE);
//
//	// 球体の半径の設定
//	colliderSphere_->SetRadius(CHASE_RANGE);
//
//	// 追加
//	collMng_.Add(colliderSphere_);
//}

void ControllerActionEnemy::CreateLineCollider(const VECTOR& start, const VECTOR& end)
{
	// コライダーの生成
	std::shared_ptr<ColliderLine> coll = std::make_shared<ColliderLine>(owner_, CollisionTags::TAG::ENEMY_VIEW);

	// 先端位置設定
	coll->SetLocalPosPointHead(start);

	// 末端位置の設定
	coll->SetLocalPosPointEnd(end);

	// 判定後削除
	coll->SetDelete();

	// 追加
	collMng_.Add(std::move(coll));
}

int ControllerActionEnemy::GetRandGoalIndex()
{
	int index = GetRand(totalPoints_ - 1);
	return index;
}