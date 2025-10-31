#pragma once
#include <array>
#include "AnomalyBase.h"

class AnomalyBloodyRoom : public AnomalyBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	AnomalyBloodyRoom();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~AnomalyBloodyRoom();

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

	// 異変を起こすオブジェクト数
	static constexpr int OBJS = 2;

	// 異変を起こすオブジェクトのキー
	const std::string KEYS[OBJS] = {"RoomB", "RoomC"};

};

