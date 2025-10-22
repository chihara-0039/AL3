#include "ViewProjection.h"
using namespace DirectX;

void ViewProjection::Initialize() { UpdateMatrix(); }

void ViewProjection::UpdateMatrix() {
	const XMVECTOR vEye = XMLoadFloat3(&eye);
	const XMVECTOR vTarget = XMLoadFloat3(&target);
	const XMVECTOR vUp = XMLoadFloat3(&up);

	const XMMATRIX view = XMMatrixLookAtRH(vEye, vTarget, vUp);
	const XMMATRIX proj = XMMatrixPerspectiveFovRH(fovY, aspect, nearZ, farZ);
	const XMMATRIX vp = XMMatrixMultiply(view, proj);

	XMStoreFloat4x4(&matView, view);
	XMStoreFloat4x4(&matProjection, proj);
	XMStoreFloat4x4(&matViewProj, vp);
}