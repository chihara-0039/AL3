#pragma once
#include "SceneManager.h"
#include <Windows.h>

class ClearScene : public IScene {
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;
};
