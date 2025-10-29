#include "../../../Manager/Generic/CharacterManager.h"
#include "../../../Manager/Generic/CollisionManager.h"
#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/Generic/GameStateManager.h"
#include "../../../Manager/Generic/StageManager.h"
#include "../../../Manager/Generic/Camera.h"
#include "../../../Utility/UtilityCommon.h"
#include "../../../Utility/Utility3D.h"
#include "../../../Core/Common/Timer.h"
#include "../../../Core/Common/ScreenShake.h"
#include "../../Actor/Character/Player.h"
#include "../../Actor/Character/Enemy.h"
#include "../../Collider/ColliderLine.h"
#include "../../Collider/ColliderSphere.h"
#include "../ControllerPathFinder.h"
#include "../ControllerAnimation.h"
#include "../ControllerCameraTransition.h"
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
	shake_ = nullptr;
	totalPoints_ = 0;
	goalIndex_ = 0;
	isViewRange_ = false;
	nextPointPos_ = Utility3D::VECTOR_ZERO;
	state_ = STATE::NONE;

	// 処理で使うインスタンスの生成
	timer_ = std::make_unique<Timer>();
	transition_ = std::make_unique<ControllerCameraTransition>();
	shake_ = std::make_unique<ScreenShake>();
}

ControllerActionEnemy::~ControllerActionEnemy()
{
}

void ControllerActionEnemy::Init()
{
	// トランジションの初期化
	transition_->Init();

	// 画面シェイク初期化
	shake_->Init();

	// ポイント数の取得
	totalPoints_ = static_cast<int>(movePosList_.size());

	// 経路探索初期化
	pathFinder_.Init();

	// ゴール位置をランダムで生成
	goalIndex_ = pathFinder_.GetNearNodeIndex(owner_.GetTransform().pos);

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


	for (auto& pos : movePosList_)
	{
		DrawSphere3D(pos, 10.0f, 16, UtilityCommon::ORANGE, UtilityCommon::ORANGE, false);
	}

	if (points_.empty())
	{
		return;
	}

	constexpr int OFFSET = 50;

	//目的地の描画
	for (auto point : points_)
	{
		VECTOR pos = movePosList_[point];
		pos.y += OFFSET;
		DrawSphere3D(pos, 10.0f, 16, UtilityCommon::CYAN, UtilityCommon::CYAN, false);
	}

	for (int i = 0; i < points_.size() - 1; i++)
	{
		VECTOR start = movePosList_[points_[i]];
		VECTOR end = movePosList_[points_[i + 1]];
		start.y += OFFSET;
		end.y += OFFSET;
		DrawLine3D(start, end, UtilityCommon::CYAN);
	}


	VECTOR my = owner_.GetTransform().pos;
	VECTOR target = targetTransform_.pos;
	color = UtilityCommon::YELLOW;
	my.y += OFFSET;
	target.y += OFFSET;
	// プレイヤーと敵との距離の衝突判定
	for (auto model : pathFinder_.GetModels())
	{
		auto it = MV1CollCheck_Line(model, -1, my, target);
		if (it.HitFlag)
		{
			color = UtilityCommon::RED;
			break;
		}
	}

	// プレイヤーと敵との距離
	DrawLine3D(my, target, color);

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

	// 現在地が最も近い位置番号を習得
	int startIndex = pathFinder_.GetNearNodeIndex(owner_.GetTransform().pos);

	// ランダムでゴールを設定
	goalIndex_ = GetRandGoalIndex();

	// ポイントの初期化
	points_.clear();

	// 新しい経路を探索する
	pathFinder_.FindPath(startIndex, goalIndex_, ADJACENT_NODE_DIST, points_);

	// 視野外に設定
	isViewRange_ = false;
}

void ControllerActionEnemy::ChangeStateIdle()
{
	updateFunc_ = std::bind(&ControllerActionEnemy::UpdateIdle, this);

	// ランダムで待機時間を設定
	timer_->SetGoalTime(static_cast<float>(IDLE_TIME_MIN + GetRand(IDLE_TIME_RANGE)));

	// アニメーションの遷移
	animation_.Play(Enemy::ANIM_IDLE);
}

