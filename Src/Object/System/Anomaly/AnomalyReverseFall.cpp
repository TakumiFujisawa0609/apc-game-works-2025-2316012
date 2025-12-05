#include "../../../Manager/Game/AnomalyManager.h"
#include "../../../Manager/Game/GameSystemManager.h"
#include "../../../Manager/Game/CharacterManager.h"
#include "../../../Manager/Game/StageManager.h"
#include "../../../Manager/Common/SceneManager.h"
#include "../../../Manager/Common/Camera.h"
#include "../../../Manager/Common/SoundManager.h"
#include "../../../Manager/Common/SceneManager.h"
#include "../../../Utility/UtilityCommon.h"
#include "../../../Utility/Utility3D.h"
#include "../../../Common/Quaternion.h"
#include "../../../Core/Common/ScreenShake.h"
#include "../../../Core/Common/Timer.h"
#include "../../Controller/Camera/ControllerCameraJumpScare.h"
#include "../../Controller/ControllerAnimation.h"
#include "../../Actor/Character/Player.h"
#include "../../Actor/Stage/StageMesh.h"
#include "../../Actor/Stage/CherryBlossomsWorld.h"
#include "AnomalyReverseFall.h"

AnomalyReverseFall::AnomalyReverseFall(const Json& param) :
	AnomalyBase(param),
	MADNESS_UP_TIME(param["madnessUpTime"]),
	CAMERA_PULL_TIME(param["cameraPullTime"]),
	CAMERA_BACK_POW(param["cameraBackPow"]),
	SCREEN_SHAKE_TIME(param["screenShakeTime"]),
	SCREEN_SHAKE_POW(param["screenShakePow"]),
	CAMERA_DOWN_POW(param["cameraDownPow"]),
	CAMERA_DOWN_TIME(param["cameraDownTime"]),
	OBJ_TRANSFORM(param["transform"])
{
	state_ = STATE::NONE;
	camera_ = nullptr;
	screenShake_ = nullptr;
	preCameraPos_ = Utility3D::VECTOR_ZERO;
	preTargetPos_ = Utility3D::VECTOR_ZERO;

	// 状態変更処理の登録
	changeStateMap_.emplace(STATE::NONE, std::bind(&AnomalyReverseFall::ChangeStateNone, this));
	changeStateMap_.emplace(STATE::CAMERA_PULL, std::bind(&AnomalyReverseFall::ChangeStateCameraPull, this));
	changeStateMap_.emplace(STATE::EARTH_QUAKE, std::bind(&AnomalyReverseFall::ChangeStateEarthQuake, this));
	changeStateMap_.emplace(STATE::REVERSE_FALL, std::bind(&AnomalyReverseFall::ChangeStateReverseFall, this));
	changeStateMap_.emplace(STATE::REVERSE_UP, std::bind(&AnomalyReverseFall::ChangeStateReverseUp, this));
	changeStateMap_.emplace(STATE::CAMRA_BACK, std::bind(&AnomalyReverseFall::ChangeStateCameraBack, this));
	changeStateMap_.emplace(STATE::MADNESS_TIME, std::bind(&AnomalyReverseFall::ChangeStaetMadnessTime, this));
}

AnomalyReverseFall::~AnomalyReverseFall()
{
}

void AnomalyReverseFall::Init()
{
	// カメラ生成
	camera_ = std::make_unique<ControllerCameraJumpScare>();

	// 画面シェイク
	screenShake_ = std::make_unique<ScreenShake>();
	screenShake_->Init();

	// タイマー
	timer_ = std::make_unique<Timer>(MADNESS_UP_TIME);
	timer_->InitCountUp();

	// 状態をNONEに変更
	ChangeState(STATE::NONE);
}

