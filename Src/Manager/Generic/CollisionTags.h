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
		PLAYER,
		MAIN_STAGE,		// メインステージ
		DECO_STAGE,		// 装飾(ステージ)
		STAGE_GIMMICK,	// ステージギミック
		DECO_GIMMICK,	// 装飾(ギミック)
		REPORT,			
		MAX
	};

	// タグの数
	static constexpr int TAG_COUNT = static_cast<int>(TAG::MAX);

	// 各種タグの名前を管理するマップ
	static const std::map<TAG, std::string> TAG_NAME_MAP;
};