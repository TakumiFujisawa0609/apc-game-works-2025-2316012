#include "ScenePause.h"
#include <DxLib.h>
#include "../Application.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Resource/FontManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Utility/UtilityCommon.h"

ScenePause::ScenePause(void)
{
	//更新関数のセット
	updataFunc_ = std::bind(&ScenePause::NormalUpdate, this);
	//描画関数のセット
	drawFunc_ = std::bind(&ScenePause::NormalDraw, this);

	imgTest_ = -1;
	pauseFont_ = -1;
	selectIndex_ = 0;
	
	//リストごとに処理を分ける
	listFuncTable_ =
	{
		{LIST::RESUME,[this]()
		{
			//ポーズを解除して前のシーンに戻る
			scnMng_.PopScene();
		}},
		{LIST::TITLE,[this]()
		{
			//タイトルシーンに戻る
			scnMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
		}}
	};
}

ScenePause::~ScenePause(void)
{
}

void ScenePause::Load(void)
{
	imgTest_ = resMng_.GetHandle("testImage");
	pauseFont_ = fontMng_.CreateMyFont(resMng_.GetFontName("fontHanazome"), FONT_SIZE, FONT_THICK);
}

void ScenePause::Init(void)
{	
	
}

void ScenePause::NormalUpdate(void)
{
	if (inputMng_.IsTrgDown(InputManager::TYPE::PAUSE) || inputMng_.IsTrgDown(InputManager::TYPE::SELECT_CANCEL))
	{
		//シーンを戻す
		scnMng_.PopScene();
		return;
	}
	else if (inputMng_.IsTrgDown(InputManager::TYPE::SELECT_DOWN))
	{
		selectIndex_ = UtilityCommon::WrapStepIndex(selectIndex_, -1, 0, LIST_MAX);
		return;
	}
	else if (inputMng_.IsTrgDown(InputManager::TYPE::SELECT_UP))
	{
		selectIndex_ = UtilityCommon::WrapStepIndex(selectIndex_, 1, 0, LIST_MAX);
		return;
	}
	else if (inputMng_.IsTrgDown(InputManager::TYPE::SELECT_DECISION))
	{
		listFuncTable_[static_cast<LIST>(selectIndex_)]();
		return;
	}
}

void ScenePause::NormalDraw(void)
{
	static constexpr int MARGINT = 50;
	static constexpr int OFFSET_Y = 200;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, PAUSE_ALPHA);
	DrawBox(
		MARGINT,
		MARGINT,
		Application::SCREEN_SIZE_X- MARGINT,
		Application::SCREEN_SIZE_Y- MARGINT,
		UtilityCommon::WHITE,
		true);	
	
	DrawRotaGraph(
		Application::SCREEN_HALF_X,
		Application::SCREEN_HALF_Y,
		1.0f,
		0.0f,
		imgTest_,
		true,
		false
	);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	for (int i = 0; i < LIST_MAX; ++i)
	{
		//カラーを設定
		int color = UtilityCommon::BLUE;
		if (i == selectIndex_)
		{
			color = UtilityCommon::RED; // 選択中は赤色
		}

		//座標位置を設定
		int posX = static_cast<int>(Application::SCREEN_HALF_X - pasueList_[i].length() * FONT_SIZE / 2);
		int posY = Application::SCREEN_HALF_Y - OFFSET_Y * i;

		//文字列を描画
		DrawFormatStringToHandle(
			posX,
			posY,
			color,
			pauseFont_,
			pasueList_[i].c_str());
	}
}