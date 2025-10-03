#pragma once
#include <memory>
#include <string>
#include <functional>

#include <nlohmann/json.hpp>

class StageObjectBase;

// JSON–¼‘O‹óŠÔ
using Json = nlohmann::json;

class StagaObjectFactory
{
public:

	StagaObjectFactory();

	~StagaObjectFactory();

	std::unique_ptr<StageObjectBase> Create(const std::string& key, const Json& param);

private:


};

