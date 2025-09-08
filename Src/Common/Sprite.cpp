#include "Sprite.h"
#include <DxLib.h>

Sprite::Sprite() :
	handleId(-1),
	pos(Vector2(0, 0)),
	size(Vector2(0, 0)),
	div(Vector2(1, 1)),
	center(Vector2(0, 0)),
	angle(0.0f),
	scale(1.0f),
	isTrans(true),
	isFlipX(false),
	isFlipY(false)
{
}

Sprite::~Sprite()
{
}

const Vector2 Sprite::GetCenterPos() const
{
	// íÜêSç¿ïWÇåvéZÇµÇƒï‘Ç∑
	return Vector2(
		pos.x + size.x / 2,
		pos.y + size.y / 2
	);
}

const void Sprite::Draw() const
{
	DrawGraph(
		pos.x,
		pos.y,
		handleId,
		isTrans
	);
}

const void Sprite::DrawRota() const
{
	DrawRotaGraph(
		pos.x,
		pos.y,
		scale,
		angle,
		handleId,
		isTrans,
		isFlipX,
		isFlipY
	);
}