#include <DxLib.h>
#include <fstream>
#include <iostream>
#include "../../../Application.h"
#include "../../../Utility/UtilityCommon.h"
#include "ParameterLoad.h"

ParameterLoad::ParameterLoad(const std::string& fileName) :
    FILE_NAME(fileName)
{
}

ParameterLoad::~ParameterLoad()
{
}

void ParameterLoad::Load()
{
    try
    {
        // JSONファイルのパス
        std::string jsonPath = (Application::PATH_JSON + FILE_NAME + ".json");

        // JSONファイルの読み込み
        std::ifstream ifs(jsonPath);
        if (!ifs.is_open())
        {
            throw std::runtime_error("ファイルが開けません: " + jsonPath);
        }

        // JSONデータのパース
        Json jsonData;
        ifs >> jsonData;

        // リスト順にデータを格納
        for (auto& [key, value] : jsonData.items())
        {
            // 配列の場合
            if (value.is_array())
            {
                // 配列の場合
                for (auto& obj : value)
                {
                    // 1つずつ格納
                    parameterMap_[key].push_back(obj);
                }
            }
            // 単一オブジェクトの場合
            else
            {
                // そのまま格納
                parameterMap_[key].push_back(value);
            }
        }
        ifs.close();
    }
    catch (const std::exception& e)
    {
        std::cerr << "エラーが発生しました: " << e.what() << std::endl;
    }
}

const std::vector<Json> ParameterLoad::GetParameterFile(const std::string& key) const
{
	// マップからキーに対応するJsonオブジェクトを検索
	auto it = parameterMap_.find(key);

	// キーが見つかった場合
	if (it != parameterMap_.end())
	{
		// 取得した値をそのまま返す
		return it->second;
	}

	// キーが見つからない場合は空のJsonオブジェクトを返す
	return std::vector<Json>();
}