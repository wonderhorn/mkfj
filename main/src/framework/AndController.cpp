#include"framework/VirtualController.h"
using namespace gfw;

AndController::AndController(VirtualController* vc1, VirtualController* vc2)
	:vc1(vc1), vc2(vc2){}

bool AndController::poll()
{
	z.setValue(vc1->Z().isDown() || vc2->Z().isDown());
	x.setValue(vc1->X().isDown() || vc2->X().isDown());
	c.setValue(vc1->C().isDown() || vc2->C().isDown());
	a.setValue(vc1->A().isDown() || vc2->A().isDown());
	s.setValue(vc1->S().isDown() || vc2->S().isDown());
	space.setValue(vc1->Space().isDown() || vc2->Space().isDown());

	up.setValue(vc1->Up().isDown() || vc2->Up().isDown());
	right.setValue(vc1->Right().isDown() || vc2->Right().isDown());
	down.setValue(vc1->Down().isDown() || vc2->Down().isDown());
	left.setValue(vc1->Left().isDown() || vc2->Left().isDown());

	return true;
}