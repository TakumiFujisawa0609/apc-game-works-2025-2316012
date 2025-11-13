#pragma once
#include <unordered_map>
#include "../../Template/Singleton.h"
#include "../Common/CharacterString.h"

class PostEffectSnowNoise;

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

private:

	//最低ローディング時間
	static constexpr float LOADING_TIME = 2.0f;

	//フォントサイズ
	static constexpr int FONT_SIZE = 28;

	//ローディング
	static constexpr int COMMA_MAX_NUM = 7;											//「now loading......」のコンマの数
	static constexpr float COMMA_TIME = 0.5f;										//「now loading......」のコンマ数を増やす時間
	static constexpr int LOADING_STRING_POS_X = Application::SCREEN_SIZE_X - 300;	//「now loading......」の座標X
	static constexpr int LOADING_STRING_POS_Y = Application::SCREEN_SIZE_Y - 40;	//「now loading......」の座標Y

	// ローディング中ポストエフェクト
	std::unique_ptr<PostEffectSnowNoise> snowNoiseEffect_;

	// ローディング用スクリーン
	int loadingScreen_;

	//ローディング経過時間
	float loadingTime_;

	//ローディング文字列
	CharacterString loadingString_;

	//「now loading......」の描画
	void DrawNowLoading(void);

	// コンストラクタ
	Loading();

	// デストラクタ
	~Loading();
};

