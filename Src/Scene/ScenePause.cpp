#include <DxLib.h>
#include "../Application.h"
#include "../Manager/Common/ResourceManager.h"
#include "../Manager/Common/FontManager.h"
#include "../Manager/Common/SceneManager.h"
#include "../Manager/Common/Camera.h"
#include "../Manager/Common/SoundManager.h"
#include "../Manager/Common/InputManager.h"
#include "../Utility/UtilityCommon.h"
#include "ScenePause.h"

ScenePause::ScenePause()
{
	//更新関数のセット
	updataFunc_ = std::bind(&ScenePause::NormalUpdate, this);

	//描画関数のセット
	drawFunc_ = std::bind(&ScenePause::NormalDraw, this);

	// 初期化
	pauseFont_ = -1;
	selectIndex_ = 0;
	
	//リストごとに処理を分ける
	listFuncTable_ =
	{
		{LIST::RESUME,[this]()
		{
			// カメラを戻す
			mainCamera.ChangeMode(Camera::MODE::FPS);

			// マウス非表示
			SetMouseDispFlag(false);

			// ポーズを解除して前のシーンに戻る
			scnMng_.PopScene();
		}},
		{LIST::TITLE,[this]()
		{
			// タイトルシーンに戻る
			scnMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
		}},
		{LIST::GAME_END,[this]()
		{
			// ゲーム終了
			Application::GetInstance().GameEnd();
		}}
	};

	pauseFont_ = fontMng_.CreateMyFont(resMng_.GetFontName("fontKazuki"), FONT_SIZE, FONT_THICK);
}

ScenePause::~ScenePause()
{
}

void ScenePause::NormalUpdate()
{

	if (inputMng_.IsTrgDown(InputManager::TYPE::PAUSE) || inputMng_.IsTrgDown(InputManager::TYPE::SELECT_CANCEL))
	{
		// カメラを戻す
		mainCamera.ChangeMode(Camera::MODE::FPS);

		// マウス非表示
		SetMouseDispFlag(false);

		//シーンを戻す
		scnMng_.PopScene();

		// 効果音再生
		sndMng_.PlaySe(SoundType::SE::DECISION);
		return;
	}
	else if (inputMng_.IsTrgDown(InputManager::TYPE::SELECT_DOWN))
	{
		selectIndex_ = UtilityCommon::WrapStepIndex(selectIndex_, 1, 0, LIST_MAX);

		// 効果音再生
		sndMng_.PlaySe(SoundType::SE::SELECT);
		return;
	}
	else if (inputMng_.IsTrgDown(InputManager::TYPE::SELECT_UP))
	{
		selectIndex_ = UtilityCommon::WrapStepIndex(selectIndex_, -1, 0, LIST_MAX);

		// 効果音再生
		sndMng_.PlaySe(SoundType::SE::SELECT);
		return;
	}
	else if (inputMng_.IsTrgDown(InputManager::TYPE::SELECT_DECISION))
	{
		listFuncTable_[static_cast<LIST>(selectIndex_)]();

		// 効果音再生
		sndMng_.PlaySe(SoundType::SE::DECISION);
		return;
	}
}

void ScenePause::NormalDraw()
{
	static constexpr int OFFSET_Y = 200;
	static constexpr int TEXT_MARGINT = 150;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, PAUSE_ALPHA);
	DrawBox(
		0,
		0,
		Application::SCREEN_SIZE_X,
		Application::SCREEN_SIZE_Y,
		UtilityCommon::BLACK,
		true);	
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	for (int i = 0; i < LIST_MAX; ++i)
	{
		//カラーを設定
		int color = UtilityCommon::WHITE;
		if (i == selectIndex_)
		{
			color = UtilityCommon::RED; // 選択中は赤色
		}

		//座標位置を設定
		int posX = static_cast<int>(Application::SCREEN_HALF_X - pasueList_[i].length() * FONT_SIZE / 2);
		int posY = Application::SCREEN_HALF_Y - OFFSET_Y + TEXT_MARGINT * i;

		//文字列を描画
		DrawFormatStringToHandle(
			posX,
			posY,
			color,
			pauseFont_,
			pasueList_[i].c_str());
	}
}