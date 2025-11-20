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
	ControllerDrawGrassRoom(const int model, GrassRoom& owner);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ControllerDrawGrassRoom();

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

private:

	// 親クラスの参照
	GrassRoom& owner_;

	// 異変開始位置
	VECTOR startPos_;

	// バッファーの更新
	void UpdateBuffer() override;
};

