#include "../../../Manager/Common/Camera.h"
#include "../../../Manager/Common/SceneManager.h"
#include "../../../Manager/Game/CharacterManager.h"
#include "../../../Manager/Game/StageManager.h"
#include "../../../Manager/Game/CollisionManager.h"
#include "../../../Manager/Game/GameSystemManager.h"
#include "../../../Manager/Game/ShadowManager.h"
#include "../../../Manager/Game/GameEffectManager.h"
#include "../../../Manager/Game/GameStateManager.h"
#include "../../../Object/Actor/Stage/StageObjectBase.h"
#include "../../../Object/Actor/Character/CharacterBase.h"
#include "../../../Object/Actor/Character/Enemy.h"
#include "../../../Object/Controller/ControllerAnimation.h"
#include "../../../Object/Controller/Camera/ControllerCameraBase.h"
#include "../../../Core/Common/Timer.h"
#include "../../../Core/Common/ScreenShake.h"
#include "../../../Utility/UtilityCommon.h"
#include "../../../Utility/Utility3D.h"
#include "GameStateEnemyRespown.h"

GameStateEnemyRespown::GameStateEnemyRespown()
{
	state_ = STATE::WAIT;
	target_ = nullptr;

	// 状態変更関数管理
	changeStateMap_.emplace(STATE::WAIT, std::bind(&GameStateEnemyRespown::ChangeStateWait, this));
	changeStateMap_.emplace(STATE::START, std::bind(&GameStateEnemyRespown::ChangeStateStart, this));
	changeStateMap_.emplace(STATE::WALK, std::bind(&GameStateEnemyRespown::ChangeStateWalk, this));
	changeStateMap_.emplace(STATE::MOVE_CAMERA_FORWARD, std::bind(&GameStateEnemyRespown::ChangeStateMoveCameraForward, this));
	changeStateMap_.emplace(STATE::ZOOM_OUT, std::bind(&GameStateEnemyRespown::ChangeStateZoomOut, this));
	changeStateMap_.emplace(STATE::ROAR, std::bind(&GameStateEnemyRespown::ChangeStateRoar, this));
	changeStateMap_.emplace(STATE::END, std::bind(&GameStateEnemyRespown::ChangeStateEnd, this));
}

GameStateEnemyRespown::~GameStateEnemyRespown()
{
}

void GameStateEnemyRespown::Init()
{
	// タイマー
	timer_ = std::make_unique<Timer>();
	timer_->InitCountUp();

	// 画面揺れ
	screenShake_ = std::make_unique<ScreenShake>();
	screenShake_->Init();

	// カメラ制御
	cameraController_ = std::make_unique<ControllerCameraBase>();
	cameraController_->Init();

	// 初期状態へ遷移
	ChangeState(STATE::WAIT);
}

void GameStateEnemyRespown::Update()
{
	// 状態別更新
	update_();

	// ターゲットの更新
	if (target_) { target_->Update(); }
}

void GameStateEnemyRespown::Draw()
{
	// 影描画
	shadowMng_.Draw();

	// ステージ描画
	stageMng_.Draw();

	// キャラクター描画
	charaMng_.Draw();

	// ポストエフェクトの描画
	effectMng_.Draw();
}

void GameStateEnemyRespown::ChangeState(const STATE state)
{
	state_ = state;

	changeStateMap_[state_]();
}

void GameStateEnemyRespown::ChangeStateWait()
{	
	update_ = std::bind(&GameStateEnemyRespown::UpdateWait, this);
}

void GameStateEnemyRespown::ChangeStateStart()
{
	update_ = std::bind(&GameStateEnemyRespown::UpdateStart, this);

	// プレイヤーの活動状態を非表示
	charaMng_.SetIsActive(CharacterManager::TYPE::PLAYER, false);

	// 敵のリストを取得
	auto& enemies = charaMng_.GetCharacters(CharacterManager::TYPE::ENEMY);

	// カメラ位置
	mainCamera.ChangeMode(Camera::MODE::FIXED_POINT);
	mainCamera.SetPos({ -1340, 52, -620 });
	mainCamera.SetTargetPos({ -1430, 30, -802 });

	// リストが空でない場合
	if (!enemies.empty()) 
	{
		// 先頭のポインタを取得してキャストする
		target_ = dynamic_cast<Enemy*>(enemies.front().get());
	}
	else 
	{
		// ターゲットを空にする
		target_ = nullptr;
	}

	// キャストが失敗した場合、アサートを発動する
	assert(target_ != nullptr && "dynamic_castに失敗しました");
}

