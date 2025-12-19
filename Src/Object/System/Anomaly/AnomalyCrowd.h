#pragma once
#include "AnomalyBase.h"
class AnomalyCrowd : public AnomalyBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	AnomalyCrowd(const Json& param);
	 
	/// <summary>
	/// デストラクタ
	/// </summary>
	~AnomalyCrowd() override;
	 
	/// <summary>
	/// 発生処理
	/// </summary>
	/// <param name="param">パラメータ</param>
	void Occurrence() override;

private:

	// マップ情報
	const Json& MAP_PARAM;

	// 生成するステージネーム
	const std::string KEY = "CrowdRoom";
};

