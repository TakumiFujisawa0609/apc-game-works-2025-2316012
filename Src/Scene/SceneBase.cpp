#include <DxLib.h>
#include <string>
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Resource/FontManager.h"
#include "../Manager/Resource/SoundManager.h"
#include "../Utility/UtilityCommon.h"
#include "SceneBase.h"

SceneBase::SceneBase(void) :
	resMng_(ResourceManager::GetInstance()),
	scnMng_(SceneManager::GetInstance()),
	inputMng_(InputManager::GetInstance()),
	fontMng_(FontManager::GetInstance()),
	sndMng_(SoundManager::GetInstance())
{
	loadingTime_ = -1;
}

SceneBase::~SceneBase(void)
{
}

void SceneBase::Load(void)
{
	//ローディング用文字列設定
	loadingString_.fontHandle = fontMng_.CreateMyFont(resMng_.GetFontName("fontKazuki"), 32, 0);
	loadingString_.color = UtilityCommon::WHITE;
	loadingString_.pos = { LOADING_STRING_POS_X, LOADING_STRING_POS_Y };
	loadingString_.string = L"Now loading";
}

void SceneBase::Init(void)
{
}

void SceneBase::Update(void)
{
	updataFunc_();
	return;
}

void SceneBase::Draw(void)
{
	drawFunc_();
	return;
}

void SceneBase::LoadingUpdate(void)
{
	bool loadTimeOver = UtilityCommon::IsTimeOver(loadingTime_, LOADING_TIME);

	//ロードが完了したか判断
	if (GetASyncLoadNum() == 0 && loadTimeOver)
	{
		//非同期処理を無効にする
		SetUseASyncLoadFlag(false);

		//初期化処理
		Init();

		//フェードイン開始
		scnMng_.StartFadeIn();

		//通常の処理へ移る
		ChangeNormal();
	}
}

void SceneBase::NormalUpdate(void)
{
}

void SceneBase::LoadingDraw(void)
{
	//NowLoadingの描画
	DrawNowLoading();
}

void SceneBase::NormalDraw(void)
{
}

void SceneBase::ChangeNormal(void)
{
}

void SceneBase::DrawNowLoading(void)
{
	//ロード中
	auto time = scnMng_.GetTotalTime();
	int count = static_cast<int>(time / COMMA_TIME);
	count %= COMMA_MAX_NUM;

	loadingString_.string = L"Now Loading";
	std::wstring dotStr = L".";

	//テキストの設定
	for (int i = 0; i < count; i++)
	{
		loadingString_.string += dotStr;
	}

	//文字の描画
	loadingString_.Draw();

}