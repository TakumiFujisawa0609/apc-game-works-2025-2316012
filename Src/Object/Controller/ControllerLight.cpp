#include "../../Application.h"
#include "../../Manager/Generic/SceneManager.h"
#include "../../Manager/Generic/Camera.h"
#include "../../Manager/Generic/CollisionManager.h"
#include "../../Manager/Generic/InputManager.h"
#include "../../Manager/Resource/SoundManager.h"
#include "../../Utility/Utility3D.h"
#include "../Actor/ActorBase.h"
#include "../Collider/ColliderLine.h"
#include "../Collider/ColliderModel.h"
#include "ControllerLight.h"

ControllerLight::ControllerLight(ActorBase& owner) :
	owner_(owner),
	input_(InputManager::GetInstance()),
	sndMng_(SoundManager::GetInstance())
{
	lightPos_ = Utility3D::VECTOR_ZERO;
	collider_ = nullptr;
	isLight_ = false;
}

ControllerLight::~ControllerLight()
{
}

void ControllerLight::Load()
{
	// プレイヤーライトのコライダー
	collider_ = std::make_shared<ColliderLine>(owner_, CollisionTags::TAG::PLAYER_LIGHT);

	// コライダー追加
	CollisionManager::GetInstance().Add(collider_);
}

void ControllerLight::Init()
{
	// 初期状態は電源を付けておく
	isLight_ = true;

	Update();
}

void ControllerLight::Update()
{
	// 画面中心の3D座標
	const VECTOR screenCenter = ConvScreenPosToWorldPos({ Application::SCREEN_HALF_X,Application::SCREEN_HALF_Y, 0 });

	// 末端の位置を取得
	VECTOR endPos = VAdd(screenCenter, VScale(mainCamera.GetForward(), 500.0f));

	// 先端位置を取得
	VECTOR startPos = owner_.GetTransform().pos;
	//startPos.y += 120.0f;

	// 先端位置設定
	collider_->SetLocalPosPointHead(screenCenter);

	// 末端位置の設定
	collider_->SetLocalPosPointEnd(endPos);

	// 末端の位置を取得
	lightPos_ = endPos;

	// 電源の判定
	if (input_.IsTrgDown(InputManager::TYPE::LIGHT_SWITCH))
	{
		// 電源の切り替え
		isLight_ = !isLight_;

		// 効果音の再生
		sndMng_.PlaySe(SoundType::SE::NOISE_SWITCH);
	}
}