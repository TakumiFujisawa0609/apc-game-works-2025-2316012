#include "FontManager.h"
#include <DxLib.h>
#include <cassert>
#include "../Application.h"

FontManager::FontManager()
{
	fontHandleMap_.clear();
}

void FontManager::Init()
{
	// フォントをシステムに登録
	for (int i = 0; i < FONT_TYPES; i++)
	{
		if (AddFontResourceEx(
			(Application::PATH_FONT + FONT_PATH[i]).c_str(),
			FR_PRIVATE,
			NULL)
			== -1)
		{
			return;
		}
	}
}

int FontManager::CreateMyFont(const FONT_TYPE type, const int size, const int thickness)
{
	FontKey key{ type, size, thickness };            // キー

	auto it = fontHandleMap_.find(key);                    // 既存検索
	if (it != fontHandleMap_.end())
	{
		return it->second;                           // 既存を返す
	}

	// 生成
	int handle = CreateFontToHandle(
		FONT_NAME[static_cast<int>(type)].c_str(),   // フォント名
		size,                                        // サイズ
		thickness);                                  // 太さ

	assert(handle != -1 && L"フォント生成に失敗しました");

	// 上限を超えたら最古を一つ破棄する
	//if (static_cast<int>(fontHandleMap_.size()) >= FONT_CACHE_LIMIT)
	//{
	//	auto oldest = fontHandleMap_.begin();        // 先頭要素
	//	DeleteFontToHandle(oldest->second);          // 破棄
	//	fontHandleMap_.erase(oldest);                // 削除
	//}

	fontHandleMap_[key] = handle;                    // 登録
	return handle;                                   // 生成結果
}

void FontManager::Release()
{
	SceneChangeRelease();

	// フォント登録解除
	for (int i = 0; i < FONT_TYPES; i++)
	{
		if (RemoveFontResourceEx(
			(Application::PATH_FONT + FONT_PATH[i]).c_str(),
			FR_PRIVATE,
			NULL)
			== -1)
		{
			return;
		}
	}
}

void FontManager::SceneChangeRelease()
{
	//生成したフォントを解放
	for (const auto& font : fontHandleMap_)
	{
		DeleteFontToHandle(font.second);
	}

	fontHandleMap_.clear();
}
