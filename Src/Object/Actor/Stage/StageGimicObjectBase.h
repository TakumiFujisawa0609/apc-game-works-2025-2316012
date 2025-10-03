#pragma once
#include "StageObjectBase.h"

class StageGimicObjectBase : public StageObjectBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="key">ステージハンドル用名前キー</param>
	/// <param name="mapParam">配置情報</param>
	/// <param name="colliderParam">コライダー情報</param>
	StageGimicObjectBase(const std::string& key, const Json& mapParam, const Json& colliderParam);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~StageGimicObjectBase() override;

	/// <summary>
	/// 色を変更
	/// </summary>
	/// <param name="red">赤値</param>
	/// <param name="green">緑値</param>
	/// <param name="blue">青値</param>
	void SetChangeColor(const int red, const int green, const int blue);

protected:


};

