#pragma once
#include "StageMain.h"

class GrassRoom : public StageMain
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="key">オブジェクトの名前</param>
	/// <param name="mapParam">配置情報</param>
	/// <param name="colliderParam">コライダー情報</param>
	GrassRoom(const std::string& key, const Json& mapParam, const Json& colliderParam);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GrassRoom() override;

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load() override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 異変の設定
	/// </summary>
	void SetAnomaly() override;

	/// <summary>
	/// 異変後の後処理
	/// </summary>
	void Refresh() override;

	/// <summary>
	/// 距離を返す
	/// </summary>
	/// <returns>距離</returns>
	const float GetDistance() const { return distance_; }

	/// <summary>
	/// 開始位置を返す
	/// </summary>
	/// <returns>開始位置</returns>
	const VECTOR& GetStartPos() const { return startPos_; }

private:

	// 草の生成数
	static constexpr int CREATE_GRASS = 300;

	// 影響を及ぼす距離
	float distance_;

	// 開始位置
	VECTOR startPos_;

	// 矩形のサイズ
	VECTOR rectMax_;	// 最大
	VECTOR rectMin_;	// 最小

	// 生成位置
	VECTOR GetRespownGrassPos();
};