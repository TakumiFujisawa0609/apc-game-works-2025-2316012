#pragma once
#include <nlohmann/json.hpp>
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>

// JSON名前空間
using Json = nlohmann::json;

class UtilityLoad
{
public:

	/// <summary>
	/// JSONデータを読み込んで返す
	/// </summary>
	/// <param name="fileName">読み込むファイル名前</param>
	/// <returns>読み込んだファイルデータ</returns>
	static const std::unordered_map<std::string, std::vector<Json>> GetJsonData(const std::string& fileName);
};

