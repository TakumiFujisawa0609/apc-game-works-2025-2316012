#include "CharacterString.h"
#include <DxLib.h>

CharacterString::CharacterString(void):
	fontHandle_(-1),
	srt_(L""),
	pos_({ 0, 0 }),
	color_(0),
	data_(0)
{
}

CharacterString::~CharacterString(void)
{
}

void CharacterString::Draw(void) const
{
	// 文字列を描画
	DrawFormatStringToHandle(pos_.x, pos_.y, color_, fontHandle_, srt_.c_str(), data_);
}

void CharacterString::DrawCenter(void) const
{
	// フォーマット済みの文字列を作成
	wchar_t buffer[256];

	swprintf(buffer, 256, srt_.c_str());

	// フォーマット済み文字列の幅を取得
	int strWidth = GetDrawStringWidthToHandle(buffer, wcslen(buffer), fontHandle_);

	// 幅の直後に次の文字列を描画
	DrawStringToHandle(pos_.x - strWidth / 2, pos_.y, srt_.c_str(), color_, fontHandle_);
}

void CharacterString::DrawFormatCenter(void) const
{
	// フォーマット済み文字列用バッファ
	wchar_t buffer[256];

	swprintf_s(buffer, 256, srt_.c_str(), data_);

	// フォーマット済み文字列の幅を取得
	int strWidth = GetDrawStringWidthToHandle(buffer, wcslen(buffer), fontHandle_);

	// 中央に描画
	DrawStringToHandle(pos_.x - strWidth / 2, pos_.y, buffer, color_, fontHandle_);
}
