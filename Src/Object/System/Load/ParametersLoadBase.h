#pragma once
#include <memory>
#include <unordered_map>
#include <string>

class ParameterBase;

class ParametersLoadBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ParametersLoadBase();
	 
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ParametersLoadBase() {};
	 
	/// <summary>
	/// 読み込み処理
	/// </summary>
	virtual void Load() = 0;

protected:

	//パラメーターマップ
	std::unordered_map<std::string, std::unique_ptr<ParameterBase>> parametersMap_;
};

