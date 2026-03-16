#include <DxLib.h>
#include "../../Application.h"
#include "../../Manager/Common/SceneManager.h"
#include "../../Manager/Common/SoundManager.h"
#include "../../Core/PostEffect/PostEffectTvError.h"
#include "../../Core/Common/Timer.h"
#include "../../SceneTitle.h"
#include "TitleStateTvError.h"

TitleStateTvError::TitleStateTvError(SceneTitle& parent, const Json& param) :
	TitleStateBase(parent),
	TIME(param["time"]),
	POST_EFFECT_PARAM(param["postEffectParam"])
{
	screen_ = 0;
	postEffectTvError_ = nullptr;
	isChangeScene_ = true;
}

TitleStateTvError::~TitleStateTvError()
{
	DeleteGraph(screen_);
}

void TitleStateTvError::Init()
{
	screen_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);

	postEffectTvError_ = std::make_unique<PostEffectTvError>(POST_EFFECT_PARAM);
	postEffectTvError_->Init();

	timer_ = std::make_unique<Timer>(TIME);
	timer_->InitCountUp();
}

void TitleStateTvError::Update()
{
	if (isChangeScene_ && timer_->CountUp())
	{
		scnMng_.ChangeScene(SceneManager::SCENE_ID::GAME, Fader::STATE::FINISH);
		isChangeScene_ = false;
		sndMng_.StopBgm(SoundType::BGM::TITLE_SELECT);
		sndMng_.StopSe(SoundType::SE::TV_ERROR);
	}
}

void TitleStateTvError::Draw()
{
	//スクリーンの設定
	SetDrawScreen(screen_);

	// 画面を初期化
	ClearDrawScreen();

	// ポストエフェクト描画
	postEffectTvError_->Draw();

	// メインに戻す
	SetDrawScreen(scnMng_.GetMainScreen());

	// 描画
	DrawGraph(0, 0, screen_, false);
}