void AnomalyReverseFall::Occurrence()
{	
	CharacterManager& charaMng = CharacterManager::GetInstance();
	AnomalyManager& anomalyMng = AnomalyManager::GetInstance();
	const auto& player = dynamic_cast<Player*>(&charaMng_.GetCharacter(CharacterManager::TYPE::PLAYER));

	// プレイヤー位置のバックアップを保持
	prePlayerPos_ = player->GetTransform().pos;

	// システムのタイマー停止
	systemMng_.SetIsActiveSystem(GameSystemManager::TYPE::GAME_TIMER, false);

	// UIの非表示
	systemMng_.SetIsActiveSystem(GameSystemManager::TYPE::SCREEN, false);
	systemMng_.SetIsActiveSystem(GameSystemManager::TYPE::MANUAL, false);
	
	// 敵の更新停止
	charaMng.SetIsActive(CharacterManager::TYPE::ENEMY, false);

	// プレイヤーの状態変更
	player->ChangeState(Player::STATE::HAPPENING);

	// この異変中新しい異変の追加を停止
	anomalyMng.SetIsOccurrence(false);

	// カメラや注視点位置のバックアップを保持
	preCameraPos_ = mainCamera.GetPos();
	preTargetPos_ = mainCamera.GetTargetPos();

	// カメラの後ろ方向
	VECTOR backDir = VScale(mainCamera.GetForward(), -1.0f);

	// ベクトル生成
	VECTOR cameraVec = VScale(backDir, CAMERA_BACK_POW);

	// 目的地の取得
	VECTOR goalPos = VAdd(preCameraPos_, cameraVec);

	// 注視点の位置
	VECTOR targetGoalPos = preTargetPos_;
	targetGoalPos.y = 0;	// 正面を向くようにY値を初期化

	// カメラ設定
	camera_->Set(goalPos, targetGoalPos, Utility3D::DIR_U, 0.0f, CAMERA_PULL_TIME);

	// 状態変更
	ChangeState(STATE::CAMERA_PULL);
}

void AnomalyReverseFall::Update()
{
	update_();
}

void AnomalyReverseFall::UpdateCameraPull()
{
	// カメラの移動が完了している場合
	if (camera_->IsEnd())
	{
		// 状態変更
		ChangeState(STATE::EARTH_QUAKE);
	}
	else
	{
		// カメラの更新処理
		camera_->Update();
	}
}

void AnomalyReverseFall::UpdateEarthQuake()
{
	// 終了判定
	if (screenShake_->IsEnd())
	{
		// 状態変更
		ChangeState(STATE::REVERSE_FALL);
	}
	else
	{
		// 画面シェイク更新
		screenShake_->Update();
	}
}

void AnomalyReverseFall::UpdateReverseFall()
{
	// カメラの移動が完了している場合
	if (camera_->IsEnd())
	{
		// 状態変更
		ChangeState(STATE::REVERSE_UP);
	}
	else
	{
		// カメラの更新処理
		camera_->Update();
	}
}

void AnomalyReverseFall::UpdateReverseUp()
{
	// カメラの移動が完了している場合
	if (camera_->IsEnd())
	{
		// 状態変更
		ChangeState(STATE::CAMRA_BACK);
	}
	else
	{
		// カメラの更新処理
		camera_->Update();
	}
}

void AnomalyReverseFall::UpdateCameraBack()
{
	// カメラの移動が完了している場合
	if (camera_->IsEnd())
	{
		// 演出終了後の処理
		AfterDirection();

		// 状態変更
		ChangeState(STATE::MADNESS_TIME);
	}
	else
	{
		// カメラの更新処理
		camera_->Update();
	}
}

void AnomalyReverseFall::UpdateMadnessTime()
{
	// 更新時間になった場合
	if (timer_->CountUp())
	{
		// プレイヤーの狂気値を上昇させる
		const auto& player = dynamic_cast<Player*>(&charaMng_.GetCharacter(CharacterManager::TYPE::PLAYER));
		player->AddMadnessValue(1);
	}
}

void AnomalyReverseFall::ChangeState(const STATE state)
{
	// 状態変更
	state_ = state;

	// 状態別変更処理
	changeStateMap_[state]();
}

void AnomalyReverseFall::ChangeStateNone()
{
	// 更新処理の変更
	update_ = std::bind(&AnomalyReverseFall::UpdateNone, this);
}

void AnomalyReverseFall::ChangeStateCameraPull()
{
	// 更新処理の変更
	update_ = std::bind(&AnomalyReverseFall::UpdateCameraPull, this);
}

