#pragma once
#ifndef COMPONENT_H
#define COMPONENT_H


class GameObject;

/// <summary>
/// コンポーネント基底クラス
/// </summary>
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

#endif // COMPONENT_H