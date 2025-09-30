#pragma once
#include <nlohmann/json.hpp>
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>

// JSON名前空間
using Json = nlohmann::json;

class ParameterLoad
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="fileName">ファイル名前</param>
	ParameterLoad(const std::string& fileName);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ParameterLoad();

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load();

	/// <summary>
	/// 指定したパラメーターファイル配列を返す
	/// </summary>
	/// <param name="key">取得したい要素のキー</param>
	/// <returns>パラメータファイル配列</returns>
	const std::vector<Json> GetParameterFile(const std::string& key) const;

	/// <summary>
	/// パラメーター管理マップを返す
	/// </summary>
	/// <returns>パラメーター管理マップ</returns>
	const std::unordered_map<std::string, std::vector<Json>> GetParameterMap() const { return parameterMap_; }

private:

	// ファイル名前
	const std::string FILE_NAME;

	// パラメーター管理マップ
	std::unordered_map<std::string, std::vector<Json>> parameterMap_;
};


