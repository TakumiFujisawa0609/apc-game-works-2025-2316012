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
	/// <param name="nameList">読み込み用名前リスト</param>
	ParameterLoad(const std::string& fileName, const std::vector<std::string>& nameList);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ParameterLoad();

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load();

	/// <summary>
	/// 指定したパラメーターファイルを返す
	/// </summary>
	/// <param name="key">取得したい要素のキー</param>
	/// <returns>パラメータファイル</returns>
	const Json GetParameterFile(const std::string& key) const;

	/// <summary>
	/// パラメーターファイルをすべて返す
	/// </summary>
	/// <returns>パラメーターファイルすべて</returns>
	std::vector<Json> GetParameterFiles();

private:

	// ファイル名前
	const std::string FILE_NAME;

	// パラメータ読み込み用名前リスト
	const std::vector<std::string> NAME_LIST;

	// パラメーター管理マップ
	std::unordered_map<std::string, Json> parameterMap_;
};


