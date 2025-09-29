#pragma once

class StageManager;

class AnomalyBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	AnomalyBase();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~AnomalyBase();

	/// <summary>
	/// 読み込み処理
	/// </summary>
	virtual void Load();

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

private:

	// ステージ管理クラス
	StageManager& stageMng_;;
};

