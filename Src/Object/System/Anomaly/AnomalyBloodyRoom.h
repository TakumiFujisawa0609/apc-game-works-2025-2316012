#pragma once
#include <array>
#include "AnomalyBase.h"

class AnomalyBloodyRoom : public AnomalyBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	AnomalyBloodyRoom(const Json& param);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~AnomalyBloodyRoom();

	/// <summary>
	/// 発生処理
	/// </summary>
	/// <param name="param">パラメータ</param>
	void Occurrence() override;

private:

	// 異変を起こすオブジェクト数
	static constexpr int OBJS = 2;

	// 異変を起こすオブジェクトのキー
	const std::string KEYS[OBJS] = {"RoomB", "RoomC"};

};