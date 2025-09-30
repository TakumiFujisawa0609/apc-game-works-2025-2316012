#pragma once
#include <nlohmann/json.hpp>
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>

class ParameterBase;

// JSON名前空間
using Json = nlohmann::json;

class ParameterLoadBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ParameterLoadBase();
	 
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ParameterLoadBase() {};
	 
	/// <summary>
	/// 読み込み処理
	/// </summary>
	virtual void Load() = 0;

	/// <summary>
	/// パラメーターファイルを取得
	/// </summary>
	/// <param name="key">取得したい要素のキー</param>
	/// <returns>パラメータファイル</returns>
	Json GetParameterFile(const std::string& key);

protected:

	//パラメーターマップ
	std::unordered_map<std::string, Json> parameterMap_;
};

