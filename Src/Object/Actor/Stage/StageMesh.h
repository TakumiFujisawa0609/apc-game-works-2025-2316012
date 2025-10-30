#pragma once
#include "StageObjectBase.h" 

class StageMesh : public StageObjectBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="key">オブジェクトの名前</param>
	/// <param name="mapParam">配置情報</param>
	/// <param name="colliderParam">コライダー情報</param>
	StageMesh(const std::string& key, const Json& mapParam, const Json& colliderParam);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~StageMesh() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

private:



};

