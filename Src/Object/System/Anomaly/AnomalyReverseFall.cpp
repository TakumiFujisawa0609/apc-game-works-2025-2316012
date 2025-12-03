#include "../../../Manager/Game/AnomalyManager.h"
#include "../../../Manager/Game/GameSystemManager.h"
#include "../../../Manager/Game/CharacterManager.h"
#include "../../../Manager/Common/SceneManager.h"
#include "../../../Manager/Common/Camera.h"
#include "../../../Manager/Common/SoundManager.h"
#include "../../../Utility/UtilityCommon.h"
#include "../../../Utility/Utility3D.h"
#include "../../../Common/Quaternion.h"
#include "../../../Core/Common/ScreenShake.h"
#include "../../Controller/Camera/ControllerCameraJumpScare.h"
#include "../../Actor/Character/Player.h"
#include "AnomalyReverseFall.h"

AnomalyReverseFall::AnomalyReverseFall(const Json& param) :
	AnomalyBase(param),
	CAMERA_PULL_TIME(param["cameraPullTime"]),
	CAMERA_BACK_POW(param["cameraBackPow"]),
	SCREEN_SHAKE_TIME(param["screenShakeTime"]),
	SCREEN_SHAKE_POW(param["screenShakePow"])
{
	state_ = STATE::NONE;
	camera_ = nullptr;
	screenShake_ = nullptr;

	// 状態変更処理の登録
	changeStateMap_.emplace(STATE::NONE, std::bind(&AnomalyReverseFall::ChangeStateNone, this));
	changeStateMap_.emplace(STATE::CAMERA_PULL, std::bind(&AnomalyReverseFall::ChangeStateCameraPull, this));
	changeStateMap_.emplace(STATE::EARTH_QUAKE, std::bind(&AnomalyReverseFall::ChangeStateEarthQuake, this));
	changeStateMap_.emplace(STATE::REVERSE_FALL, std::bind(&AnomalyReverseFall::ChangeStateReverseFall, this));
	changeStateMap_.emplace(STATE::REVERSE_UP, std::bind(&AnomalyReverseFall::ChangeStateReverseUp, this));
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

	// 状態をNONEに変更
	ChangeState(STATE::NONE);
}

void AnomalyReverseFall::Occurrence()
{	
	CharacterManager& charaMng = CharacterManager::GetInstance();
	AnomalyManager& anomalyMng = AnomalyManager::GetInstance();
	const auto& player = dynamic_cast<Player*>(&charaMng_.GetCharacter(CharacterManager::TYPE::PLAYER));

	// システムのタイマー停止
	systemMng_.SetIsActiveSystem(GameSystemManager::TYPE::GAME_TIMER, false);

	// UIの非表示
	systemMng_.SetIsActiveSystem(GameSystemManager::TYPE::SCREEN, false);
	
	// 敵の更新停止
	charaMng.SetIsActive(CharacterManager::TYPE::ENEMY, false);

	// プレイヤーの状態変更
	player->ChangeState(Player::STATE::HAPPENING);

	// この異変中新しい異変の追加を停止
	anomalyMng.SetIsOccurrence(false);

	// カメラの後ろ方向
	VECTOR backDir = VScale(mainCamera.GetForward(), -1.0f);

	// ベクトル生成
	VECTOR cameraVec = VScale(backDir, CAMERA_BACK_POW);

	// 目的地の取得
	VECTOR goalPos = VAdd(mainCamera.GetPos(), cameraVec);

	// 注視点の位置
	VECTOR targetGoalPos = mainCamera.GetTargetPos();
	targetGoalPos.y = 0;	// 正面を向くようにY値を初期化

	// カメラ設定
	camera_->Set(goalPos, targetGoalPos, CAMERA_PULL_TIME);

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
	// カメラをZ軸で半回転、Y軸落下と同時に暗転させていく
	//mainCamera.SetAngles(VAdd(mainCamera.GetAngles(), { 0,0,10 }));
}

void AnomalyReverseFall::UpdateReverseUp()
{
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
	// 更新処理の変更
	update_ = std::bind(&AnomalyReverseFall::UpdateEarthQuake, this);

	// 効果音再生
	sndMng_.PlaySe(SoundType::SE::DRUM_ROLL_END);

	// 画面揺れ設定
	screenShake_->Set(SCREEN_SHAKE_TIME, SCREEN_SHAKE_POW);
}

void AnomalyReverseFall::ChangeStateReverseFall()
{
	// 更新処理の変更
	update_ = std::bind(&AnomalyReverseFall::UpdateReverseFall, this);
}

void AnomalyReverseFall::ChangeStateReverseUp()
{
	// 更新処理の変更
	update_ = std::bind(&AnomalyReverseFall::UpdateReverseUp, this);
}