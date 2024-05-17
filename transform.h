#pragma once
#include "component.h"
#include "main.h"

class Transform :public Component
{
public:
	XMFLOAT3 position{ 0.0f,0.0f,0.0f };
	XMFLOAT3 rotation{ 0.0f,0.0f,0.0f };
	XMFLOAT3 scale = { 1.0f,1.0f,1.0f };
	
	auto operator <=>(const Transform&) const = default;
};