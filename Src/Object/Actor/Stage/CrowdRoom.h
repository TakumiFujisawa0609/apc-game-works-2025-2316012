#pragma once
#include "StageMain.h"
class CrowdRoom : public StageMain
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="key">オブジェクトの名前</param>
	/// <param name="mapParam">配置情報</param>
	/// <param name="colliderParam">コライダー情報</param>
	CrowdRoom(const std::string& key, const Json& mapParam, const Json& colliderParam);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~CrowdRoom() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 異変の設定
	/// </summary>
	void SetAnomaly() override;

	/// <summary>
	/// 異変後の後処理
	/// </summary>
	void Refresh() override;
};

