#pragma once
#include <DxLib.h>
#include <nlohmann/json.hpp>

// JSON名前空間
using json = nlohmann::json;

class ParameterBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ParameterBase();
	 
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ParameterBase() = default;

	/// <summary>
	/// データの格納
	/// </summary>
	/// <param name="file">パラメータファイル</param>
	virtual void SetDataFile(const json& file);
	
	// 移動速度
	float moveSpeed_;

	// ダッシュ速度
	float dashSpeed_;

	// 重力
	float gravity_;

	// 回転完了までの時間
	float timeRot_;

	// 初期座標
	VECTOR initialPosition_;

	// デフォルトのアニメーション速度
	float animationDefaultSpeed_;
};