#pragma once
#include "StageObjectBase.h"

class StageGimicObjectBase : public StageObjectBase
{
public:


	StageGimicObjectBase();

	~StageGimicObjectBase();

	/// <summary>
	/// 色を変更
	/// </summary>
	/// <param name="red">赤値</param>
	/// <param name="green">緑値</param>
	/// <param name="blue">青値</param>
	void SetChangeColor(const int red, const int green, const int blue);

protected:


};