void ControllerActionEnemy::ChangeStateChase()
{
	updateFunc_ = std::bind(&ControllerActionEnemy::UpdateChase, this);

	// 時間の設定
	timer_->SetGoalTime(CHANGE_POINT_TIME);

	// ターゲットまでのパスを生成
	FindPathToTarget();
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

	// アニメーションを実行
	animation_.Play(Enemy::ANIM_ACTION);

	// カメラを固定する
	mainCamera.ChangeMode(Camera::MODE::FIXED_POINT);	
	
	// 所有者の座標取得
	VECTOR pos = owner_.GetTransform().pos;

	// 敵からカメラへ向かうベクトル
	VECTOR TargetDir = VSub(targetTransform_.pos, pos);

	// 目標の回転クォータニオンを生成
	Quaternion goalQua = Quaternion::LookRotation(TargetDir, VGet(0.0f, 1.0f, 0.0f));

	// マトリックスの回転情報を習得
	MATRIX enemyRot = Quaternion::ToMatrix(goalQua);
	
	// ベクトル生成
	VECTOR cameraVec = VTransform(JUMP_SCARE_CAMERA_POS, enemyRot);
	VECTOR cameraTVec = VTransform(JUMP_SCARE_CAMERA_TARGET_POS, enemyRot);

	// トランジション設定
	transition_->Set(VAdd(cameraVec, pos), VAdd(cameraTVec, pos), TRANSITION_TIME);

	// 画面シェイク設定
	shake_->Set(SHAKE_TIME, SHAKE_POWER);
}

void ControllerActionEnemy::UpdateSearch()
{	
	// ターゲットを見つけた場合
	if (IsSearchTarget())
	{
		// 処理を飛ばす
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

		// ランダム確率で待機状態に変更
		if (GetRand(IDLE_RAND) == 0)
		{
			// 状態を待機に変更
			ChangeState(STATE::IDLE);
			return;
		}
	}
}

void ControllerActionEnemy::UpdateIdle()
{
	// ターゲットを見つけた場合
	if (IsSearchTarget())
	{
		// 処理を飛ばす
		return;
	}

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
		owner_.SetMoveSpeed(DASH_SPEED);

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
	float distance = static_cast<float>(Utility3D::Distance(nextPointPos_, myPos));

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
	// カメラのトランジション処理
	transition_->Update();

	// トランジションが終了している場合
	if (transition_->IsEnd())
	{
		// 画面シェイク処理
		shake_->Update();

		// 画面揺れが終わった場合
		if (shake_->IsEnd())
		{
			// リザルトに変わる
			scnMng_.ChangeScene(SceneManager::SCENE_ID::RESULT);
			GameStateManager::GetInstance().ChangeState(GameStateManager::STATE::NONE);
			return;
		}
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

	// 所有者の向きをターゲットに向かせる
	owner_.SetGoalQuaRot(Quaternion::LookRotation(dir));
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

		int size = points_.size();
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
	int startIndex = pathFinder_.GetNearNodeIndex(owner_.GetTransform().pos);

	// ポイントの初期化
	points_.clear();

	// 経路探索
	pathFinder_.FindPath(startIndex, goalIndex_, ADJACENT_NODE_DIST, points_);
	
	// 次のポイントを目的地に設定
	nextPointPos_ = movePosList_[points_.front()];

	// 先頭ポイントを削除
	points_.erase(points_.begin());
}

bool ControllerActionEnemy::IsSearchTarget()
{
	if (isViewRange_)
	{
		// 状態遷移
		ChangeState(STATE::CHASE_NEAR);
		return true;
	}

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
			// 位置調整
			myPos.y += 50;
			targetPos.y += 50;
			CreateLineCollider(myPos, targetPos);

			// 現時点では範囲内と設定
			isViewRange_ = true;
		}
	}

	return false;
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