#pragma once
#include <map>
#include <string>

class ColliderType
{
public:

	/// <summary>
	/// 種類
	/// </summary>
	enum class TYPE
	{
		CAPSULE,
		SPHERE,
		LINE,
		MODEL,
		MAX
	};

	// コライダー種別名前管理マップ
	static const std::map<TYPE, std::string> TYPE_NAME_MAP;

	// コライダー種類数
	static constexpr int COLLIDER_TYPES = static_cast<int>(TYPE::MAX);
};

const std::map<ColliderType::TYPE, std::string> ColliderType::TYPE_NAME_MAP =
{
	{TYPE::CAPSULE, "capsule"},
	{TYPE::LINE, "line"},
	{TYPE::MODEL, "model"},
	{TYPE::SPHERE, "sphere"},
};