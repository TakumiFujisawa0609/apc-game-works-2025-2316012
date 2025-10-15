#include <vector>
#include <algorithm>
#include <cmath>
#include <map>
#include <queue>
#include "../../Utility/UtilityCommon.h"
#include "../../Utility/Utility3D.h"
#include "ControllerPathFinder.h"

ControllerPathFinder::ControllerPathFinder()
{
	points_.clear();
}

ControllerPathFinder::~ControllerPathFinder()
{

}

void ControllerPathFinder::Update()
{

}

bool ControllerPathFinder::FindPath(const int startIndex, const int goalIndex, float maxMoveDistance, std::vector<int>& path)
{
	// 開始ノードと終了ノードが同じ場合、開始ノードを経路に追加して終了
    if (startIndex == goalIndex) 
    {
        path.push_back(startIndex);
        return true;
    }

    // オープンリスト(総コストが最小のノードが先頭に来る)
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openList;
    
    // クローズドリスト(探索済みノード)
	std::map<int, Node> closedList; 

    // 開始ノードの初期化
    Node startNode;
    startNode.index = startIndex;
    startNode.goalCost = 0.0f;

    // ゴールまでの直線距離(ヒューリスティック)
    startNode.heuristic = Utility3D::Length(points_[startIndex], points_[goalIndex]);
	startNode.totalCost = startNode.goalCost + startNode.heuristic;
	startNode.parentIndex = -1; // 親ノードなし

	openList.push(startNode);
    closedList[startIndex] = startNode;

    // 探索ループ
    while (!openList.empty())
    {
		Node currentNode = openList.top();
        openList.pop();

        int currentIndex = currentNode.index;

        // mapに格納された総コストよりも大きい場合
		if (currentNode.goalCost > closedList[currentIndex].goalCost)
        {
            // スキップ
			continue;
		}

        // ゴールに到達した場合
        if (currentIndex == goalIndex)
        {
            // 経路復元
            int current = goalIndex;
			while (current != -1)
			{
				path.push_back(current);
				current = closedList[current].parentIndex;
			}

            // 経路をスタートからゴールへ反転
			std::reverse(path.begin(), path.end());
            return true;
        }

		// 隣接ノードの探索
        const VECTOR& currentPos = points_[currentIndex];

		for (int i = 0; i < points_.size(); ++i)
		{
			// 自分自身はスキップ
			if (i == currentIndex)
			{
				continue;
			}
			// 隣接ノードの位置と距離を計算
			const VECTOR& neighborPos = points_[i];
			float distance = Utility3D::Length(currentPos, neighborPos);

			// 隣接ノードの判定
			if (distance <= maxMoveDistance)
			{
				float tentativeGoalCost = currentNode.goalCost + distance;

				// 既にクローズドリストに存在する場合
				if (closedList.find(i) != closedList.end())
				{
					// より良いコストであれば更新
					if (tentativeGoalCost < closedList[i].goalCost)
					{
						Node& neighborNode = closedList[i];
						neighborNode.goalCost = tentativeGoalCost;
						neighborNode.totalCost = tentativeGoalCost + neighborNode.heuristic;
						neighborNode.parentIndex = currentIndex;

						openList.push(neighborNode);
					}
				}
				else
				{
					// 新しいノードとして追加
					Node neighborNode;
					neighborNode.index = static_cast<int>(i);
					neighborNode.goalCost = tentativeGoalCost;
					neighborNode.heuristic = Utility3D::Length(neighborPos, points_[goalIndex]);
					neighborNode.totalCost = neighborNode.goalCost + neighborNode.heuristic;
					neighborNode.parentIndex = currentIndex;

					openList.push(neighborNode);
					closedList[i] = neighborNode;
				}
			}
		}
    }
}
