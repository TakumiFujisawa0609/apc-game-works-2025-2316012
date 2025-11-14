#include <DxLib.h>
#include "../Application.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Resource/ResourceManager.h"
#include "../Manager/Resource/SoundManager.h"
#include "../Manager/Resource/FontManager.h"
#include "../Utility/UtilityCommon.h"
#include "../Core/PostEffect/PostEffectSnowNoise.h"
#include "Loading.h"

void Loading::Init()
{
	// ポストエフェクトの生成
	snowNoiseEffect_ = std::make_unique<PostEffectSnowNoise>();
	snowNoiseEffect_->Load();

	// ポストエフェクトスクリーン
	loadingScreen_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);

	// シーン内のリソースを読み込む
	ResourceManager::GetInstance().SceneChangeResource(static_cast<int>(SceneManager::GetInstance().GetSceneID()));

	// サウンドのリソースの切り替え
	SoundManager::GetInstance().SceneChangeResources();

	// 効果音の再生
	SoundManager::GetInstance().PlaySe(SoundType::SE::TV_NOISE_SNOW, true, 50);

	//ローディング用文字列設定
	const std::wstring& fontName = ResourceManager::GetInstance().GetFontName("fontKazuki");
	loadingString_.fontHandle = FontManager::GetInstance().CreateMyFont(fontName, 32, 0);
	loadingString_.color = UtilityCommon::RED;
	loadingString_.pos = { LOADING_STRING_POS_X, LOADING_STRING_POS_Y };
	loadingString_.string = L"Now loading";
}

void Loading::Update()
{
	// ローディング経過時間更新
	bool loadTimeOver = UtilityCommon::IsTimeOver(loadingTime_, LOADING_TIME);

	//ロードが完了したか判断
	if (GetASyncLoadNum() == 0 && loadTimeOver)
	{
		//非同期処理を無効にする
		SetUseASyncLoadFlag(false);		
		
		// 非同期読み込みの終了判定
		isLoading_ = false;

		// 効果音の停止
		SoundManager::GetInstance().StopSe(SoundType::SE::TV_NOISE_SNOW);
	}
}

void Loading::Draw()
{
	// スクリーンの設定
	SetDrawScreen(loadingScreen_);

	// 画面を初期化
	ClearDrawScreen();

	// ポストエフェクトの描画
	snowNoiseEffect_->Draw();

	// メインに戻す
	SetDrawScreen(SceneManager::GetInstance().GetMainScreen());

	// 描画
	DrawGraph(0, 0, loadingScreen_, false);

	// NowLoadingの描画
	DrawNowLoading();
}

void Loading::StartASyncLoad() 
{
	// ローディング中フラグを立てる
	isLoading_ = true;

	// 非同期読み込みを有効にする
	SetUseASyncLoadFlag(true);

	// ローディング時間の初期化
	loadingTime_ = 0.0f;
}

void Loading::DrawNowLoading(void)
{
	//ロード中
	auto time = SceneManager::GetInstance().GetTotalTime();
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

Loading::Loading()
{
	loadingTime_ = 0.0f;
	loadingScreen_ = -1;
	isLoading_ = false;
}

Loading::~Loading()
{
	DeleteGraph(loadingScreen_);
}