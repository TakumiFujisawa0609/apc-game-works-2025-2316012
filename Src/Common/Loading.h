#pragma once
#include <unordered_map>
#include <nlohmann/json.hpp>
#include "../Common/Vector2.h"
#include "../Template/Singleton.h"
#include "../Common/CharacterString.h"

class PostEffectSnowNoise;

// JSON名前空間
using Json = nlohmann::json;

class Loading : public Singleton<Loading>
{
	//シングルトンにだけ共有する
	friend class Singleton<Loading>;

public:
	
	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 非同期読み込みの開始
	/// </summary>
	void StartASyncLoad();

	/// <summary>
	/// 非同期読み込み中か判定
	/// </summary>
	/// <returns>trueの場合非同期処理中、falseの場合終了済み</returns>
	const bool IsLoading() const { return isLoading_; }

private:

	// ファイル名
	const std::string FILE_NAME = "Loading";

	// ポストエフェクト用情報
	Json POST_EFFECT_PARAM;

	// 非同期中表示メッセージ
	std::wstring TEXT;

	// コンマ
	std::wstring COMMA;

	// 最低ローディング時間
	float LOADING_TIME;

	// フォント名
	std::string FONT_NAME;

	//フォントサイズ
	int FONT_SIZE;

	// コンマ表示数
	int COMMA_MAX_NUM;	

	// コンマ表示時間
	float COMMA_TIME;

	// テキスト位置
	Vector2 TEXT_POS;

	// ローディング中ポストエフェクト
	std::unique_ptr<PostEffectSnowNoise> snowNoiseEffect_;

	// ローディング用スクリーン
	int loadingScreen_;

	// ローディング経過時間
	float loadingTime_;

	// ローディング中かどうか
	bool isLoading_;

	//ローディング文字列
	CharacterString loadingString_;

	//「now loading......」の描画
	void DrawNowLoading(void);

	// コンストラクタ
	Loading();

	// デストラクタ
	~Loading();
};

