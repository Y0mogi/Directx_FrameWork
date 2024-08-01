#pragma once
#include "collision_base.h"
#include "component.h"

class OrientedBox :public Component, public Collision_Base
{
public:
	// ������
	void Init()   override;

	// �X�V
	void Uninit() override;

	// �J��
	void Update(const float& dt) override;

	// �`��
	void Draw()   override;

	// �R���|�[�l���g�̏��\��
	void CompInfo() override;   

	/// <summary>
	/// ���g�Ƃ̓����蔻��̌�������
	/// </summary>
	/// <param name="other">���肵���������蔻��̃|�C���^</param>
	/// <returns>True:�������Ă��� False:�������Ă��Ȃ�</returns>
	bool Intersects(Collision_Base* other) const override;

	/// <summary>
	/// ���g�Ƃ̓����蔻��̂̓����
	/// </summary>
	/// <param name="other">���肵���������蔻��̃|�C���^</param>
	/// <returns>True:����Ă��� False:����Ă��Ȃ�</returns>
	bool Contains(Collision_Base* other) const override;

	// gettter

	inline BoundingOrientedBox& GetCollision() { return m_Collision; }


	// �R���X�g���N�^�E�f�X�g���N�^
	inline OrientedBox() { SetCollisionType(CollisionType::OBB); }
	inline ~OrientedBox() override {}

	
	
private: 
	BoundingOrientedBox m_Collision;
	class Transform*    m_Transform{};
	
	// �f�o�b�O�\���p

	ID3D11VertexShader* m_VertexShader{};
	ID3D11InputLayout* m_VertexLayout{};
	ID3D11PixelShader* m_PixelShader{};
	
};

