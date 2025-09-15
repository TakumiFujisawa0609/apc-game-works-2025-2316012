#pragma once
#include <memory>
#include <unordered_map>
#include "../../Template/Singleton.h"

class CharacterBase;

class CharacterManager : public Singleton<CharacterManager>
{
	//シングルトンにだけ共有する
	friend class Singleton<CharacterManager>;

public:

	//キャラクターの種類
	enum class TYPE
	{
		PLAYER,
		MAX
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
	/// 更新処理
	/// </summary>
	void Update();

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

	//キャラクターオブジェクトの管理マップ
	std::unordered_map<TYPE, std::unique_ptr<CharacterBase>> characterMap_;
	
	//コンストラクタ
	CharacterManager();
	
	//デストラクタ
	~CharacterManager() = default;
};

