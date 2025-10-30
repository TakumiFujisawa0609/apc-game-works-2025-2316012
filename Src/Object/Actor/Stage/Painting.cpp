#include "Painting.h"

Painting::Painting(const std::string& key, const Json& mapParam, const Json& colliderParam) : 
	StageGimmickObjectBase(key, mapParam, colliderParam)
{
	isActive_ = false;
}

Painting::~Painting()
{
}

void Painting::Draw()
{
	// 本オブジェクトのみ活動状態に限らず描画を行う
	DrawMain();
}

void Painting::Refresh()
{
	// テクスチャを戻す
	MV1SetTextureGraphHandle(transform_.modelId, textureIndex_, preTexture_, true);

	// 初期化
	textureIndex_ = -1;
	preTexture_ = -1;

	isActive_ = false;
}
