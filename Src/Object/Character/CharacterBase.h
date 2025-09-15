#pragma once
#include "../ActorBase.h"

class CharacterBase : public ActorBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CharacterBase();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CharacterBase() = default;
	
	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Init() override;

	/// <summary>
	/// トランスフォームを返す
	/// </summary>
	/// <returns>トランスフォーム</returns>
	const Transform& GetTransform() const { return transform_; }

protected:

	//通常アニメーション速度
	float animDefaultSpeed_;

	/// <summary>
	/// 更新処理の適用
	/// </summary>
	virtual void UpdateApply() override;

	/// <summary>
	/// メインの描画処理
	/// </summary>
	virtual void DrawMain() override;

	/// <summary>
	/// アニメーションの初期化
	/// </summary>
	virtual void InitAnimation() = 0;
};

