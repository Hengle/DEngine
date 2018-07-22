#include "DCuller.h"
#include "DGraphics.h"

DCuller::DCuller()
{
}


DCuller::~DCuller()
{
}

bool DCuller::Culling(DBounds bounds)
{

	//if (current != NULL)
	{
		DMatrix4x4 view, proj, mv;
		//current->GetViewMatrix(view);
		//current->GetProjection(proj);
		DGraphics::GetModelView(view);
		DGraphics::GetProjection(proj);

		mv = view*proj;

		int code = ComputeOutCode(DVector4(bounds.center.x + bounds.size.x * 0.5f, bounds.center.y + bounds.size.y * 0.5f,
			bounds.center.z + bounds.size.z * 0.5f, 1), mv);

		code &=
			ComputeOutCode(DVector4(bounds.center.x - bounds.size.x * 0.5f, bounds.center.y + bounds.size.y * 0.5f,
				bounds.center.z + bounds.size.z * 0.5f, 1), mv);

		code &=
			ComputeOutCode(DVector4(bounds.center.x + bounds.size.x * 0.5f, bounds.center.y - bounds.size.y * 0.5f,
				bounds.center.z + bounds.size.z * 0.5f, 1), mv);

		code &=
			ComputeOutCode(DVector4(bounds.center.x - bounds.size.x * 0.5f, bounds.center.y - bounds.size.y * 0.5f,
				bounds.center.z + bounds.size.z * 0.5f, 1), mv);

		code &=
			ComputeOutCode(DVector4(bounds.center.x + bounds.size.x * 0.5f, bounds.center.y + bounds.size.y * 0.5f,
				bounds.center.z - bounds.size.z * 0.5f, 1), mv);

		code &=
			ComputeOutCode(DVector4(bounds.center.x - bounds.size.x * 0.5f, bounds.center.y + bounds.size.y * 0.5f,
				bounds.center.z - bounds.size.z * 0.5f, 1), mv);

		code &=
			ComputeOutCode(DVector4(bounds.center.x + bounds.size.x * 0.5f, bounds.center.y - bounds.size.y * 0.5f,
				bounds.center.z - bounds.size.z * 0.5f, 1), mv);

		code &=
			ComputeOutCode(DVector4(bounds.center.x - bounds.size.x * 0.5f, bounds.center.y - bounds.size.y * 0.5f,
				bounds.center.z - bounds.size.z * 0.5f, 1), mv);

		if (code != 0) return false;

		return true;
	}

	return false;
}

int DCuller::ComputeOutCode(DVector4 & inputpos, DMatrix4x4 & projection)
{
	DVector4 pos = inputpos*projection;
	int code = 0;
	if (pos.x < -pos.w) code |= 0x01;
	if (pos.x > pos.w) code |= 0x02;
	if (pos.y < -pos.w) code |= 0x04;
	if (pos.y > pos.w) code |= 0x08;
	if (pos.z < -pos.w) code |= 0x10;
	if (pos.z > pos.w) code |= 0x20;
	return code;
}
