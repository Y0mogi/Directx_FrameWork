#pragma once

class ParticleBase {
public:
	virtual void Init()		{};
	virtual void Uninit()	{};
	virtual void Update()	{};
	virtual void Draw()		{};

private:
	Transform m_Transform;
	
};