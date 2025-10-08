#include "../../../Application.h"
#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/Generic/Camera.h"
#include "../../../Manager/Generic/InputManager.h"
#include "../../../Manager/Generic/CollisionManager.h"
#include "../../../Manager/Generic/CollisionTags.h"
#include "../../../Manager/Generic/Camera.h"
#include "../../../Utility/Utility3D.h"
#include "../../../Utility/UtilityCommon.h"
#include "../../Actor/Character/Player.h"
#include "../../Collider/ColliderLine.h"
#include "../ControllerAnimation.h"
#include "ControllerActionPlayer.h"

ControllerActionPlayer::ControllerActionPlayer(Player& player) :
	ControllerActionBase(player),
	player_(player),
	input_(InputManager::GetInstance()),
	collMng_(CollisionManager::GetInstance()),
	REPORT_INPUT_TIME(player.GetReportTime())
{
	isEndLanding_ = false;
}

ControllerActionPlayer::~ControllerActionPlayer()
{
}

void ControllerActionPlayer::Load()
{
}

void ControllerActionPlayer::Init()
{
	// 各種変数の初期化
	isEndLanding_ = false;
}

void ControllerActionPlayer::Update()
{
	// 移動操作処理
	ProcessMove();

	// ジャンプ操作処理
	ProcessJump();

	// レポート操作処理
	ProcessReport();
}

void ControllerActionPlayer::ProcessMove()
{
	// X軸回転を除いた、重力方向に垂直なカメラ角度(XZ平面)を取得
	Quaternion cameraRot = mainCamera.GetQuaRotOutX();

	// プレイヤーのジャンプ判定
	bool isJump = player_.IsJump();

	// 回転したい角度
	double rotDeg = -1;

	// 方向ベクトル
	VECTOR dir = Utility3D::VECTOR_ZERO;

	// 移送速度
	float speed = 0.0f;

	// 右移動
	if (input_.IsNew(InputManager::TYPE::PLAYER_MOVE_RIGHT))
	{
		rotDeg = ROT_DEG_RIGHT;
		dir = cameraRot.GetRight();
	}

	// 左移動
	if (input_.IsNew(InputManager::TYPE::PLAYER_MOVE_LEFT))
	{
		rotDeg = ROT_DEG_LEFT;
		dir = cameraRot.GetLeft();
	}

	// 前移動
	if (input_.IsNew(InputManager::TYPE::PLAYER_MOVE_UP))
	{
		rotDeg = ROT_DEG_FRONT;
		dir = cameraRot.GetForward();
	}

	// 後移動
	if (input_.IsNew(InputManager::TYPE::PLAYER_MOVE_DOWN))
	{
		rotDeg = ROT_DEG_BACK;
		dir = cameraRot.GetBack();
	}

	// プレイヤーの着地判定
	isEndLanding_ = IsEndLanding();

	// アニメーション制御クラスを取得
	auto& animation = player_.GetControllerAnimation();

	// ジャンプ中じゃないかつ操作入力がされたとき
	if (!Utility3D::EqualsVZero(dir) && (isJump || isEndLanding_))
	{
		// ダッシュ
		bool isDash = input_.IsNew(InputManager::TYPE::PLAYER_DASH);

		// 歩く移動速度取得
		speed = player_.GetSpeedMove();

		// ダッシュ中なら速度を変更
		if (isDash)
		{
			speed = player_.GetSpeedRun();
		}

		if (!isJump && isEndLanding_)
		{
			// アニメーション
			if (isDash)
			{
				animation.Play(Player::ANIM_RUN);
			}
			else
			{
				animation.Play(Player::ANIM_WALK);
			}
		}
	}
	else
	{
		if (!isJump && IsEndLanding())
		{
			animation.Play(Player::ANIM_IDLE);
		}
	}

	// 移動ベクトルの設定
	player_.SetMoveDir(dir);

	// 移動量の設定
	player_.SetMoveSpeed(speed);

	// 回転角度の設定
	player_.SetRotDeg(rotDeg);
}

