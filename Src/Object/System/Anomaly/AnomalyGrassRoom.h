#pragma once
#include "AnomalyBase.h"
class AnomalyGrassRoom : public AnomalyBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	AnomalyGrassRoom();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~AnomalyGrassRoom();

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load() override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init() override;

	/// <summary>
	/// 発生処理
	/// </summary>
	/// <param name="param">パラメータ</param>
	void Occurrence(Json& param) override;

private:

	// 異変を起こすオブジェクトのキー
	const std::string KEY = "RoomD";
};

