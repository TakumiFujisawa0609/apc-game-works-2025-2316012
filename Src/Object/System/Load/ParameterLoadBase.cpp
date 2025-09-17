#include "ParameterLoadBase.h"
#include "../../Character/Parameter/ParameterBase.h"

ParameterLoadBase::ParameterLoadBase()
{
}

ParameterBase* ParameterLoadBase::GetParameter(const std::string& name)
{
	// マップから要素を検索
	auto it = parametersMap_.find(name);

	// 見つかった場合
	if (it != parametersMap_.end())
	{
		return it->second.get();
	}

	// 見つからない場合、nullptrを返す
	return nullptr;
}
