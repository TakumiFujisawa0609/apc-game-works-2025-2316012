#pragma once
#include <DxLib.h>
#include <memory>
#include "../../Common/Transform.h"
#include "../../Collider/ColliderBox.h"

class ColliderBox;

class TestModel
{
public:

	TestModel();
	~TestModel();

	void Load();
	void Init();
	void Update();
	void Draw();

	/// <summary>
	/// サイズの半分の設定
	/// </summary>
	/// <param name="halfSize">サイズの半分</param>
	void SetHalfSize(const VECTOR& halfSize);

private:

	Transform transform_;

	// 回転バウンディングボックス
	ColliderBox::OBB obb_;

	// 箱の回転情報の取得
	const VECTOR GetAxis(const int num)const { return obb_.axis[num]; }

	// クォータニオンから回転軸を計算
	void UpdateObbAxis(void);

	// 各頂点の計算（ワールド座標）
	void CalculateVertices(VECTOR outVertices[8]) const;

	// 線分とAABBの最短距離の二乗計算
	float ClosestSegmentAABB(const VECTOR& segA, const VECTOR& segB, const VECTOR& aabbMin, const VECTOR& aabbMax);
};

