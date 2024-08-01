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

	/// <summary>
	/// ������
	/// </summary>
	virtual void Init() {};

	/// <summary>
	/// �I��
	/// </summary>
	virtual void Uninit() {};

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="dt">1�t���[��������̎���</param>
	virtual void Update(const float& dt) {};

	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw() {};

	/// <summary>
	/// ImGui�ŃR���|�[�l���g���̏���\��
	/// </summary>
	virtual void CompInfo() {};

	Component() {};
	virtual ~Component(){}

public:
	GameObject* Parent = nullptr;	// �A�^�b�`����Ă���I�u�W�F�N�g
};

#endif // COMPONENT_H