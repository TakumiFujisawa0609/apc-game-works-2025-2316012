#include <DxLib.h>
#include <string>
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Resource/FontManager.h"
#include "../Manager/Resource/SoundManager.h"
#include "../Core/PostEffect/PostEffectSnowNoise.h"
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
	loadingScreen_ = -1;
	snowNoiseEffect_ = nullptr;
}

SceneBase::~SceneBase(void)
{
	DeleteGraph(loadingScreen_);
}

void SceneBase::Load(void)
{
	// 読み込み時間初期化
	loadingTime_ = 0.0f;	
	
	// 非同期読み込み開始
	//SetUseASyncLoadFlag(true);
	
	// ポストエフェクトの生成
	snowNoiseEffect_ = std::make_unique<PostEffectSnowNoise>();
	snowNoiseEffect_->Load();

	// ポストエフェクトスクリーン
	loadingScreen_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);
	
	// シーン内のリソースを読み込む
	resMng_.SceneChangeResource(static_cast<int>(scnMng_.GetSceneID()));

	// サウンドのリソースの切り替え
	sndMng_.SceneChangeResources();

	//ローディング用文字列設定
	const std::wstring& fontName = resMng_.GetFontName("fontKazuki");
	loadingString_.fontHandle = fontMng_.CreateMyFont(fontName, 32, 0);
	loadingString_.color = UtilityCommon::WHITE;
	loadingString_.pos = { LOADING_STRING_POS_X, LOADING_STRING_POS_Y };
	loadingString_.string = L"Now loading";
}

void SceneBase::Init(void)
{
	// ポストエフェクト初期化
	snowNoiseEffect_->Init();
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
	//スクリーンの設定
	SetDrawScreen(loadingScreen_);

	// 画面を初期化
	ClearDrawScreen();

	// ポストエフェクトの描画
	snowNoiseEffect_->Draw();

	// メインに戻す
	SetDrawScreen(scnMng_.GetMainScreen());

	// 描画
	DrawGraph(0, 0, loadingScreen_, false);

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