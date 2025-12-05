#pragma once
#include "StageMain.h"

class CherryBlossomsWorld : public StageMain
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="key">オブジェクトの名前</param>
	/// <param name="mapParam">配置情報</param>
	/// <param name="colliderParam">コライダー情報</param>
	/// <param name="resetPlayerPos">プレイヤーリセット用座標</param>
	CherryBlossomsWorld(const std::string& key, const Json& mapParam, const Json& colliderParam, const VECTOR& resetPlayerPos);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~CherryBlossomsWorld() override;

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load() override;

	/// <summary>
	/// 初期化処理
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

private:

	void DrawMain() override;

	// 削除用メッシュの名前キー
	const std::string DELETE_MESH_KEY = "CherryBlossomsWorldMesh";

	// プレイヤーのリセット用位置
	const VECTOR RESET_PLAYER_POS;

};

