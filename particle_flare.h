#pragma once
#include "particle_base.h"

class Particle_flare : public Particle_Base
{
public:
	void Update(const float& dt) override;

	using Particle_Base::Particle_Base;
	~Particle_flare()override {};
};

