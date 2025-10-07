#pragma once

class ReportSystem;

class ReportStateBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="parent">親インスタンス</param>
	ReportStateBase(ReportSystem& parent);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ReportStateBase();

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

	// 親の参照インスタンス
	ReportSystem& parent_;

};