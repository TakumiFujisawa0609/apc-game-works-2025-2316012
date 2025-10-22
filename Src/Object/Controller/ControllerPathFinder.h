#pragma once
#include <vector>
#include <DxLib.h>
#include "ControllerBase.h"

class ControllerPathFinder : public ControllerBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ControllerPathFinder();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ControllerPathFinder() override;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 経路探索
	/// </summary>
	/// <param name="startIndex">開始インデックス</param>
	/// <param name="goalIndex">終了インデックス</param>
	/// <param name="maxMoveDistance">隣接ノードとみなす最大距離</param>
	/// <param name="path">経路のノードインデックスを格納する</param>
	/// <returns>trueの場合成功,falseの場合失敗</returns>
	bool FindPath(
		const int startIndex,
		const int goalIndex,
		float maxMoveDistance,
		std::vector<int>& path
		);

	/// <summary>
	/// 移動ポイント配列の設定
	/// </summary>
	/// <param name="points">ポイント配列</param>
	void SetPoints(const std::vector<VECTOR>& points) { points_ = points; }

	/// <summary>
	/// 指定した座標が最も近い位置番号を返す
	/// </summary>
	/// <param name="pos">位置リストの番号</param>
	/// <returns></returns>
	int GetNearNodeIndex(const VECTOR& pos);

	std::vector<int> GetModels() const { return models_; }

private:	
	
	// ノード
	struct Node
	{
		int index;          // ノード番号
		float goalCost;     // スタートからの実コスト
		float heuristic;    // ゴールまでの推定コスト（ヒューリスティック）
		float totalCost;    // 評価コスト
		int parentIndex;	// 経路復元のための親ノードのインデックス

		// 優先度付きキューで使う比較演算子（総コストが小さいほど優先度が高い）
		bool operator>(const Node& other) const
		{
			return totalCost > other.totalCost;
		}
	};

	// ステージとの衝突判定
	bool CheckCollisionModelToLine(const VECTOR& start, const VECTOR& end);

	// 移動ポイントのリスト
	std::vector<VECTOR> points_;

	// 判定用のモデル配列
	std::vector<int> models_;
};