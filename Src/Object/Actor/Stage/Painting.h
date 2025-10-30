#pragma once
#include "StageGimmickObjectBase.h"

class Painting : public StageGimmickObjectBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Painting(const std::string& key, const Json& mapParam, const Json& colliderParam);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Painting() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 異変後の後処理
	/// </summary>
	void Refresh() override;

private:
};

