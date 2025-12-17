#pragma once
#include "BillboardObjectBase.h"

class SceneManager;
class ModelMaterial;
class BillboardRenderer;

class Human : public BillboardObjectBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">座標</param>
	Human(const VECTOR& pos);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Human() override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;

	/// <summary>
	/// メインの更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

private:

};

