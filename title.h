#pragma once
#include "scene.h"

class Title : public Scene
{
public:
	void Init() override;
	void Uninit() override;
	void Update(const float& dt) override;
	void Draw() override;


	~Title()override{};
};

