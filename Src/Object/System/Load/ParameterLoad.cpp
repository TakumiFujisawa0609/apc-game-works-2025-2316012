#include <DxLib.h>
#include <fstream>
#include <iostream>
#include "../../../Application.h"
#include "../../../Utility/UtilityCommon.h"
#include "ParameterLoad.h"

ParameterLoad::ParameterLoad(const std::string& fileName, const std::vector<std::string>& nameList) :
    FILE_NAME(fileName),
    NAME_LIST(nameList)
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
        for (auto& key : NAME_LIST)
        {
            if (jsonData.contains(key))
            {
                parameterMap_.emplace(key, jsonData[key]);
            }
        }
        ifs.close();
    }
    catch (const std::exception& e)
    {
        std::cerr << "エラーが発生しました: " << e.what() << std::endl;
    }
}

const Json ParameterLoad::GetParameterFile(const std::string& key) const
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
	return Json();
}

std::vector<Json> ParameterLoad::GetParameterFiles()
{
    std::vector<Json> paramFiles;
    paramFiles.reserve(parameterMap_.size());

    // 
    for (const auto& pair : parameterMap_) 
    {
        // 値の格納
        paramFiles.push_back(pair.second);
    }

    return paramFiles;
}
