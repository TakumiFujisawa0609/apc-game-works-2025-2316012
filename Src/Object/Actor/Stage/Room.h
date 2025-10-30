#pragma once
#include "StageObjectBase.h"

class ColliderBox;

class Room : public StageObjectBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="key">オブジェクトの名前</param>
	/// <param name="mapParam">配置情報</param>
	/// <param name="colliderParam">コライダー情報</param>
	Room(const std::string& key, const Json& mapParam, const Json& colliderParam);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Room() override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;

	/// <summary>
	/// オブジェクトがカメラ内か判定
	/// </summary>
	/// <returns>trueの場合カメラ内, そうでない場合flase</returns>
	bool CheckCameraViewClip();
};

