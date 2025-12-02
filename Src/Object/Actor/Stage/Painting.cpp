#include "../../../Manager/Common/ResourceManager.h"
#include "../../../Manager/Game/CharacterManager.h"
#include "../../../Utility/UtilityCommon.h"
#include "../../../Utility/Utility3D.h"
#include "../../../Common/Quaternion.h"
#include "../../Controller/Draw/ControllerDrawStage.h"
#include "../../Controller/Draw/ControllerDrawPainting.h"
#include "../../Controller/OnHit/ControllerOnHitStageObject.h"
#include "Painting.h"

Painting::Painting(const std::string& key, const Json& mapParam, const Json& colliderParam) : 
	StageGimmickObjectBase(key, mapParam, colliderParam)
{
}

Painting::~Painting()
{
}

void Painting::Load()
{
	// モデルの設定
	transform_.SetModel(resMng_.GetHandle(STAGE_KEY));

	// 描画
	draw_ = std::make_unique<ControllerDrawStage>(transform_.modelId);
	draw_->Load();

	// 衝突後処理の生成
	onHit_ = std::make_unique<ControllerOnHitStageObject>(*this);

	// 基底クラスの読み込み
	ActorBase::Load();
}

void Painting::Refresh()
{
	StageGimmickObjectBase::Refresh();

	// テクスチャを戻す
	MV1SetTextureGraphHandle(transform_.modelId, textureIndex_, preTexture_, true);

	// 初期化
	textureIndex_ = -1;
	preTexture_ = -1;
}

void Painting::DrawMain()
{
	// 描画
	draw_->Draw();
}
