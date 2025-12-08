#include "../../../Manager/Game/StageManager.h"
#include "../../../Manager/Game/CharacterManager.h"
#include "../../../Manager/Game/GameSystemManager.h"
#include "../../../Manager/Game/CollisionManager.h"
#include "../../../Manager/Game/CollisionTags.h"
#include "../../../Manager/Common/SceneManager.h"
#include "../../../Manager/Common/ResourceManager.h"
#include "../../../Manager/Common/EffectManager.h"
#include "../../Collider/ColliderModel.h"
#include "../../Controller/Draw/ControllerDrawCherryBlossomsWorld.h"
#include "../../Controller/OnHit/ControllerOnHitStageObject.h"
#include "../Character/Player.h"
#include "CherryBlossomsWorld.h"
#include "StageMesh.h"

CherryBlossomsWorld::CherryBlossomsWorld(const std::string& key, const Json& mapParam, const Json& colliderParam, const VECTOR& restPlayerPos) :
	 StageMain(key, mapParam, colliderParam),
	RESET_PLAYER_POS(restPlayerPos)
{
}

CherryBlossomsWorld::~CherryBlossomsWorld()
{
}

void CherryBlossomsWorld::Load()
{
	// モデルの設定
	transform_.SetModel(resMng_.GetHandle(STAGE_KEY));

	// 描画の設定
	draw_ = std::make_unique<ControllerDrawCherryBlossomsWorld>(transform_.modelId);
	draw_->Load();

	// 基底クラスの読み込み
	ActorBase::Load();

	// 衝突後処理の生成
	onHit_ = std::make_unique<ControllerOnHitStageObject>(*this);
}

void CherryBlossomsWorld::Init()
{
	StageMain::Init();

	// 生成時に異変を発生
	SetAnomaly();
}

void CherryBlossomsWorld::SetAnomaly()
{
	// モデルのコライダーを生成
	anomalyCollider_ = std::make_shared<ColliderModel>(*this, CollisionTags::TAG::ANOMALY);

	// コライダーの追加
	collMng_.Add(anomalyCollider_);
}

void CherryBlossomsWorld::Refresh()
{
	StageManager& stageMng = StageManager::GetInstance();
	CharacterManager& charaMng = CharacterManager::GetInstance();
	GameSystemManager& systemMng = GameSystemManager::GetInstance();

	// 基底クラスの処理
	StageMain::Refresh();

	// 自身の削除
	SetDelete();

	// 衝突判定用のメッシュも削除
	StageMesh * mesh = dynamic_cast<StageMesh*>(stageMng.GetStageObjects(DELETE_MESH_KEY)[0].get());
	mesh->SetDelete();

	// メインステージの復元
	stageMng.SetIsActiveByAllObjects(true);

	// システムのタイマー再開
	systemMng.SetIsActiveSystem(GameSystemManager::TYPE::GAME_TIMER, true);

	// UIの表示
	systemMng.SetIsActiveSystem(GameSystemManager::TYPE::SCREEN, true);
	systemMng.SetIsActiveSystem(GameSystemManager::TYPE::MANUAL, true);

	// 敵の更新開始
	charaMng.SetIsActive(CharacterManager::TYPE::ENEMY, true);

	// プレイヤーの位置を初期位置の位置に戻す
	auto& player = charaMng.GetCharacter(CharacterManager::TYPE::PLAYER);
	player.SetPos(RESET_PLAYER_POS);

	// フォグの距離を修正
	SetFogStartEnd(SceneManager::FOG_START, SceneManager::FOG_END);

	// エフェクトの停止
	EffectManager::GetInstance().Stop(EffectType::TYPE::PETAL_FALL);
}

void CherryBlossomsWorld::DrawMain()
{
	draw_->Draw();
	//MV1DrawModel(transform_.modelId);
}
