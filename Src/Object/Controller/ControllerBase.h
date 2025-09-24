#pragma once

class SceneManager;

class ControllerBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ControllerBase();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ControllerBase();

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update();

protected:

	//シーン管理クラス
	SceneManager& scnMng_;
};

