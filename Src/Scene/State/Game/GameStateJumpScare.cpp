#include "../../../Manager/Game/CharacterManager.h"
#include "../../../Manager/Game/StageManager.h"
#include "../../../Object/Actor/Stage/StageObjectBase.h"
#include "../../../Object/Actor/Character/CharacterBase.h"
#include "GameStateJumpScare.h"

GameStateJumpScare::GameStateJumpScare() 
{
}

GameStateJumpScare::~GameStateJumpScare()
{
}

void GameStateJumpScare::Init()
{

}

void GameStateJumpScare::Update()
{
	// キャラクターの本体更新
	charaMng_.Update();

	// ステージ更新
	stageMng_.Update();
}

void GameStateJumpScare::Draw()
{
	// ステージ描画
	stageMng_.Draw();

	// キャラクター描画
	charaMng_.Draw();
}
