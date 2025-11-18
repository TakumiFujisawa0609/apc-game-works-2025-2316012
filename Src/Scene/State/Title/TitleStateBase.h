#pragma once
#include <memory>

class SceneTitle;
class SceneManager;
class InputManager;
class SoundManager;

class TitleStateBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	TitleStateBase(SceneTitle& parent);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~TitleStateBase();

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Init();

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw();

protected:

	// 親クラス
	SceneTitle& parent_;

	// 各種参照
	SceneManager& scnMng_;
	InputManager& inputMng_;
	SoundManager& sndMng_;
};

