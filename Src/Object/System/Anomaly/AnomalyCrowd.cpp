#include "../../../Manager/Game/StageManager.h"
#include "../../../Utility/Utility3D.h"
#include "../../Actor/Stage/Sub/Human.h"
#include "AnomalyCrowd.h"

AnomalyCrowd::AnomalyCrowd(const Json& param) :
	AnomalyBase(param)
{
}

AnomalyCrowd::~AnomalyCrowd()
{
}

void AnomalyCrowd::Occurrence()
{
	// êlÇÃê∂ê¨
	std::unique_ptr<Human> human = std::make_unique<Human>(Utility3D::VECTOR_ZERO);

	// ä«óùÉNÉâÉXÇ…äiî[
	stageMng_.AddBillboardObject(StageManager::BILLBOARD_OBJ_TYPE::HUMAN, std::move(human));
}
