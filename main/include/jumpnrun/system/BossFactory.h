#include"jumpnrun/mover/character/Character.h"
#include"jumpnrun/system/JumpnRunData.h"
namespace jnr
{
	class BossFactory
	{
	public:
		BossFactory(){}
		Character* create(gfw::Tasklist& tl, const JumpnRunData& data, const gmtr::Vector2D& p);

	private:
	};
}