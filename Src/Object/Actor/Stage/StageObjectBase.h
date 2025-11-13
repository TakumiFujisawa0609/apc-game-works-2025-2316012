#pragma once
#include "../ActorBase.h"

class CharacterManager;
class ControllerDrawBase;

class StageObjectBase : public ActorBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="key">オブジェクトの名前</param>
	/// <param name="mapParam">配置情報</param>
	/// <param name="colliderParam">コライダー情報</param>
	StageObjectBase(const std::string& key, const Json& mapParam, const Json& colliderParam);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~StageObjectBase();

	/// <summary>
	/// 読み込み処理
	/// </summary>
	virtual void Load() override;

	/// <summary>
	/// モデルIDを返す
	/// </summary>
	/// <returns>モデルID</returns>
	const int GetModelId() const { return transform_.modelId; }

	/// <summary>
	/// 部屋の識別タグを返す
	/// </summary>
	/// <returns></returns>
	const std::string& GetRoomTag() const { return ROOM_TAG; }

	/// <summary>
	/// ステージキーを返す
	/// </summary>
	/// <returns>ステージキー</returns>
	const std::string& GetStageKey() const { return STAGE_KEY; }

	/// <summary>
	/// 透過描画判定(順番を後半に回すか)
	/// </summary>
	/// <returns>trueの場合順番を後半に回す</returns>
	const bool IsTranslucent() const { return isTrans_; }

protected:

	// ステージキー
	const std::string STAGE_KEY;

	// 処理用部屋識別タグ
	const std::string ROOM_TAG;

	// 透過描画判定
	bool isTrans_;

	// 描画の制御
	std::unique_ptr<ControllerDrawBase> draw_;

	// メインの描画処理
	void DrawMain() override;

	// トランスフォームの初期化
	void InitTransform() override;

	// デバッグ時の描画
	void DebugDraw() override;
};