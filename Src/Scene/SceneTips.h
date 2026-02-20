#pragma once
#include <functional>
#include <unordered_map>
#include "SceneBase.h"
#include "../Common/Image.h"
#include "../Common/CharacterString.h"

class PixelMaterial;
class PixelRenderer;
class SceneBackGround;

class SceneTips : public SceneBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	SceneTips();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SceneTips() override;

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load()override {};

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init()override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void NormalUpdate()override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void NormalDraw()override;

private:	

	// 切り替えアイコンの数
	static constexpr int CHANGE_ICON_MAX = 2;

	// ヒントの総数
	static constexpr int TIPS_COUNT = 7;

	// リソースネーム
	const std::string RESOURCE_NAMES[TIPS_COUNT] =
	{
		"tips1",
		"tips2",
		"tips3",
		"tips4",
		"tips5",
		"tips6",
		"tips7",
	};

	// ヒントメッセージ
	const std::wstring TIPS_MESSAGES[TIPS_COUNT] =
	{
		L"かわいいゴーストさん\n意外と色んなところにいる",
		L"青っぽい絵画に注意\n実は結構発生しやすい",
		L"椅子がきれいに並んで......\nこんな崩れてたかな",
		L"きゃー！！！壁に血が...ってでっか",
		L"草生えてて草なんだが",
		L"世界が変わった、でも焦らずに調べてみるのだ",
		L"うわ誰だお前ら",
	};

	// テキスト配置関連
	static constexpr int TIPS_TITLE_POS_Y = 100;
	static constexpr int TIPS_MESSAGE_POS_Y = 600;
	static constexpr int TIPS_UI_POS_Y = 696;
	static constexpr int TIPS_PAGE_POS_X = 1200;

	// アイコン・画像関連
	static constexpr int TIPS_CHANGE_ICON_OFFSET_X = 500; 
	static constexpr int TIPS_CHANGE_ICON_LEFT_X = 140;
	static constexpr int TIPS_CHANGE_ICON_RIGHT_X = 1140;

	// 説明画像関連
	static constexpr int TIPS_IMAGE_POS_X = 320;
	static constexpr int TIPS_IMAGE_POS_Y = 180;
	static constexpr int TIPS_IMAGE_WIDTH = 640;
	static constexpr int TIPS_IMAGE_HEIGHT = 360;

	// 回転角度
	static constexpr float ANGLE_FLIP_DEG = 180.0f;

	// シェーダー定数バッファ用
	static constexpr float TIPS_SHADER_CENTER_X = 640.0f;
	static constexpr float TIPS_SHADER_CENTER_Y = 360.0f;

	// フォントサイズ関連
	static constexpr int FONT_SIZE_TITLE = 52;
	static constexpr int FONT_SIZE_MESSAGE = 28;
	static constexpr int FONT_SIZE_OPERATION = 20;

	// フォント太さ関連
	static constexpr int FONT_THICK_TITLE = 6;
	static constexpr int FONT_THICK_COMMON = 4;

	// 画面の操作方法
	const std::wstring OPERATION_MESSAGE_KEY = L"A・Dで選択/Spaceで決定/BackSpaceで戻る";
	const std::wstring OPERATION_MESSAGE_PAD = L"左スティックで選択/Bで決定/Aで戻る";

	// タイトルメッセージ
	const std::wstring TITLE_MESSAGE = L"異変探しのヒント";

	// 現在のページ数
	int pageCount_;

	// 見出し
	CharacterString titleText_;

	// 吹き出しメッセージ
	CharacterString messageText_;

	// ページ数
	CharacterString pageText_;

	// 画面の操作方法
	CharacterString screenOperationMessage_;	
	
	// 切り替えアイコン
	Image changeIcon_[CHANGE_ICON_MAX];

	// ヒント画像
	std::unique_ptr<PixelMaterial> tipsMaterial_;
	std::unique_ptr<PixelRenderer> tipsRenderer_;

	// 背景画像
	std::unique_ptr<SceneBackGround> backGround_;

	// ページの更新
	void UpdatePage();

	// 画面の操作方法の切り替え処理
	const std::wstring GetOperationMessage() const;
};