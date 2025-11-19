#pragma once
#include "ControllerDrawBase.h"

class GrassRoom;

class ControllerDrawGrassRoom : public ControllerDrawBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="model">モデルID</param>
	ControllerDrawGrassRoom(const int model, GrassRoom& parent);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ControllerDrawGrassRoom();

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load() override;

private:

	// 親クラスの参照
	GrassRoom& parent_;

	// 異変開始位置
	VECTOR startPos_;

	// 影響を及ぼす距離
	float distance_;

	// バッファーの更新
	void UpdateBuffer() override;
};

