#pragma once
#include <DirectXMath.h>

class ViewProjection {
public:
	// 公開プロパティ（必要に応じて直接いじれるように）
	DirectX::XMFLOAT3 eye{0.0f, 2.5f, -6.5f};
	DirectX::XMFLOAT3 target{0.0f, 1.5f, 0.0f};
	DirectX::XMFLOAT3 up{0.0f, 1.0f, 0.0f};
	float fovY = DirectX::XMConvertToRadians(60.0f);
	float aspect = 16.0f / 9.0f;
	float nearZ = 0.05f;
	float farZ = 1000.0f;

	// 行列（CPU側で保持）
	DirectX::XMFLOAT4X4 matView{};
	DirectX::XMFLOAT4X4 matProjection{};
	DirectX::XMFLOAT4X4 matViewProj{};

	void Initialize();   // デフォルトのままでも良いが明示化
	void UpdateMatrix(); // eye/target/up, fov などから行列を再計算
};
