#include <DxLib.h>
#include <fstream>
#include <cassert>
#include <iostream>
#include "../../../Application.h"
#include "../../../Utility/UtilityCommon.h"
#include "ParameterLoadCharacter.h"

ParameterLoadCharacter::ParameterLoadCharacter()
{
}

void ParameterLoadCharacter::Load()
{
    try
    {
        // JSONファイルのパス
        std::string jsonPath = (Application::PATH_JSON + "ParameterCharater.json");

        // JSONファイルの読み込み
        std::ifstream ifs(jsonPath);
        if (!ifs.is_open())
        {
            throw std::runtime_error("ファイルが開けません: " + jsonPath);
        }

        // JSONデータのパース
        Json jsonData;
        ifs >> jsonData;

        //キー
        std::string key = "";

        //プレイヤーのデータを処理
		key = "player";
        if (jsonData.contains(key))
        {
			parameterMap_.emplace(key, jsonData[key]);
            //parameterMap_[key] = jsonData[key];
        }

        ////敵のデータを処理
        //key = "enemy";
        //if (jsonData.contains(key) && jsonData[key].is_array())
        //{
        //    parameterMap_[key] = jsonData[key];
        //}

        ifs.close();
    }
    catch (const std::exception& e)
    {
        std::cerr << "エラーが発生しました: " << e.what() << std::endl;
    }
}
