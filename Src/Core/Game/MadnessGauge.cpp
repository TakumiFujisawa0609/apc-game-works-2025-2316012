#include "../../Application.h"
#include "../../Manager/Common/SceneManager.h"
#include "../../Manager/Common/ResourceManager.h"
#include "../../Manager/Common/SoundManager.h"
#include "../../Manager/Common/FontManager.h"
#include "../../Utility/UtilityCommon.h"
#include "../../Object/Actor/Character/Player.h"
#include "MadnessGauge.h"

MadnessGauge::MadnessGauge(const Json& param, const Player& player) :
	player_(player),
	GAUGE_POS({ param["gaugePos"]["x"],param["gaugePos"]["y"]}),
	STR_SUN_POS({param["stringSunPos"]["x"], param["stringSunPos"]["y"]}),
	STR_DIE_POS({param["stringDiePos"]["x"], param["stringDiePos"]["y"]}),
	PIN_POS_X(param["pinPosX"]),
	PIN_POS_Y_MAX(param["pinPosYMax"]),
	PIN_POS_Y_MIN(param["pinPosYMin"]),
	STR_SUN(UtilityCommon::GetWStringFromString(UtilityCommon::ConvertUtf8ToSjis(param["stringSun"]))),
	STR_DIE(UtilityCommon::GetWStringFromString(UtilityCommon::ConvertUtf8ToSjis(param["stringDie"]))),
	FONT_NAME(param["fontName"]),
	FONT_SIZE(param["fontSize"]),
	FONT_THICK(param["fontThick"])
{
}

MadnessGauge::~MadnessGauge()
{
}

void MadnessGauge::Init()
{	
	// フォントの生成
	int font = fontMng_.CreateMyFont(resMng_.GetFontName(FONT_NAME), FONT_SIZE, FONT_THICK);	
	
	// リソースの用意
	imgGauge_.handleId = resMng_.GetHandle("madnessGauge");
	imgGauge_.pos = GAUGE_POS;

	imgPin_.handleId = resMng_.GetHandle("madnessGaugePin");
	imgPin_.pos = { PIN_POS_X, PIN_POS_Y_MAX };

	strSun_.fontHandle = font;
	strSun_.pos = STR_SUN_POS;
	strSun_.string = STR_SUN;
	strSun_.color = UtilityCommon::WHITE;

	strDie_.fontHandle = font;
	strDie_.pos = STR_DIE_POS;
	strDie_.string = STR_DIE;
	strDie_.color = UtilityCommon::WHITE;
}

void MadnessGauge::Update()
{
	constexpr float GAUGE_MAX = 100.0f;

	// ゲージのピン位置の更新
	int value = player_.GetMadnessValue();

	// 差分を計算
	float range = static_cast<float>(PIN_POS_Y_MIN - PIN_POS_Y_MAX);

	// 調整分の値を取得
	float offset = range * (static_cast<float>(value) / GAUGE_MAX);

	// ピンの位置を更新
	imgPin_.pos.y = PIN_POS_Y_MAX + static_cast<int>(offset);
}

void MadnessGauge::Draw()
{
	// ゲージの描画
	imgGauge_.DrawRota();

	// ゲージの装飾
	strSun_.DrawCenter();
	strDie_.DrawCenter();

	// 現在の狂気値を指すピンの描画
	imgPin_.DrawRota();
}