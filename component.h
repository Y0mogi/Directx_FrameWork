#pragma once

class GameObject;

class Component
{
public:
	virtual void Init() {};
	virtual void Uninit() {};
	virtual void Update() {};
	virtual void Draw() {};

	Component() {};
	virtual ~Component(){}

public:
	GameObject* Parent = nullptr;
};