void ControllerActionPlayer::ProcessJump()
{
	// 現在のジャンプ判定
	bool isJump = player_.IsJump();

	// 現在のジャンプ用ステップ
	float stepJump = player_.GetStepJump();

	// ジャンプ受付時間
	const float JUMP_ACCEPT_TIME = player_.GetJumpAcceptTime();

	// ジャンプキーを入力したか
	bool isInputJump = input_.IsNew(InputManager::TYPE::PLAYER_JUMP);

	// アニメーション制御クラスを取得
	auto& animation = player_.GetControllerAnimation();

	// ジャンプ
	if (isInputJump && (isJump || isEndLanding_))
	{
		// ジャンプ中ではない場合
		if (!isJump)
		{
			// 制御無しジャンプ
			//mAnimationController->Play((int)ANIM_TYPE::JUMP);
			// ループしないジャンプ
			//mAnimationController->Play((int)ANIM_TYPE::JUMP, false);
			// 切り取りアニメーション
			//mAnimationController->Play((int)ANIM_TYPE::JUMP, false, 13.0f, 24.0f);
			// 無理やりアニメーション
			animation.Play(Player::ANIM_JUMP, true, 13.0f, 25.0f);
			animation.SetEndLoop(23.0f, 25.0f, 5.0f);
		}

		// ジャンプ中に設定
		isJump = true;

		// ジャンプの入力受付時間をヘラス
		stepJump += scnMng_.GetDeltaTime();
		if (stepJump < JUMP_ACCEPT_TIME)
		{
			player_.SetJumpPow(VScale(Utility3D::DIR_U, player_.GetJumpAmount()));
		}
	}

	// ボタンを離したらジャンプ力に加算しない
	if (!isInputJump)
	{
		stepJump = JUMP_ACCEPT_TIME;
	}

	// ジャンプ判定の設定
	player_.SetIsJump(isJump);

	// ステップの設定
	player_.SetStepJump(stepJump);
}

void ControllerActionPlayer::ProcessReport()
{
	// 初期パーセント(未入力の場合は0)
	float newPercent = 0;

	// 入力中の場合
	if(input_.IsNew(InputManager::TYPE::ANOMARY_REPORT))
	{		
		// レポート進捗率取得
		float nowPercent = player_.GetReportPercent();
		
		// ゲージの更新値を計算
		float step = GAUGE_MAX / REPORT_INPUT_TIME * scnMng_.GetDeltaTime();

		// 新しい進捗率を計算
		newPercent = nowPercent + step;
	}

	// 進捗率が最大値以上の場合
	if (GAUGE_MAX <= newPercent)
	{
		// 線の判定を生成
		CreateLineCollider();
	}

	// 設定
	player_.SetReportPercent(newPercent);
}

bool ControllerActionPlayer::IsEndLanding() const
{
	//アニメーション制御クラスを取得
	auto& animation = player_.GetControllerAnimation();

	// アニメーションがジャンプではない
	if (animation.GetPlayType() != Player::ANIM_JUMP)
	{
		return true;
	}

	// アニメーションが終了しているか
	if (animation.IsEnd())
	{
		return true;
	}

	return false;
}

void ControllerActionPlayer::CreateLineCollider()
{
	//コライダー生成
	std::shared_ptr<ColliderLine> coll = std::make_shared<ColliderLine>(player_, CollisionTags::TAG::REPORT);

	// 線の長さを定義（ワールド座標系での距離）
	constexpr float LINE_DISTANCE = 50.0f; // 例として50.0ユニット

	// 画面中心を取得
	VECTOR worldDir = ConvScreenPosToWorldPos({ Application::SCREEN_HALF_X,Application::SCREEN_HALF_Y, 0 });

	// カメラ位置を取得
	VECTOR camera = mainCamera.GetPos();

	// 方向ベクトルの取得
	VECTOR dir = VNorm(VSub(worldDir, camera));

	// 末端の位置を取得
	VECTOR endPos = (camera, VScale(dir, LINE_DISTANCE));

	// 先端位置設定
	coll->SetLocalPosPointHead(camera);

	// 末端位置の設定
	coll->SetLocalPosPointEnd(endPos);

	// 判定後すぐ消す
	//coll->SetDelete();

	// 判定に追加
	collMng_.Add(std::move(coll));
}