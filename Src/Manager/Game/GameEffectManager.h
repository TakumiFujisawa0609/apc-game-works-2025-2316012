#pragma once
#include <unordered_map>
#include "../../Template/Singleton.h"

class PostEffectBase;

class GameEffectManager : public Singleton<GameEffectManager>
{
	// シングルトンにだけ共有する
	friend class Singleton<GameEffectManager>;

public:

	// 種類
	enum class TYPE
	{
		NONE,		// 使用しない
		GAME_SCREEN,// 通常描画のゲームスクリーン
		CRACKS,		// ひび割れ
		FILM_BURN,	// 写真焼き
		CHROMATIC_ABERRATION, // 色収差
	};

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// エフェクトの切り替え
	/// </summary>
	/// <param name="type">エフェクトの種類</param>
	void ChangeEffect(const TYPE type) { type_ = type; }

	/// <summary>
	/// エフェクトの種類を返す
	/// </summary>
	/// <returns>エフェクトの種類</returns>
	const TYPE GetType() const { return type_; }

	void SetStep(const float step);

private:	

	// 種類
	TYPE type_;
	
	// エフェクトスクリーン
	int effectScreen_;

	// ポストエフェクトの管理
	std::unordered_map<TYPE, std::unique_ptr<PostEffectBase>> effectMap_;

	// コンストラクタ
	GameEffectManager();

	// デストラクタ
	~GameEffectManager();
};

