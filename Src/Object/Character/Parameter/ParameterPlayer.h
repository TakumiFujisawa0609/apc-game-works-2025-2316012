#pragma once
#include "ParameterBase.h"

class ParameterPlayer : public ParameterBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ParameterPlayer();
	 
	/// <summary>
	/// デストラクタ
	/// </summary>
	~ParameterPlayer() override {};

	/// <summary>
	/// データの格納
	/// </summary>
	/// <param name="file">パラメータファイル</param>
	void SetDataFile(const json& file) override;

	// ジャンプアニメーション速度
	float animationJumpSpeed_; 

	// ジャンプ力
	float jumpPower_;

	//ジャンプ受付時間
	float jumpAcceptTime_;
};