void AnomalyReverseFall::ChangeStateEarthQuake()
{
	const auto& player = dynamic_cast<Player*>(&charaMng_.GetCharacter(CharacterManager::TYPE::PLAYER));

	// 更新処理の変更
	update_ = std::bind(&AnomalyReverseFall::UpdateEarthQuake, this);

	// 効果音再生
	sndMng_.PlaySe(SoundType::SE::DRUM_ROLL_END);

	// 画面揺れ設定
	screenShake_->Set(SCREEN_SHAKE_TIME, SCREEN_SHAKE_POW);

	// プレイヤーのアニメーションを行う
	player->GetControllerAnimation().Play(Player::ANIM_LOOK_AROUND);
}

void AnomalyReverseFall::ChangeStateReverseFall()
{
	// 更新処理の変更
	update_ = std::bind(&AnomalyReverseFall::UpdateReverseFall, this);

	// 目的地を現在位置からかなり下まで下げる
	VECTOR goalPos = VAdd(mainCamera.GetPos(), { 0 ,-CAMERA_DOWN_POW, 0 });

	// カメラ設定
	camera_->Set(goalPos, mainCamera.GetTargetPos() , mainCamera.GetForward(), CAMERA_ROTATION_DEG, CAMERA_DOWN_TIME);
}

void AnomalyReverseFall::ChangeStateReverseUp()
{
	// 更新処理の変更
	update_ = std::bind(&AnomalyReverseFall::UpdateReverseUp, this);

	// 最初の位置まで戻す
	VECTOR goalPos = VAdd(mainCamera.GetPos(), { 0 ,CAMERA_DOWN_POW, 0 });

	// カメラ設定
	camera_->Set(goalPos, mainCamera.GetTargetPos(), mainCamera.GetForward(), CAMERA_ROTATION_DEG, CAMERA_DOWN_TIME);

	// ステージの生成
	CreateStage();

	// フォグの距離を変更
	SetFogStartEnd(FOG_START, FOG_END);
}

void AnomalyReverseFall::ChangeStateCameraBack()
{
	// 更新処理の変更
	update_ = std::bind(&AnomalyReverseFall::UpdateCameraBack, this);

	// カメラ設定
	camera_->Set(preCameraPos_, preTargetPos_, Utility3D::DIR_U, 0.0f, CAMERA_PULL_TIME);
}

void AnomalyReverseFall::ChangeStaetMadnessTime()
{
	// 更新処理の変更
	update_ = std::bind(&AnomalyReverseFall::UpdateMadnessTime, this);

	// タイマー初期化
	timer_->InitCountUp();
}

void AnomalyReverseFall::CreateStage()
{
	// 全てのステージオブジェクトの活動を停止
	stageMng_.SetIsActiveByAllObjects(false);

	// 判定用のメッシュと描画用のステージの2種を生成
	auto mesh = std::make_unique<StageMesh>(KEY_MESH, OBJ_TRANSFORM, stageMng_.GetStageObjectColliderParam(KEY_MESH));
	auto main = std::make_unique<CherryBlossomsWorld>(KEY_MAIN, OBJ_TRANSFORM, stageMng_.GetStageObjectColliderParam(KEY_MAIN), prePlayerPos_);

	mesh->Load();
	mesh->Init();

	main->Load();
	main->Init();

	// 追加
	stageMng_.Add(KEY_MESH, std::move(mesh));
	stageMng_.Add(KEY_MAIN, std::move(main));
}

void AnomalyReverseFall::AfterDirection()
{
	const auto& player = dynamic_cast<Player*>(&charaMng_.GetCharacter(CharacterManager::TYPE::PLAYER));

	// プレイヤーの状態を戻す
	player->ChangeState(Player::STATE::ALIVE);

	// アニメーションを待機に戻す
	player->GetControllerAnimation().Play(Player::ANIM_IDLE);

	// UIの表示
	systemMng_.SetIsActiveSystem(GameSystemManager::TYPE::SCREEN, true);
	systemMng_.SetIsActiveSystem(GameSystemManager::TYPE::MANUAL, true);

	// カメラをFPS視点操作に戻す
	mainCamera.ChangeMode(Camera::MODE::FPS);
}
