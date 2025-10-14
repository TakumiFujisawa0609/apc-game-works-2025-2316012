#pragma once
#include <DxLib.h>
#include "../Common/Vector2.h"
#include "../Object/Collider/ColliderBox.h"

class Utility3D
{
public:

	//VECTORの初期化
	static constexpr VECTOR VECTOR_ZERO = { 0.0f, 0.0f, 0.0f };
	static constexpr VECTOR VECTOR_ONE = { 1.0f, 1.0f, 1.0f };

	// 回転軸
	static constexpr VECTOR AXIS_X = { 1.0f, 0.0f, 0.0f };
	static constexpr VECTOR AXIS_Y = { 0.0f, 1.0f, 0.0f };
	static constexpr VECTOR AXIS_Z = { 0.0f, 0.0f, 1.0f };
	static constexpr VECTOR AXIS_XZ = { 1.0f, 0.0f, 1.0f };
	static constexpr VECTOR AXIS_XYZ = { 1.0f, 1.0f, 1.0f };

	// 方向
	static constexpr VECTOR DIR_F = { 0.0f, 0.0f, 1.0f };
	static constexpr VECTOR DIR_B = { 0.0f, 0.0f, -1.0f };
	static constexpr VECTOR DIR_R = { 1.0f, 0.0f, 0.0f };
	static constexpr VECTOR DIR_L = { -1.0f, 0.0f, 0.0f };
	static constexpr VECTOR DIR_U = { 0.0f, 1.0f, 0.0f };
	static constexpr VECTOR DIR_D = { 0.0f, -1.0f, 0.0f };

	static constexpr float kEpsilonNormalSqrt = 1e-15F;

	// Y軸回転
	static VECTOR RotXZPos(const VECTOR& centerPos, const VECTOR& radiusPos, float rad);

	// ベクトルの長さ
	static double Magnitude(const VECTOR& v);
	static float MagnitudeF(const VECTOR& v);
	static float SqrMagnitudeF(const VECTOR& v);
	static double SqrMagnitude(const VECTOR& v);
	static double SqrMagnitude(const VECTOR& v1, const VECTOR& v2);
	static double Distance(const VECTOR& v1, const VECTOR& v2);

	// 球体同士の衝突判定
	static bool IsHitSpheres(
		const VECTOR& pos1, float radius1, const VECTOR& pos2, float radius2);

	// 球体とカプセルの衝突判定
	static bool IsHitSphereCapsule(
		const VECTOR& sphPos, float sphRadius,
		const VECTOR& capPos1, const VECTOR& capPos2, float capRadius);

	// 比較
	static bool Equals(const VECTOR& v1, const VECTOR& v2);
	static bool EqualsVZero(const VECTOR& v1);

	// 正規化
	static VECTOR Normalize(const Vector2& v);
	static VECTOR VNormalize(const VECTOR& v);

	// 2つのベクトルの間の角度
	static double AngleDeg(const VECTOR& from, const VECTOR& to);

	/// <summary>
	/// カプセルとラインの衝突判定
	/// </summary>
	/// <param name="capTopPos">カプセル先端座標</param>
	/// <param name="capDownPos">カプセル末端座標</param>
	/// <param name="capRadius">カプセルの半径</param>
	/// <param name="lineTopPos">ライン先端座標</param>
	/// <param name="lineEndPos">ラインの末端座標</param>
	/// <returns></returns>
	static bool CheckHitCapsuleToLine(const VECTOR& capTopPos, const VECTOR& capDownPos, float capRadius, const VECTOR& lineTopPos, const VECTOR& lineEndPos);

	/// <summary>
	/// ボックスとカプセルの衝突判定
	/// </summary>
	/// <param name="obb">バウンディングボックスの情報</param>
	/// <param name="boxPos">ボックスの位置</param>
	/// <param name="capTopPos">カプセル上部座標</param>
	/// <param name="capDownPos">カプセル下部座標</param>
	/// <param name="radius">カプセル半径</param>
	/// <returns>判定結果</returns>
	static bool CheckHitBox_Capsule(const ColliderBox::OBB& obb, const VECTOR& boxPos, const VECTOR& capTopPos, const VECTOR& capDownPos, const float radius);

	/// <summary>
	/// ボックスとラインの衝突判定
	/// </summary>
	/// <param name="obb">バウンディングボックスの情報</param>
	/// <param name="boxPos">ボックスの位置</param>
	/// <param name="lineTopPos">ラインの先端座標</param>
	/// <param name="lineEndPos">ラインの末尾座標</param>
	/// <returns>判定結果</returns>
	static bool CheckHitBox_Line(const ColliderBox::OBB& obb, const VECTOR& boxPos, const VECTOR& lineTopPos, const VECTOR& lineEndPos);

};