#pragma once
#include <nlohmann/json.hpp>
#include "../Common/Transform.h"

class ResourceManager;
class SceneManager;
class CollisionManager;
class ColliderFactory;
class ColliderBase;
enum class COLLISION_TAG;

// JSON名前空間
using Json = nlohmann::json;

class ActorBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ActorBase(const Json& param);

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
	/// メインの更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// 衝突後の処理
	/// </summary>
	/// <param name="opponentCollider">衝突相手のコライダー</param>
	virtual void OnHit(const std::weak_ptr<ColliderBase>& opponentCollider);

	/// <summary>
	/// トランスフォームを返す
	/// </summary>
	/// <returns>トランスフォーム</returns>
	const Transform& GetTransform() const { return transform_; }

protected:

	// ローカル回転のデフォルト値(度)
	static constexpr float DEFAULT_LOCAL_QUAROT_Y_DEG = 180.0f;

	// 初期位置
	const VECTOR INITIAL_POS;

	// 初期角度
	const VECTOR INITIAL_ROT;

	// 初期スケール
	const VECTOR INITIAL_SCL;

	// シングルトン参照
	ResourceManager& resMng_;
	SceneManager& scnMng_;
	CollisionManager& collMng_;
	ColliderFactory& collFtr_;

	// モデル制御の基本情報
	Transform transform_;

	//コライダー
	std::shared_ptr<ColliderBase> collider_;

	// トランスフォームの初期設定
	virtual void InitTransform();

	// メインの更新処理
	virtual void UpdateBody();

	// 処理の適用
	virtual void UpdateApply();

	// メインの描画
	virtual void DrawMain();

	// 半透明の描画
	virtual void DrawTranslucent();

	// UIの描画
	virtual void DrawUI();

	// 衝突時のコールバック
	virtual void OnCollision();

	// コライダーの追加
	virtual void AddCollider();

	// デバッグ時の更新
	virtual void DebugUpdate();

	// デバッグ時の描画
	virtual void DebugDraw();
};