#pragma once
#include "scene.h"

class Result : public Scene
{
public:
	void Init() override;
	void Uninit() override;
	void Update(const float& dt) override;
	void Draw() override;


	~Result()override {};
};

