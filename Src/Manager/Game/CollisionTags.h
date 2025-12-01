#pragma once
#include <string>
#include <map>

class CollisionTags
{
public:

	// 衝突物の種類
	enum class TAG
	{
		NONE,
		PLAYER,					// プレイヤー
		STAGE_MAIN,				// ステージメイン
		STAGE_GIMMICK,			// ステージギミック
		REPORT,					// 報告用
		GHOST,					// ゴースト
		ENEMY,					// 敵
		ENEMY_VIEW,				// 敵の視線
		CHARACTER_GRAVITY, // 重力判定用線 
		PLAYER_LIGHT,			// ライト
		ANOMALY,				// 異変
		MAX
	};

	// タグの数
	static constexpr int TAG_COUNT = static_cast<int>(TAG::MAX);

	// 各種タグの名前を管理するマップ
	static const std::map<TAG, std::string> TAG_NAME_MAP;
};