#include <DxLib.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <cassert>
#include <iostream>
#include "../../../Application.h"
#include "../../../Utility/UtilityCommon.h"
#include "../../Character/Parameter/ParameterBase.h"
#include "../../Character/Parameter/ParameterPlayer.h"
#include "ParameterLoadCharacter.h"

// JSON名前空間
using json = nlohmann::json;

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
        nlohmann::json jsonData;
        ifs >> jsonData;

        // "player"キーのデータを処理
        if (jsonData.contains("player") && jsonData["player"].is_array())
        {
            for (const auto& character : jsonData["player"])
            {
                std::unique_ptr<ParameterBase> param = std::make_unique<ParameterPlayer>();
                param->SetDataFile(character);
                parametersMap_["player"] = std::move(param);
            }
        }

        //// "enemy"キーのデータを処理
        //if (jsonData.contains("enemy") && jsonData["enemy"].is_array())
        //{
        //    for (const auto& character : jsonData["enemy"])
        //    {
        //        std::unique_ptr<ParameterBase> param = std::make_unique<ParameterEnemy>(); // enemy用のクラスを想定
        //        param->SetDataFile(character);
        //        parametersMap_["enemy"] = std::move(param);
        //    }
        //}

        ifs.close();
    }
    catch (const std::exception& e)
    {
        std::cerr << "エラーが発生しました: " << e.what() << std::endl;
    }
}
