#pragma once
#include "missile_base.h"
#include "component.h"

class NormalMissile : public Missile_Base {
public:
	void Init() override;
	void Uninit() override;
	void Update(const float& dt) override;
	void Draw() override;

	void CompInfo() override;


	using Missile_Base::Missile_Base;
	~NormalMissile() {};
};