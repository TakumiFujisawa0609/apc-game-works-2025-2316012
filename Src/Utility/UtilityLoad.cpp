#include <DxLib.h>
#include <fstream>
#include <iostream>
#include "../Application.h"
#include "UtilityLoad.h"

const std::unordered_map<std::string, std::vector<Json>> UtilityLoad::GetJsonData(const std::string& fileName) 
{
    std::unordered_map<std::string, std::vector<Json>> dataMap;

    try
    {
        // JSONファイルのパス
        std::string jsonPath = (Application::PATH_JSON + fileName + ".json");

        // JSONファイルの読み込み
        std::ifstream ifs(jsonPath);
        if (!ifs.is_open())
        {
            throw std::runtime_error("ファイルが開けません: " + jsonPath);
            return dataMap;
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
                    dataMap[key].push_back(obj);
                }
            }
            // 単一オブジェクトの場合
            else
            {
                // そのまま格納
                dataMap[key].push_back(value);
            }
        }
        ifs.close();
    }
    catch (const std::exception& e)
    {
        std::cerr << "エラーが発生しました: " << e.what() << std::endl;
    }

    return dataMap;
}
