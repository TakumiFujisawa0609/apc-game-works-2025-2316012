#pragma once
#include <DxLib.h>
#include "ColliderBase.h"
#include "../Common/Transform.h"

class ColliderCapsule : public ColliderBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">所有者のインスタンス</param>
	/// <param name="tag">衝突物の種類</param>
	ColliderCapsule(ActorBase& owner, const CollisionTags::TAG tag);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ColliderCapsule() override;

	/// <summary>
	/// 描画
	/// </summary>
	void DebugDraw() override;

	/// <summary>
	/// 親トランスフォームからの相対位置を返す
	/// </summary>
	/// <param name=""></param>
	/// <returns>カプセル上部</returns>
	VECTOR GetLocalPosTop(void) const { return localPosTop_; }

	/// <summary>
	/// 親トランスフォームからの相対位置を返す
	/// </summary>
	/// <param name=""></param>
	/// <returns>カプセル下部</returns>
	VECTOR GetLocalPosDown(void) const { return localPosDown_; }

	/// <summary>
	/// 座標を返す
	/// </summary>
	/// <param name=""></param>
	/// <returns>カプセル上部</returns>
	VECTOR GetPosTop(void) const;

	/// <summary>
	/// 座標を返す
	/// </summary>
	/// <param name=""></param>
	/// <returns>カプセル下部</returns>
	VECTOR GetPosDown(void) const;

	/// <summary>
	/// 半径を返す
	/// </summary>
	/// <param name=""></param>
	/// <returns>半径</returns>
	float GetRadius(void) const { return radius_; }

	/// <summary>
	/// 高さを返す
	/// </summary>
	/// <param name=""></param>
	/// <returns>高さ</returns>
	float GetHeight(void) const { return localPosTop_.y; }

	/// <summary>
	/// 中心座標を返す
	/// </summary>
	/// <param name=""></param>
	/// <returns>中心座標</returns>
	VECTOR GetCenter(void) const;	
	
	/// <summary>
	/// 親からの相対位置を設定
	/// </summary>
	/// <param name="pos">上部の相対位置</param>
	void SetLocalPosTop(const VECTOR& pos) { localPosTop_ = pos; }

	/// <summary>
	/// 親からの相対位置を設定
	/// </summary>
	/// <param name="pos">下部の相対位置</param>
	void SetLocalPosDown(const VECTOR& pos) { localPosDown_ = pos; }
	
	/// <summary>
	/// 半径の設定
	/// </summary>
	/// <param name="radius">半径</param>
	void SetRadius(float radius) { radius_ = radius; }

private:

	// 半径
	float radius_;	

	// 親からの相対位置(上側)
	VECTOR localPosTop_;

	// 親からの相対位置(下側)
	VECTOR localPosDown_;
};