#include "ReportSystem.h"

ReportSystem::ReportSystem()
{
	state_ = STATE::NONE;
}

ReportSystem::~ReportSystem()
{
}

void ReportSystem::Load()
{
}

void ReportSystem::Init()
{
	// èâä˙èÛë‘
	state_ = STATE::WAIT;
}

void ReportSystem::Update()
{
	updateMap_[state_]();
}

void ReportSystem::Draw()
{
	drawMap_[state_]();
}

void ReportSystem::RegisterStateFunc(const STATE state, std::function<void()> update, std::function<void()> draw)
{
	updateMap_[state] = update;
	drawMap_[state] = draw;
}

void ReportSystem::UpdateWait()
{
}

void ReportSystem::UpdateReport()
{
}

void ReportSystem::UpdateReporting()
{
}

void ReportSystem::UpdateMiss()
{
}

void ReportSystem::UpdtateComplete()
{
}

void ReportSystem::DrawWait()
{
}

void ReportSystem::DrawReport()
{
}

void ReportSystem::DrawReporting()
{
}

void ReportSystem::DrawMiss()
{
}

void ReportSystem::DrawComplite()
{
}
