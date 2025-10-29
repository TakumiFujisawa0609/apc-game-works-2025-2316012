#pragma once
#include <memory>
#include "../CoreBase.h"

class PixelMaterial;
class PixelRenderer;

class CoreTitleBase : public CoreBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CoreTitleBase();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~CoreTitleBase() override;

protected:

	std::unique_ptr<PixelMaterial> material_;
	std::unique_ptr<PixelRenderer> renderer_;

};

