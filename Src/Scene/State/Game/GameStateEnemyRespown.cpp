#include "../../../Application.h"
#include "../../../Manager/Common/Camera.h"
#include "../../../Manager/Common/SceneManager.h"
#include "../../../Manager/Common/SoundManager.h"
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

GameStateEnemyRespown::GameStateEnemyRespown() :
	scnMng_(SceneManager::GetInstance()),
	sndMng_(SoundManager::GetInstance())
{
	state_ = STATE::WAIT;
	target_ = nullptr;
	fireStep_ = 0.0f;
	oldTexture_ = -1;

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
	DeleteGraph(oldTexture_);
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

	// 遷移前用のテクスチャを生成
	oldTexture_ = MakeGraph(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y);

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

	// エフェクト開始
	effectMng_.ChangeEffect(GameEffectManager::TYPE::FIRE_TRANSITION);

	// 遷移前の画面をサブテクスチャに保存
	int result = GetDrawScreenGraph(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, oldTexture_, true);
	effectMng_.SetSubTexture(GameEffectManager::TYPE::FIRE_TRANSITION, oldTexture_);

	// プレイヤーの活動状態を非表示
	charaMng_.SetIsActive(CharacterManager::TYPE::PLAYER, false);

	// 敵のリストを取得
	auto& enemies = charaMng_.GetCharacters(CharacterManager::TYPE::ENEMY);

	// カメラ位置
	mainCamera.ChangeMode(Camera::MODE::FIXED_POINT);
	mainCamera.SetPos(START_CAMERA_POS);
	mainCamera.SetTargetPos(START_CAMERA_TARGET);

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

	// 座標更新
	VECTOR dir = Utility3D::DIR_R;

	// 目的地から移動方向を取得
	target_->SetMoveDir(dir);

	// 目標回転角度の設定
	target_->SetGoalQuaRot(Quaternion::LookRotation(dir));

	// エフェクト用ステップの初期化
	fireStep_ = 0.0f;
	effectMng_.SetStep(GameEffectManager::TYPE::FIRE_TRANSITION, fireStep_);

	// SEの再生
	sndMng_.PlaySe(SoundType::SE::FIRE_TRANSITION);
}

void GameStateEnemyRespown::ChangeStateWalk()
{
	update_ = std::bind(&GameStateEnemyRespown::UpdateWalk, this);

	// エフェクト変更
	effectMng_.ChangeEffect(GameEffectManager::TYPE::NONE);

	// アニメーション開始
	target_->GetControllerAnimation().Play(Enemy::ANIM_WALK);

	// タイマー設定
	timer_->SetGoalTime(WALK_TIME);

	// 移動速度の設定
	target_->SetMoveSpeed(ENEMY_MOVE_SPEED);

	// うめき声を再生
	sndMng_.PlaySe(SoundType::SE::ENEMY_GROAN);
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
	cameraController_->Set(ZOOM_IN_CAMERA_POS, ZOOM_IN_CAMERA_TARGET, Utility3D::DIR_U, 0.0f, CAMERA_MOVE_TIME);
}

void GameStateEnemyRespown::ChangeStateZoomOut()
{
	update_ = std::bind(&GameStateEnemyRespown::UpdateZoomOut, this);

	// カメラ制御
	cameraController_->Set(END_CAMERA_POS, END_CAMERA_TARGET, Utility3D::DIR_U, 0.0f, ZOOM_OUT_TIME);
}

void GameStateEnemyRespown::ChangeStateRoar()
{
	update_ = std::bind(&GameStateEnemyRespown::UpdateRoar, this);

	// アニメーション開始
	target_->GetControllerAnimation().Play(Enemy::ANIM_ROAR);

	// タイマー設定
	timer_->SetGoalTime(ROAR_TIME);
	timer_->InitCountUp();

	// 画面揺れ設定
	screenShake_->Set(SCREEN_SHAKE_TIME, SCREEN_SHAKE_POWER);
}

void GameStateEnemyRespown::ChangeStateEnd()
{
	update_ = std::bind(&GameStateEnemyRespown::UpdateEnd, this);

	// 画面揺れのリセット
	screenShake_->Reset();

	// エフェクト開始
	effectMng_.ChangeEffect(GameEffectManager::TYPE::FIRE_TRANSITION);

	// ステップの初期化	
	fireStep_ = 0.0f;
	effectMng_.SetStep(GameEffectManager::TYPE::FIRE_TRANSITION, 0.0f);

	// 画面をテクスチャに保存
	int result = GetDrawScreenGraph(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, oldTexture_, true);
	effectMng_.SetSubTexture(GameEffectManager::TYPE::FIRE_TRANSITION, oldTexture_);

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

	// SEの再生
	sndMng_.PlaySe(SoundType::SE::FIRE_TRANSITION);
}

void GameStateEnemyRespown::UpdateWait()
{
	ChangeState(STATE::START);
}

void GameStateEnemyRespown::UpdateStart()
{
	fireStep_ += SceneManager::GetInstance().GetDeltaTime() * TRANSITION_EFFECT_SPEED_RATE;
	effectMng_.SetStep(GameEffectManager::TYPE::FIRE_TRANSITION, fireStep_);

	if (fireStep_ > 1.0f)
	{
		// 最大値の設定
		effectMng_.SetStep(GameEffectManager::TYPE::FIRE_TRANSITION, 1.0f);

		// 状態遷移
		ChangeState(STATE::WALK);
	}
}

void GameStateEnemyRespown::UpdateWalk()
{
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
		// SEが再生されていない場合
		if (!sndMng_.IsCheckPlaySe(SoundType::SE::ENEMY_ROAR))
		{
			// SEの再生
			sndMng_.PlaySe(SoundType::SE::ENEMY_ROAR);
		}

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
	fireStep_ += SceneManager::GetInstance().GetDeltaTime() * TRANSITION_EFFECT_SPEED_RATE;
	effectMng_.SetStep(GameEffectManager::TYPE::FIRE_TRANSITION, fireStep_);

	if (fireStep_ > 1.0f)
	{
		// メッセージ表示
		systemMng_.ChangeMessage(Message::TYPE::ENEMY_RESPOWN);

		// 最大値の設定
		effectMng_.SetStep(GameEffectManager::TYPE::FIRE_TRANSITION, 1.0f);

		// 状態遷移
		ChangeState(STATE::WAIT);

		// ゲーム状態遷移
		GameStateManager::GetInstance().ChangeState(GameStateManager::STATE::PLAY);

		// エフェクトを戻す
		effectMng_.ChangeEffect(GameEffectManager::TYPE::GAME_SCREEN);
	}
}