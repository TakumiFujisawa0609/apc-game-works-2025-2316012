#pragma once
#include "AnomalyBase.h"
class AnomalyGrassRoom : public AnomalyBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="param">パラメータ</param>
	AnomalyGrassRoom(const Json& param);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~AnomalyGrassRoom();

	/// <summary>
	/// 発生処理
	/// </summary>
	void Occurrence() override;

private:

	// 音量
	static constexpr int VOLUME = 50;

	// 異変を起こすオブジェクトのキー
	const std::string KEY = "RoomD";
};