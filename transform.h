#pragma once
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "component.h"
#include "main.h"
#include "XMFLOAT_Helper.h"

/// <summary>
/// �I�u�W�F�N�g�̈ړ��A��]�A�X�P�[�����Ǘ�����R���|�[�l���g
/// </summary>
class Transform :public Component
{
public:

	// ==========��{�����o=========
	XMFLOAT3 position{ 0.0f,0.0f,0.0f };
	XMFLOAT3 rotation{ 0.0f,0.0f,0.0f };
	XMFLOAT3 scale{ 1.0f,1.0f,1.0f };
	// =============================


public:

	// �g��������
	void Update() override {
		

	};

	// ===�����x�N�g���̎擾===
	const XMFLOAT3 GetForward() const {
		// �I�C���[�p�����]�s��𐶐�
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw
		(this->rotation.x, this->rotation.y, this->rotation.z);

		XMFLOAT3 foward;
		// ��]�s���3�s�ڂ���Z�������x�N�g�������o��
		XMStoreFloat3(&foward, rotationMatrix.r[2]);

		// ���K�����Ă���
		
		return VectorNormalize(foward);
	};
	const XMFLOAT3 GetBack() const {
		// �I�C���[�p�����]�s��𐶐�
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw
		(this->rotation.x, this->rotation.y, this->rotation.z);

		XMFLOAT3 back;
		// ��]�s���3�s�ڂ���Z�������x�N�g�������o��
		XMStoreFloat3(&back, rotationMatrix.r[2]);

		
		return -VectorNormalize(back);
	};
	const XMFLOAT3 GetRight() const {
		// �I�C���[�p�����]�s��𐶐�
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw
		(this->rotation.x, this->rotation.y, this->rotation.z);

		XMFLOAT3 right;
		// ��]�s���0�s�ڂ���X�������x�N�g�������o��
		XMStoreFloat3(&right, rotationMatrix.r[0]);

		

		return VectorNormalize(right);
	};
	const XMFLOAT3 GetLeft() const {
		// �I�C���[�p�����]�s��𐶐�
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw
		(this->rotation.x, this->rotation.y, this->rotation.z);

		XMFLOAT3 left;
		// ��]�s���0�s�ڂ���X�������x�N�g�������o��
		XMStoreFloat3(&left, rotationMatrix.r[0]);

		

		return -VectorNormalize(left);
	};
	const XMFLOAT3 GetTop() const {
		// �I�C���[�p�����]�s��𐶐�
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw
		(this->rotation.x, this->rotation.y, this->rotation.z);

		XMFLOAT3 top;
		// ��]�s���2�s�ڂ���Y�������x�N�g�������o��
		XMStoreFloat3(&top, rotationMatrix.r[1]);

		

		return 
	};
	const XMFLOAT3 GetUnder() const {
		// �I�C���[�p�����]�s��𐶐�
		XMMATRIX rotationMatrix;
		rotationMatrix = XMMatrixRotationRollPitchYaw
		(this->rotation.x, this->rotation.y, this->rotation.z);

		XMFLOAT3 under;
		// ��]�s���2�s�ڂ���Y�������x�N�g�������o��
		XMStoreFloat3(&under, rotationMatrix.r[1]);

		

		return -VectorNormalize(under);
	};
	// ========================

	
	auto operator <=>(const Transform&) const = default;
};

#endif // !TRANSFORM_H