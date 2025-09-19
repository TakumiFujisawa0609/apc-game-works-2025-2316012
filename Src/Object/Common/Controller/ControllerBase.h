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

protected:

	//シーン管理クラス
	SceneManager& scnMng_;
};

