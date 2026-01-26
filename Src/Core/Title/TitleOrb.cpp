#include <DxLib.h>
#include "../../Manager/Common/ResourceManager.h"
#include "../../Application.h"
#include "../Common/Timer.h"
#include "TitleOrb.h"

TitleOrb::TitleOrb()
{
	imgOrb_ = -1;
}

TitleOrb::~TitleOrb()
{
}

void TitleOrb::Init()
{
	// 画像ハンドルの取得
	imgOrb_ = resMng_.GetHandle("titleOrb");

	// タイマーの生成
	timer_ = std::make_unique<Timer>(CREATE_INTERVAL);
	timer_->InitCountUp();

	// オーブの初期生成
	CreateOrb();
}

void TitleOrb::Update()
{
	// 位置の更新
	for (size_t i = 0; i < orbs_.size(); ++i)
	{
		orbs_[i].img.pos.x += orbs_[i].speedX;
		orbs_[i].img.pos.y -= orbs_[i].speedY;

		// 画面外に出たら削除
		if (orbs_[i].img.pos.x > DELETE_POS_X || 
			orbs_[i].img.pos.y < DELETE_POS_Y)
		{
			orbs_.erase(orbs_.begin() + i);
			--i;
		}
	}

	// オーブの生成
	if (timer_->CountUp())
	{
		CreateOrb();
	}
}

void TitleOrb::Draw()
{
	// 描画
	for (const Orb& orb : orbs_)
	{
		orb.img.DrawRota();
	}
}

void TitleOrb::CreateOrb()
{
	// 生成
	Orb orb;
	orb.speedX = static_cast<float>(SPEED_MIN + GetRand(SPEED_MAX - SPEED_MIN));
	orb.speedY = static_cast<float>(SPEED_MIN + GetRand(SPEED_MAX - SPEED_MIN));
	orb.img.handleId = imgOrb_;
	orb.img.pos.x = static_cast<float>(GetRand(Application::SCREEN_HALF_X));
	orb.img.pos.y = static_cast<float>(CREATE_POS_Y);
	orb.img.scale = (SCALE_MIN + GetRand(SCALE_MAX - SCALE_MIN)) * 0.1f;
	orbs_.emplace_back(orb);
}
