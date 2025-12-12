#pragma once
#include <memory>
#include "../CoreBase.h"
#include "../Common/Image.h"

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

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

	/// <summary>
	/// エフェクトの判定を設定
	/// </summary>
	/// <param name="isEffect">エフェクトの判定</param>
	void SetIsEffect(const bool isEffect) { isEffect_ = isEffect; }

protected:

	// 通常フォントサイズ
	static constexpr int FONT_SIZE = 24;

	// 通常フォント太さ
	static constexpr int FONT_THICK = 0;

	bool isEffect_;
	Image img_;
	std::unique_ptr<PixelMaterial> material_;
	std::unique_ptr<PixelRenderer> renderer_;

};

