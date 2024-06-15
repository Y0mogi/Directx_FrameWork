#pragma once
#ifndef COMPONENT_H
#define COMPONENT_H


class GameObject;


/// <summary>
/// �R���|�[�l���g���N���X
/// </summary>
class Component
{
public:
	virtual void Init() {};
	virtual void Uninit() {};
	virtual void Update() {};
	virtual void Draw() {};

	/// <summary>
	/// ImGui�ŃR���|�[�l���g���̏���\��
	/// </summary>
	virtual void CompInfo() {};

	Component() {};
	virtual ~Component(){}

public:
	GameObject* Parent = nullptr;
};

#endif // COMPONENT_H