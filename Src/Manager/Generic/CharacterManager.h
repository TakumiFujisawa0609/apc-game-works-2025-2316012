#pragma once
#include <memory>
#include <unordered_map>
#include <string>
#include "../../Template/Singleton.h"

class ParameterLoad;
class CharacterBase;

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
		MAX
	};

	// キャラクターの名前リスト
	const std::vector<std::string> NAME_LIST = 
	{
		"player",
		"enemy",
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
	void MainUpdate();

	/// <summary>
	/// 更新後の処理
	/// </summary>
	void PostUpdate();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 指定したキャラクターを返す
	/// </summary>
	/// <param name="type">キャラクター種類</param>
	/// <returns>キャラクターを返す</returns>
	CharacterBase& GetCharacter(const TYPE type) { return *characterMap_[type]; }

private:

	// ファイル名
	const std::string FILE_NAME = "ParameterCharater";

	// パラメータ読み込み
	std::unique_ptr<ParameterLoad> paramLoad_;

	// キャラクターオブジェクトの管理マップ
	std::unordered_map<TYPE, std::unique_ptr<CharacterBase>> characterMap_;
	
	// コンストラクタ
	CharacterManager();
	
	// デストラクタ
	~CharacterManager() = default;
};