#pragma once
#include <functional>
#include <unordered_map>
#include "SceneBase.h"

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
	void Init()override {};

	/// <summary>
	/// 更新処理
	/// </summary>
	void NormalUpdate()override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void NormalDraw()override;

private:

};