void GameStateEnemyRespown::ChangeStateWalk()
{
	update_ = std::bind(&GameStateEnemyRespown::UpdateWalk, this);

	// アニメーション開始
	target_->GetControllerAnimation().Play(Enemy::ANIM_WALK);

	// タイマー設定
	timer_->SetGoalTime(2.0f);
}

void GameStateEnemyRespown::ChangeStateMoveCameraForward()
{
	update_ = std::bind(&GameStateEnemyRespown::UpdateMoveCameraForward, this);

	// 初期化
	timer_->InitCountUp();

	// アニメーション待機
	target_->GetControllerAnimation().Play(Enemy::ANIM_IDLE);	
	
	// 移動速度の設定
	target_->SetMoveSpeed(0.0f);

	// カメラ制御
	cameraController_->Set({ -1255, 170, -696 }, {-1450, 125, -686 }, Utility3D::DIR_U, 0.0f, 2.0f);
}

void GameStateEnemyRespown::ChangeStateZoomOut()
{
	update_ = std::bind(&GameStateEnemyRespown::UpdateZoomOut, this);

	// カメラ制御
	cameraController_->Set({ -1080, 113, -705 }, { -1280, 95, -694 }, Utility3D::DIR_U, 0.0f, 1.3f);
}

void GameStateEnemyRespown::ChangeStateRoar()
{
	update_ = std::bind(&GameStateEnemyRespown::UpdateRoar, this);

	// アニメーション開始
	target_->GetControllerAnimation().Play(Enemy::ANIM_ROAR);

	// タイマー設定
	timer_->SetGoalTime(5.0f);
	timer_->InitCountUp();

	// 画面揺れ設定
	screenShake_->Set(4.0, 8.0f);
}

void GameStateEnemyRespown::ChangeStateEnd()
{
	update_ = std::bind(&GameStateEnemyRespown::UpdateEnd, this);

	// 状態遷移
	GameStateManager::GetInstance().ChangeState(GameStateManager::STATE::PLAY);

	// 画面揺れのリセット
	screenShake_->Reset();

	// プレイヤーの活動状態を表示
	charaMng_.SetIsActive(CharacterManager::TYPE::PLAYER, true);

	// カメラをもとに戻す
	mainCamera.ChangeMode(Camera::MODE::FPS);

	// アニメーションを変更
	target_->GetControllerAnimation().Play(Enemy::ANIM_WALK);

	// 敵の状態を変更する
	target_->ChangeState(Enemy::STATE::ALIVE);

	// インスタンスを初期化
	target_ = nullptr;

	// メッセージ表示
	systemMng_.ChangeMessage(Message::TYPE::ENEMY_RESPOWN);
}

void GameStateEnemyRespown::UpdateWait()
{
	ChangeState(STATE::START);
}

void GameStateEnemyRespown::UpdateStart()
{
	ChangeState(STATE::WALK);
}

void GameStateEnemyRespown::UpdateWalk()
{
	// 座標更新
	VECTOR dir = Utility3D::DIR_R;

	// 目的地から移動方向を取得
	target_->SetMoveDir(dir);

	// 目標回転角度の設定
	target_->SetGoalQuaRot(Quaternion::LookRotation(dir));

	// 移動速度の設定
	target_->SetMoveSpeed(0.5f);

	if (timer_->CountUp())
	{
		// 状態遷移
		ChangeState(STATE::MOVE_CAMERA_FORWARD);
	}
}

void GameStateEnemyRespown::UpdateMoveCameraForward()
{
	// カメラの移動が完了している場合
	if (cameraController_->IsEnd())
	{
		// 状態変更
		ChangeState(STATE::ZOOM_OUT);
	}
	else
	{
		// カメラの更新処理
		cameraController_->Update();
	}
}

void GameStateEnemyRespown::UpdateZoomOut()
{
	// カメラの移動が完了している場合
	if (cameraController_->IsEnd())
	{
		// 状態変更
		ChangeState(STATE::ROAR);
	}
	else
	{
		// カメラの更新処理
		cameraController_->Update();
	}
}

void GameStateEnemyRespown::UpdateRoar()
{
	// 画面揺れを行う時間になった場合
	if (timer_->GetCount() > 1.0f)
	{
		// 画面揺れ更新
		screenShake_->Update();
	}
	// 時間に達した場合
	if (timer_->CountUp())
	{
		// 状態遷移
		ChangeState(STATE::END);
	}
}

void GameStateEnemyRespown::UpdateEnd()
{
	ChangeState(STATE::WAIT);

	GameStateManager::GetInstance().ChangeState(GameStateManager::STATE::PLAY);
}