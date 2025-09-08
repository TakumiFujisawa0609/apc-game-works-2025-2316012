#pragma once
#include <string>
#include <map>
#include "../../Template/Singleton.h"

class FontManager : public Singleton<FontManager>
{
	// シングルトンにだけ共有
	friend class Singleton<FontManager>;

public:

	enum class FONT_TYPE
	{
		DOT,
		BOKUTATI,
		CORPORATE,
		MS_GOTHIC,
		PENGS,
		BANANA,
		HANAZOME,
		SOUKOU,
		GENKAI,
		ONION,
		MAX
	};

	//フォント種類
	static constexpr int FONT_TYPES = static_cast<int>(FONT_TYPE::MAX);

	//フォントネーム
	const std::wstring FONT_NAME[static_cast<int>(FONT_TYPE::MAX)] = 
	{
		L"ベストテンDOT",					// DOTフォント
		L"ぼくたちのゴシック",				// ぼくたちフォント
		L"コーポレート・ロゴ ver3 Bold",	// コーポレートフォント
		L"ＭＳ ゴシック",					// MSゴシック
		L"PENGS",							// ペンギンフォント
		L"YDW バナナスリップplus plus",		// バナナフォント
		L"はなぞめフォント",				// 花染フォント
		L"装甲明朝",						// 装甲フォント
		L"源界明朝",						// 源界フォント
		L"玉ねぎ楷書無料版v1.06",			// おにおんフォント
	};

	//フォントパス
	const std::wstring FONT_PATH[static_cast<int>(FONT_TYPE::MAX)] =
	{
		L"Dot.otf",				// DOTフォントのパス
		L"Bokutachi.otf",		// ぼくたちフォントのパス
		L"Corporate.otf",		// コーポレートフォントのパス
		L"MSGothic.ttf",		// MSゴシックのパス
		L"Pengs.ttf",			// ペンギンフォントのパス
		L"Banana.ttf",			// バナナフォントのパス
		L"Hanazome.ttf",		// 花染フォントのパス
		L"SoukouMincho.ttf",	// 装甲フォントのパス
		L"GenkaiMincho.ttf",	// 源界フォントのパス
		L"Tamanegi.ttf",		// おにおんフォントのパス
	};

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// オリジナルフォントの生成
	/// </summary>
	/// <param name="type">フォント種類</param>
	/// <param name="size">サイズ</param>
	/// <param name="thickness">太さ</param>
	/// <returns>生成できた場合true,失敗した場合false</returns>
	int CreateMyFont(const FONT_TYPE type, const int size, const int thickness);

	/// <summary>
	/// 解放処理
	/// </summary>
	void Release();

	/// <summary>
	/// 生成したフォントを解放する
	/// </summary>
	void SceneChangeRelease();

private:

	// フォント設定情報をまとめた構造体
	struct FontKey
	{
		FONT_TYPE type;
		int size;
		int thickness;

		// map用の比較演算子
		bool operator<(const FontKey& other) const noexcept
		{
			if (type != other.type) return type < other.type;
			if (size != other.size) return size < other.size;
			return thickness < other.thickness;
		}
	};

	//生成したフォントを管理するマップ
	std::map<FontKey, int> fontHandleMap_;
	
	/// <summary>
	/// コンストラクタ
	/// </summary>
	FontManager();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~FontManager() = default;
};