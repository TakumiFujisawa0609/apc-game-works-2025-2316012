#include <DxLib.h>
#include "../../Application.h"
#include "../../Manager/Common/SceneManager.h"
#include "../../Core/PostEffect/PostEffectGameNormal.h"
#include "../../Core/PostEffect/PostEffectNightView.h"
#include "../../Core/PostEffect/PostEffectCracks.h"
#include "GameEffectManager.h"

void GameEffectManager::Init()
{
	// ノーマル
	auto normalEffect = std::make_unique<PostEffectGameNormal>();
	effectMap_.emplace(TYPE::NORMAL, std::move(normalEffect));

	// 暗視
	auto nightViewEffect = std::make_unique<PostEffectNightView>();
	effectMap_.emplace(TYPE::NIGHT_VIEW, std::move(nightViewEffect));

	// ひび割れ
	auto cracks = std::make_unique<PostEffectCracks>();
	effectMap_.emplace(TYPE::NIGHT_VIEW, std::move(cracks));

	// 読み込み処理
	for (auto& effect : effectMap_)
	{
		effect.second->Load();
		effect.second->Init();
	}

	// エフェクト画面の生成
	effectScreen_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);

	// 初期時の種類
	type_ = TYPE::NORMAL;
}

void GameEffectManager::Draw()
{
	// 種類が指定されてない場合描画しない
	if (type_ == TYPE::NONE) { return; }

	//スクリーンの設定
	SetDrawScreen(effectScreen_);

	// 画面を初期化
	ClearDrawScreen();

	// エフェクトを描画
	effectMap_[type_]->Draw();

	// メインに戻す
	SetDrawScreen(SceneManager::GetInstance().GetMainScreen());

	// 描画
	DrawGraph(0, 0, effectScreen_, false);
}

GameEffectManager::GameEffectManager()
{
	DeleteGraph(effectScreen_);
}

GameEffectManager::~GameEffectManager()
{
	effectScreen_ = -1;
}
