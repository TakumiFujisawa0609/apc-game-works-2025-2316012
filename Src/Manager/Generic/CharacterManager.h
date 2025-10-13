#pragma once
#include <memory>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include <string>
#include "../../Template/Singleton.h"

class ParameterLoad;
class CharacterBase;

// JSON名前空間
using Json = nlohmann::json;

class CharacterManager : public Singleton<CharacterManager>
{
	//シングルトンにだけ共有する
	friend class Singleton<CharacterManager>;

public:

	// 種類
	enum class TYPE
	{
		PLAYER,
		ENEMY,
		GHOST,
		MAX
	};

	// キャラクターの名前リスト
	const std::vector<std::string> NAME_LIST = 
	{
		"player",
		"enemy",
		"ghost"
	};

	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();

	/// <summary>
	/// メイン処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// キャラクターの追加
	/// </summary>
	/// <param name="type">種類</param>
	/// <param name="character">キャラクター/param>
	void AddCharacter(const TYPE type, const std::unique_ptr<CharacterBase> character);

	/// <summary>
	/// 指定したキャラクターを返す
	/// </summary>
	/// <param name="type">キャラクター種類</param>
	/// <returns>キャラクターを返す</returns>
	CharacterBase& GetCharacter(const TYPE type) { return *characterMap_[type]; }

	/// <summary>
	/// 指定されたキャラクターのパラメーターを返す
	/// </summary>
	/// <param name="type">種類</param>
	/// <returns>パラメータ</returns>
	const Json GetCharacterParam(const TYPE type) { return paramMap_[NAME_LIST[static_cast<int>(type)]].front(); }

private:

	// ファイル名
	const std::string FILE_NAME = "ParameterCharater";

	// パラメーター管理マップ
	std::unordered_map<std::string, std::vector<Json>> paramMap_;

	// キャラクターオブジェクトの管理マップ
	std::unordered_map<TYPE, std::unique_ptr<CharacterBase>> characterMap_;
	
	// コンストラクタ
	CharacterManager();
	
	// デストラクタ
	~CharacterManager() = default;
};