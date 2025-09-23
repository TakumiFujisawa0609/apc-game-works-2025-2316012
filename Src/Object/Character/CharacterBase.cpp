#include <DxLib.h>
#include "../../Manager/Generic/SceneManager.h"
#include "../../Manager/Generic/Camera.h"
#include "../../Utility/UtilityCommon.h"
#include "../../Utility/Utility3D.h"
#include "../Controller/ControllerAnimation.h"
#include "../Controller/Action/ControllerActionBase.h"
#include "../Controller/ControllerMove.h"
#include "../Controller/ControllerRotate.h"
#include "CharacterBase.h"

const std::string CharacterBase::ANIM_IDLE = "idle";	// 待機
const std::string CharacterBase::ANIM_WALK = "walk";	// 歩く
const std::string CharacterBase::ANIM_RUN = "run";		// 走る

CharacterBase::CharacterBase(const Json& param) :
	SPEED_MOVE(param["moveSpeed"]),
	SPEED_RUN(param["dashSpeed"]),
	GRAVITY(param["gravity"]),
	TIME_ROT(param["timeRot"]),
	ANIM_DEFAULT_SPEED(param["animationDefaultSpeed"]),
	INITIAL_POS({ param["initialPosition"]["x"],param["initialPosition"]["y"],param["initialPosition"]["z"] })
{
	rotDeg_ = 0.0f;	
	moveSpeed_ = 0.0f;
	moveDir_ = Utility3D::VECTOR_ZERO;
	animation_ = nullptr;
	action_ = nullptr;
	move_ = nullptr;
	rotate_ = nullptr;
}

void CharacterBase::Load()
{
	// アニメーションの制御クラスの生成
	animation_ = std::make_unique<ControllerAnimation>(transform_.modelId);

	// 移動制御クラス
	move_ = std::make_unique<ControllerMove>(*this);

	//回転制御クラス
	rotate_ = std::make_unique<ControllerRotate>(*this);

	// アニメーション初期化
	InitAnimation();
}

void CharacterBase::Init()
{	
	// トランスフォームの初期設定
	InitTransform();

	// 移動制御クラスの初期化
	move_->Init();

	// 回転制御クラスの初期化
	rotate_->Init();
}

void CharacterBase::UpdateApply()
{
	// トランスフォームの更新
	transform_.Update();

	// アニメーション制御クラスの更新
	animation_->Update();
}

void CharacterBase::DrawMain()
{
	MV1DrawModel(transform_.modelId);
}

void CharacterBase::InitTransform()
{
	transform_.quaRot = Quaternion();
	transform_.scl = Utility3D::VECTOR_ONE;
	transform_.quaRotLocal = Quaternion::Euler({ 0.0f, UtilityCommon::Deg2RadF(DEFAULT_LOCAL_DEG_Y), 0.0f });
	transform_.pos = INITIAL_POS;
	transform_.Update();
}