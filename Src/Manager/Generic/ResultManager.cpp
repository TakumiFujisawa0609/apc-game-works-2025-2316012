#include "ResultManager.h"

void ResultManager::Init()
{
	score_ = 0;
}

void ResultManager::SetEndState(const END_STATE state)
{
	state_ = state;
}

void ResultManager::AddScore(const int score)
{
	score_ += score;
}

ResultManager::ResultManager()
{
	state_ = END_STATE::MAX;
	score_ = -1;
}

ResultManager::~ResultManager()
{
}