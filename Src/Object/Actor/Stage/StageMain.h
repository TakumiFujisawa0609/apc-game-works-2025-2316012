#pragma once
#include "StageGimmickObjectBase.h"

class ColliderModel;

class StageMain : public StageGimmickObjectBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="key">オブジェクトの名前</param>
	/// <param name="mapParam">配置情報</param>
	/// <param name="colliderParam">コライダー情報</param>
	StageMain(const std::string& key, const Json& mapParam, const Json& colliderParam);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~StageMain() override;

	/// <summary>
	/// 異変の設定
	/// </summary>
	virtual void SetAnomaly() override;

	/// <summary>
	/// 異変後の後処理
	/// </summary>
	virtual void Refresh() override;

	/// <summary>
	/// オブジェクトがカメラ内か判定
	/// </summary>
	/// <returns>trueの場合カメラ内, そうでない場合flase</returns>
	bool CheckCameraViewClip();

protected:

	// コライダーモデル
	std::shared_ptr<ColliderModel> colliderModel_;
};

