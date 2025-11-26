#include <vector>
#include <random>
#include <chrono>
#include "../../../Application.h"
#include "../../../Manager/Common/SceneManager.h"
#include "../../../Manager/Common/InputManager.h"
#include "../../../Manager/Common/ResourceManager.h"
#include "../../../Manager/Common/SoundManager.h"
#include "../../../Manager/Common/FontManager.h"
#include "../../../Utility/UtilityCommon.h"
#include "../../../Core/PostEffect/PostEffectTitleSelect.h"
#include "TitleStateSelect.h"

// 乱数生成器
std::mt19937 mt(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));

TitleStateSelect::TitleStateSelect(SceneTitle& parent) :
	TitleStateBase(parent)
{
	// 初期時の更新処理と描画処理
	update_ = std::bind(&TitleStateSelect::UpdateSelect, this);
	draw_ = std::bind(&TitleStateSelect::DrawSelect, this);

	// 状態遷移の登録
	changeMap_.emplace(TYPE::START, std::bind(&TitleStateSelect::ChangeStart, this));
	changeMap_.emplace(TYPE::END, std::bind(&TitleStateSelect::ChangeEnd, this));
	effectScreen_ = -1;
	afterStep_ = 0.0f;
	type_ = -1;
	effect_ = nullptr;
}

TitleStateSelect::~TitleStateSelect()
{
	DeleteGraph(effectScreen_);
}

void TitleStateSelect::Init()
{
	// フォント
	int font = fontMng_.CreateMyFont(resMng_.GetFontName("fontKazuki"), 52, 0);

	// テキストの設定
	selectTexts_[0].pos = { Application::SCREEN_HALF_X, Application::SCREEN_SIZE_Y / 3 * 1 };
	selectTexts_[0].color = UtilityCommon::WHITE;
	selectTexts_[0].fontHandle = font;
	selectTexts_[0].string = L"開始する";

	selectTexts_[1].pos = { Application::SCREEN_HALF_X, Application::SCREEN_SIZE_Y / 3 * 2 };
	selectTexts_[1].color = UtilityCommon::WHITE;
	selectTexts_[1].fontHandle = font;
	selectTexts_[1].string = L"終了する";

	selectBack_.handleId = resMng_.GetHandle("selectBack");
	selectBack_.pos = selectTexts_[0].pos;
	selectBack_.size.x = selectTexts_[0].string.length() * 52;
	selectBack_.size.y = 60;

	// 選択後のテキスト設定
	afterText_.pos = { Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y };
	afterText_.color = UtilityCommon::BLACK;
	afterText_.fontHandle = font;

	// エフェクトの作成
	effect_ = std::make_unique<PostEffectTitleSelect>();
	effect_->Init();

	// スクリーンの生成
	effectScreen_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);

	// 初期選択項目
	type_ = 0;
}

void TitleStateSelect::Update()
{
	// 更新処理
	update_();
}

