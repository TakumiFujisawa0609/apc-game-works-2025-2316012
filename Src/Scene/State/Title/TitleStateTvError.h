#pragma once
#include "../../Common/Image.h"
#include "TitleStateBase.h"

class PostEffectTvError;
class Timer;

class TitleStateTvError : public TitleStateBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="parent">親インスタンス</param>
	TitleStateTvError(SceneTitle& parent);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TitleStateTvError() override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

private:

	// 時間
	static constexpr float TIME = 2.0f;

	// TVエラー画面
	int screen_;

	// シーン遷移フラグ
	bool isChangeScene_;

	// タイマー
	std::unique_ptr<Timer> timer_;

	// ポストエフェクト
	std::unique_ptr<PostEffectTvError> postEffectTvError_;
};

