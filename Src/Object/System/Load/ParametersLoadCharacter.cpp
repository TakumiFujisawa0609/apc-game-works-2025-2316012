#include <DxLib.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <cassert>
#include <iostream>
#include "../../../Application.h"
#include "../../../Utility/UtilityCommon.h"
#include "../../Character/Parameter/ParameterBase.h"
#include "../../Character/Parameter/ParameterPlayer.h"
#include "ParametersLoadCharacter.h"

// JSON名前空間
using json = nlohmann::json;

ParametersLoadCharacter::ParametersLoadCharacter()
{
}

void ParametersLoadCharacter::Load()
{
    try 
    {
        // JSONファイルのパス
        std::string jsonPath = (Application::PATH_JSON + "CharacterParameters.json");

        // JSONファイルの読み込み
        std::ifstream ifs(jsonPath);
        if (!ifs.is_open())
        {
            // ファイルが開けない場合、例外をスロー
            throw std::runtime_error("ファイルが開けません: " + jsonPath);
        }

        // JSONデータのパース
        nlohmann::json jsonData;
        ifs >> jsonData;

        // 読み込んだデータを使用して処理を行う
        for (const auto& character : jsonData)
        {
            std::unique_ptr<ParameterBase> param = nullptr;

            // 'type'キーのチェック
            if (!character.contains("type"))
            {
                throw std::runtime_error("JSONデータに'type'キーがありません。");
            }
            std::string type = character["type"];

            if (type == "player")
            {
                param = std::make_unique<ParameterPlayer>();
                param->SetDataFile(character);
            }
            else 
            {
                // 未知のタイプの場合、例外をスロー
                throw std::runtime_error("登録されていないキャラクタータイプです: " + type);
            }

            // マップに格納
            parametersMap_[type] = std::move(param);
        }

        ifs.close(); // ファイルを閉じる

    }
    catch (const std::exception& e)
    {
        // 例外を捕捉し、エラー処理を行う
        // エラーログの出力や、ユーザーへのメッセージ表示など
        std::cerr << "エラーが発生しました: " << e.what() << std::endl;
        // 致命的なエラーとしてアプリケーションを終了させるなどの対応も可能
    }
}
