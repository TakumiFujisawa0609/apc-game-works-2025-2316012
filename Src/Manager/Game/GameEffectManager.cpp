#include <DxLib.h>
#include "../../Application.h"
#include "../../Manager/Common/SceneManager.h"
#include "../../Core/PostEffect/PostEffectCracks.h"
#include "../../Core/PostEffect/PostEffectGameScreen.h"
#include "GameEffectManager.h"

void GameEffectManager::Init()
{
	// ゲームスクリーン
	auto game = std::make_unique<PostEffectGameScreen>();
	effectMap_.emplace(TYPE::GAME_SCREEN, std::move(game));

	// ひび割れ
	auto cracks = std::make_unique<PostEffectCracks>();
	effectMap_.emplace(TYPE::CRACKS, std::move(cracks));

	// 読み込み処理
	for (auto& effect : effectMap_)
	{
		effect.second->Load();
		effect.second->Init();
	}

	// エフェクト画面の生成
	effectScreen_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);

	// 初期時の種類
	type_ = TYPE::GAME_SCREEN;
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
	type_ = TYPE::NONE;
	effectScreen_ = -1;
}

GameEffectManager::~GameEffectManager()
{
	DeleteGraph(effectScreen_);
}
