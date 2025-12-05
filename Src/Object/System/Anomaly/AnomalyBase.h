#pragma once
#include <nlohmann/json.hpp>

class SceneManager;
class CharacterManager;
class StageManager;
class ResourceManager;
class GameSystemManager;
class SoundManager;
class Player;

// JSON名前空間
using Json = nlohmann::json;

class AnomalyBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="param">パラメータ</param>
	AnomalyBase(const Json& param);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~AnomalyBase();

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Init();

	/// <summary>
	/// 発生処理
	/// </summary>
	virtual void Occurrence();

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update();

protected:

	// 狂気値
	const int MADNESS_VALUE;

	// シーン管理クラス
	SceneManager& scnMng_;

	// キャラクター管理クラス
	CharacterManager& charaMng_;

	// ステージ管理クラス
	StageManager& stageMng_;

	// リソース管理クラス
	ResourceManager& resMng_;

	// ゲーム内のシステム管理クラス
	GameSystemManager& systemMng_;

	// サウンドの管理クラス
	SoundManager& sndMng_;
};

