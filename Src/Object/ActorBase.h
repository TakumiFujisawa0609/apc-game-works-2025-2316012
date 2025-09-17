#pragma once
#include "Common/Transform.h"

class ResourceManager;
class SceneManager;

class ActorBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ActorBase();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ActorBase();

	/// <summary>
	/// 読み込み
	/// </summary>
	virtual void Load();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init();

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

protected:

	// ローカル回転のデフォルト値(度)
	static constexpr float DEFAULT_LOCAL_DEG_Y = 180.0f;

	// シングルトン参照
	ResourceManager& resMng_;
	SceneManager& scnMng_;

	// モデル制御の基本情報
	Transform transform_;

	//メインの更新処理
	virtual void UpdateMain();

	//処理の適用
	virtual void UpdateApply();

	//メインの描画
	virtual void DrawMain();

	//半透明の描画
	virtual void DrawTranslucent();

	//UIの描画
	virtual void DrawUI();

	//衝突時のコールバック
	virtual void OnCollision();

	//デバッグ時の更新
	virtual void DebugUpdate();

	//デバッグ時の描画
	virtual void DebugDraw();

};

