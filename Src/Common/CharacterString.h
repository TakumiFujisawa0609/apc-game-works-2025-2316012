#pragma once
#include <string>
#include "Vector2.h"

class CharacterString
{

public:	
	
	/// <summary>
	/// 文字列
	/// </summary>
	std::wstring srt_; 

	/// <summary>
	/// 座標
	/// </summary>
	Vector2 pos_;

	/// <summary>
	/// フォントハンドル
	/// </summary>
	int fontHandle_; 

	/// <summary>
	/// 色
	/// </summary>
	int color_;

	/// <summary>
	/// 描画用データ
	/// </summary>
	int data_;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CharacterString(void);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~CharacterString(void);

	/// <summary>
	/// 通常描画
	/// </summary>
	/// <param name=""></param>
	void Draw(void) const;

	/// <summary>
	/// 文字列の中心で描画
	/// </summary>
	/// <param name=""></param>
	void DrawCenter(void) const;

	/// <summary>
	/// 数値を含む文字列の中心描画
	/// </summary>
	/// <param name=""></param>
	void DrawFormatCenter(void) const;
	
};