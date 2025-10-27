#pragma once
#include "StageGimmickObjectBase.h"

class ChairMountain : public StageGimmickObjectBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ChairMountain(const std::string& key, const Json& mapParam, const Json& colliderParam);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ChairMountain() override;

	/// <summary>
	/// 異変後の後処理
	/// </summary>
	void Refresh() override;
};