#pragma once
#include <memory>
#include "SceneBase.h"

class ScenePause;

class SceneGame : public SceneBase
{

public:
	
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name=""></param>
	SceneGame(void);

	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	~SceneGame(void);

	/// <summary>
	/// 読み込み処理
	/// </summary>
	/// <param name=""></param>
	void Load(void) override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name=""></param>
	void Init(void) override;

private:

	//ポーズ画面
	std::shared_ptr<ScenePause> ScenePause_;

	//更新関数
	void NormalUpdate(void) override;

	//描画関数
	void NormalDraw(void) override;

	//処理の変更
	void ChangeNormal(void) override;
	
	//デバッグ処理
	void DebugUpdate(void);
	void DebugDraw(void);

};