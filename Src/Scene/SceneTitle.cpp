#include <string>
#include <cmath>
#include <DxLib.h>
#include "../Application.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Resource/SoundManager.h"
#include "../Manager/Resource/FontManager.h"
#include "../Render/PixelMaterial.h"
#include "../Render/PixelRenderer.h"
#include "State/Title/TitleStateExplanation.h"
#include "State/Title/TitleStateMain.h"
#include "State/Title/TitleStateSelect.h"
#include "SceneTitle.h"

SceneTitle::SceneTitle()
{
	// 更新関数のセット
	updataFunc_ = std::bind(&SceneTitle::LoadingUpdate, this);

	// 描画関数のセット
	drawFunc_ = std::bind(&SceneTitle::LoadingDraw, this);

	state_ = STATE::NONE;
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Load()
{
	SceneBase::Load();
}

void SceneTitle::Init()
{
	// 基底クラスの初期化
	SceneBase::Init();

	// 状態別処理の生成
	stateMap_.emplace(STATE::MAIN, std::make_unique<TitleStateMain>(*this));
	stateMap_.emplace(STATE::EXPLANATION, std::make_unique<TitleStateExplanation>(*this));
	stateMap_.emplace(STATE::SELECT, std::make_unique<TitleStateSelect>(*this));

	// 各種初期化処理
	for (const auto& it : stateMap_)
	{
		it.second->Init();
	}

	// 初期状態
	state_ = STATE::MAIN;
}

void SceneTitle::ChangeState(const STATE state)
{
	state_ = state;
}

void SceneTitle::NormalUpdate()
{	
	stateMap_[state_]->Update();
}

void SceneTitle::NormalDraw()
{
	stateMap_[state_]->Draw();
}

void SceneTitle::ChangeNormal()
{
	//処理変更
	updataFunc_ = std::bind(&SceneTitle::NormalUpdate, this);
	drawFunc_ = std::bind(&SceneTitle::NormalDraw, this);

	//フェードイン開始
	scnMng_.StartFadeIn();
}