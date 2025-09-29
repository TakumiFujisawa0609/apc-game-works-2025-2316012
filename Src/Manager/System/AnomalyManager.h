#pragma once
#include <unordered_map>
#include "../../Template/Singleton.h"

class AnomalyBase;

class AnomalyManager : public Singleton<AnomalyManager>
{
	//シングルトンにだけ共有する
	friend class Singleton<AnomalyManager>;

public:

	enum class TYPE
	{
		REPLACE,		// 置き換え
		OCCUR,			// 何かが起こる
		COLOR_CHANGE,	// 色変え
		APPEARANCE,		// 出現
		MAX
	};
	
	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load();

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

	std::unordered_map<TYPE, std::unique_ptr<AnomalyBase>> anomalyMap_;

	//コンストラクタ
	AnomalyManager();

	//デストラクタ
	~AnomalyManager();
};

