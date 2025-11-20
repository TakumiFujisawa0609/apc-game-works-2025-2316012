#pragma once
#include "StageMain.h"

class StageManager;

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

	/// <summary>
	/// 距離を返す
	/// </summary>
	/// <returns>影響</returns>
	const float GetDistance() const { return distance_; }

	/// <summary>
	/// 開始位置を返す
	/// </summary>
	/// <returns>開始位置</returns>
	const VECTOR& GetStartPos() const;

private:

	// 速度
	static constexpr float SPEED = 3.0f;

	// 生成数
	static constexpr int CREATE_NUM = 100;

	// ステージ管理クラスの参照
	StageManager& stageMng_;

	// 矩形の範囲
	VECTOR rectMax_;		// 最大
	VECTOR rectMin_;		// 最小

	// 影響を及ぼす距離
	float distance_;

	// 生成位置をランダムで決定
	VECTOR SpawnRandomPos();

};