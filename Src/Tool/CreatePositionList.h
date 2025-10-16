#pragma once
#include <DxLib.h>
#include <string>
#include <vector>
#include "../Object/Common/Transform.h"

class InputManager;

class CreatePositionList
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CreatePositionList(const std::string& fileName, const Transform& player);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~CreatePositionList();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

private:

	// ファイル名
	const std::string fileName_;	

	// プレイヤーの参照
	const Transform& player_;

	// 入力管理クラスの参照
	InputManager& inputMng_;

	// 位置リスト
	std::vector<VECTOR> posList_;

	// 最後に格納した座標
	VECTOR lastPos_;

	// ファイルの生成
	void Create();
};

