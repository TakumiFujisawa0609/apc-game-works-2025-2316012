#pragma once
#include "StageObjectBase.h"

class ModelMaterial;
class ModelRenderer;

class StageMain : public StageObjectBase
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
	/// オブジェクトがカメラ内か判定
	/// </summary>
	/// <returns>trueの場合カメラ内, そうでない場合flase</returns>
	bool CheckCameraViewClip();

private:

	// マテリアル
	std::unique_ptr<ModelMaterial> material_;

	// レンダラー
	std::unique_ptr<ModelRenderer> renderer_;

	// 血の手用マテリアル
	std::unique_ptr<ModelMaterial> bloodyMaterial_;

	// 血の手用レンダラー
	std::unique_ptr<ModelRenderer> bloodyRenderer_;

	// メイン描画
	void DrawMain() override;
};

