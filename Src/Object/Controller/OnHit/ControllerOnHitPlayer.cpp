#include "../../Character/Player.h"
#include "../../Collider/CollisionTag.h"
#include "ControllerOnHitPlayer.h"

ControllerOnHitPlayer::ControllerOnHitPlayer(Player& owner)
{
	// Õ“Ë•¨•ÊŠÖ”‚Ì“o˜^
	RegisterOnHit(COLLISION_TAG::STAGE, [this](const ActorBase& actor) { OnHitStage(actor); });
}

ControllerOnHitPlayer::~ControllerOnHitPlayer()
{
}

void ControllerOnHitPlayer::OnHitStage(const ActorBase& actor)
{
	//À•W‚ğ‰Ÿ‚µ–ß‚·

}