void TitleStateSelect::Draw()
{
	// 背景
	DrawBox(
		0,
		0,
		Application::SCREEN_SIZE_X,
		Application::SCREEN_SIZE_Y,
		UtilityCommon::RED,
		true
	);

	// 描画処理
	draw_();

	GlitchEffect();

	////スクリーンの設定
	//SetDrawScreen(effectScreen_);

	//// 画面を初期化
	//ClearDrawScreen();

	//// ポストエフェクト描画
	//effect_->Draw();

	//// メインに戻す
	//SetDrawScreen(scnMng_.GetMainScreen());

	//// 描画
	//DrawGraph(0, 0, effectScreen_, false);

	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void TitleStateSelect::ChangeStart()
{
	// 処理の切り替え
	update_ = std::bind(&TitleStateSelect::UpdateStart, this);
	draw_ = std::bind(&TitleStateSelect::DrawAfter, this);

	// テキストの設定
	afterText_.string = L"仕事を開始します";
}

void TitleStateSelect::ChangeEnd()
{
	// 処理の切り替え
	update_ = std::bind(&TitleStateSelect::UpdateEnd, this);
	draw_ = std::bind(&TitleStateSelect::DrawAfter, this);

	// テキストの設定
	afterText_.string = L"システムを終了します";
}

void TitleStateSelect::UpdateSelect()
{
	if (inputMng_.IsTrgDown(InputManager::TYPE::SELECT_UP))
	{
		// 増加
		type_ = UtilityCommon::WrapStepIndex(type_, 1, 0, TYPE_MAX);
		sndMng_.PlaySe(SoundType::SE::SELECT);
	}
	else if (inputMng_.IsTrgDown(InputManager::TYPE::SELECT_DOWN))
	{
		// 減少
		type_ = UtilityCommon::WrapStepIndex(type_, -1, 0, TYPE_MAX);
		sndMng_.PlaySe(SoundType::SE::SELECT);
	}
	else if (inputMng_.IsTrgDown(InputManager::TYPE::SELECT_DECISION))
	{
		// 状態遷移
		changeMap_[static_cast<TYPE>(type_)]();
		sndMng_.PlaySe(SoundType::SE::DECISION);
	}

	selectBack_.pos = selectTexts_[type_].pos;
	selectBack_.size.x = selectTexts_[type_].string.length() * 52;
	selectBack_.size.y = 60;
}

void TitleStateSelect::UpdateStart()
{
	afterStep_ += scnMng_.GetDeltaTime();

	if (afterStep_ > 1.0f)
	{
		// シーン遷移
		scnMng_.ChangeScene(SceneManager::SCENE_ID::GAME, Fader::STATE::FINISH);

		// 更新処理変更
		update_ = std::bind(&TitleStateSelect::UpdateNone, this);
	}
}

void TitleStateSelect::UpdateEnd()
{
	afterStep_ += scnMng_.GetDeltaTime();

	if (afterStep_ > 1.0f)
	{
		// 終了処理
		Application::GetInstance().GameEnd();

		// 更新処理変更
		update_ = std::bind(&TitleStateSelect::UpdateNone, this);

		// SE停止
		sndMng_.StopAllSe();

		// BGM停止
		sndMng_.PlayBgm(SoundType::BGM::TITLE_SELECT);
	}
}

void TitleStateSelect::DrawSelect()
{
	selectBack_.DrawRota();
	for (int i = 0; i < TYPE_MAX; i++)
	{

		// 描画
		selectTexts_[i].DrawCenter();
	}
}

void TitleStateSelect::DrawAfter()
{
	afterText_.DrawCenter();
}

void TitleStateSelect::GlitchEffect()
{
	// グリッチエフェクトのパラメータ
	constexpr int GLITCH_LINE_HEIGHT_MIN = 5;  // グリッチのブロックの最小高さ
	constexpr int GLITCH_LINE_HEIGHT_MAX = 30; // グリッチのブロックの最大高さ
	constexpr int GLITCH_OFFSET_MAX = 100;     // 水平方向の最大ズレ量
	constexpr int GLITCH_DENSITY = 3;          // グリッチ発生の頻度 (小さいほど頻繁)
	constexpr int NOISE_PIXEL_COUNT = 500;     // ランダムなピクセルノイズの数
	constexpr int GLITCH_COLOR_MAGNITUDE = 100; // グリッチで発生する色の変化の大きさ

	// メインスクリーンの取得
	int mainScreen = scnMng_.GetMainScreen();

	// 現在の描画先を保存
	int originalTargetScreen = GetDrawScreen();

	// 一時的な画面を作成し、元の画面の内容をコピーする
	// これにより、元の描画内容に対してグリッチを適用できる
	int tempScreenHandle = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, TRUE);
	if (tempScreenHandle == -1) return;

	SetDrawScreen(tempScreenHandle);
	DrawGraph(0, 0, mainScreen, FALSE); // 元の画面の内容をコピー

	// 描画先を元の画面に戻す
	SetDrawScreen(mainScreen);

	// 描画モードを通常に戻す（グリッチ描画前に設定が必要な場合がある）
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	// 画面全体を一旦、元の赤色で塗りつぶす (ズレた部分の背景として)
	// これにより、ズレた部分が真っ黒にならず、元の赤色が背景になる
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, GetColor(255, 0, 0), TRUE);

	// ------ グリッチ処理本体 ------

	// 1. 水平方向へのブロック状のズレ
	int y = 0;
	while (y < Application::SCREEN_SIZE_Y)
	{
		int lineHeight = GLITCH_LINE_HEIGHT_MIN + (mt() % (GLITCH_LINE_HEIGHT_MAX - GLITCH_LINE_HEIGHT_MIN + 1));
		if (y + lineHeight > Application::SCREEN_SIZE_Y) {
			lineHeight = Application::SCREEN_SIZE_Y - y;
		}

		if (mt() % GLITCH_DENSITY == 0) // 一定の確率でグリッチを発生させる
		{
			int offsetX = (mt() % (GLITCH_OFFSET_MAX * 2 + 1)) - GLITCH_OFFSET_MAX; // -GLITCH_OFFSET_MAX から +GLITCH_OFFSET_MAX

			// 描画元の矩形 (コピーした画面のy座標と高さ)
			int srcX = 0;
			int srcY = y;
			int srcW = Application::SCREEN_SIZE_X;
			int srcH = lineHeight;

			// 描画先の矩形 (offsetXだけずらす)
			int destX = offsetX;
			int destY = y;

			// コピーした一時画面の該当部分を、ずらして描画
			DrawRectGraph(destX, destY, srcX, srcY, srcW, srcH, tempScreenHandle, FALSE);

			// ズレた部分に、わずかに異なる色やノイズを重ねる (オプション)
			// 例: 青みがかったノイズを重ねて色の分離感を出す
			if (mt() % 2 == 0) { // 50%の確率でノイズを重ねる
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 64); // 半透明で重ねる
				int noiseColorR = 255 - (mt() % GLITCH_COLOR_MAGNITUDE);
				int noiseColorG = mt() % GLITCH_COLOR_MAGNITUDE;
				int noiseColorB = mt() % GLITCH_COLOR_MAGNITUDE;
				DrawBox(destX, destY, destX + srcW, destY + srcH, GetColor(noiseColorR, noiseColorG, noiseColorB), TRUE);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255); // 元に戻す
			}
		}
		else
		{
			// グリッチしない場合は、元の位置に描画
			DrawRectGraph(0, y, 0, y, Application::SCREEN_SIZE_X, lineHeight, tempScreenHandle, FALSE);
		}

		y += lineHeight;
	}

	// 2. ランダムなピクセルノイズや小さなブロックノイズを重ねる
	SetDrawBlendMode(DX_BLENDMODE_ADD, 128); // 加算ブレンドでノイズを強調
	for (int i = 0; i < NOISE_PIXEL_COUNT; ++i)
	{
		int px = mt() % Application::SCREEN_SIZE_X;
		int py = mt() % Application::SCREEN_SIZE_Y;
		int noiseSize = 1 + (mt() % 5); // ノイズのサイズをランダムに
		int noiseColorR = 100 + (mt() % 155);
		int noiseColorG = mt() % 200;
		int noiseColorB = mt() % 200;
		DrawBox(px, py, px + noiseSize, py + noiseSize, GetColor(noiseColorR, noiseColorG, noiseColorB), TRUE);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255); // 元に戻す

	// 一時画面を削除
	DeleteGraph(tempScreenHandle);

	// 描画先を元の画面に戻す (万が一関数呼び出し側で変更されていることを考慮)
	SetDrawScreen(originalTargetScreen);
}