#pragma once
#include "StageGimmickObjectBase.h"

class ModelMaterial;
class ModelRenderer;
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
	~StageMain() override;

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load() override;
	
	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;

	/// <summary>
	/// 異変の設定
	/// </summary>
	void SetAnomaly() override;

	/// <summary>
	/// 異変後の後処理
	/// </summary>
	void Refresh() override;

	/// <summary>
	/// オブジェクトがカメラ内か判定
	/// </summary>
	/// <returns>trueの場合カメラ内, そうでない場合flase</returns>
	bool CheckCameraViewClip();

private:

	// マテリアル
	std::unique_ptr<ModelMaterial> material_;

	// レンダラー
	std::unique_ptr<ModelRenderer> renderer_;

	// コライダーモデル
	std::shared_ptr<ColliderModel> colliderModel_;

	// メイン描画
	void DrawMain() override;
};

