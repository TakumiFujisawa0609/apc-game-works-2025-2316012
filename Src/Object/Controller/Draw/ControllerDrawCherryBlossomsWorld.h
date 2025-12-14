#pragma once
#include "ControllerDrawBase.h"
class ControllerDrawCherryBlossomsWorld : public ControllerDrawBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="model">モデルID</param>
	ControllerDrawCherryBlossomsWorld(const int model);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ControllerDrawCherryBlossomsWorld();

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load() override;

private:

	// バッファーサイズ
	static constexpr int BUFFER_PS_SIZE = 5;
	static constexpr int BUFFER_VS_SIZE = 2;

	// 桜の強調量
	static constexpr float BOOST_AMOUNT = 1.0f;

	// 桜の目標色相
	static constexpr float TARGET_HUE = 3.0f;

	// 環境光
	const VECTOR CHERRY_AMBIENT = { 0.5f, 0.5f, 0.5f };

	// エミシッブ強度
	static constexpr float EMMISIVE_INTENSITY = 1.0f;
	
	// バッファーの更新
	void UpdateBuffer() override;
};

