//=============================================================================
// Contents   : GameObject�̈ʒu�A��]�A�X�P�[���������N���X
//              
// Author     : ���� ����
// LastUpdate : 2024/06/16
// Since      : 2024/06/01
//
// === �X�V���O ===
//
// 6 / 16   Quaternion�N���X��DirectXTK��Quaternion�N���X�ɕύX
//
//
//=============================================================================

#pragma once
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "component.h"
#include "main.h"

/// <summary>
/// �I�u�W�F�N�g�̈ړ��A��]�A�X�P�[�����Ǘ�����R���|�[�l���g
/// </summary>
class Transform :public Component
{
public:

	// ==========��{�����o=========

	XMFLOAT3 position{ 0.0f,0.0f,0.0f };
    DirectX::SimpleMath::Quaternion rotation{};
	XMFLOAT3 scale{ 1.0f,1.0f,1.0f };
    
	// =============================	

public:

	// �g��������
	void Update() override {
      
	};

    void CompInfo()override {
        using namespace ImGui;
        SeparatorText("Transform");
        if (TreeNode("TMInfo")) {

            DragFloat3("Position", reinterpret_cast<float*>(&position));

            Text("Rotation is Can only display (2024/06/14)");
            
            // �N�H�[�^�j�I������]�s��ɕϊ�
            auto rotationMatrix = DirectX::SimpleMath::Matrix::CreateFromQuaternion(rotation);
            // ��]�s�񂩂�I�C���[�p���擾
            float pitch, yaw, roll;
            ExtractPitchYawRoll(rotationMatrix, pitch, yaw, roll);

            XMFLOAT3 rotF3 = XMFLOAT3(pitch, yaw, roll);
            DragFloat3("Rotation", reinterpret_cast<float*>(&rotF3));

            DragFloat3("Scale", reinterpret_cast<float*>(&scale));

            TreePop();
        }
    }

	// ===�����x�N�g���̎擾===
   
    XMFLOAT3 GetForward() const {
            return GetDirectionVector(2);
        }
    XMFLOAT3 GetBack() const {
            XMFLOAT3 back = GetDirectionVector(2);
            return XMFLOAT3(-back.x, -back.y, -back.z);
        }
    XMFLOAT3 GetRight() const {
            return GetDirectionVector(0);
        }
    XMFLOAT3 GetLeft() const {
            XMFLOAT3 left = GetDirectionVector(0);
            return XMFLOAT3(-left.x, -left.y, -left.z);
        }
    XMFLOAT3 GetTop() const {
            return GetDirectionVector(1);
        }
    XMFLOAT3 GetUnder() const {
            XMFLOAT3 under = GetDirectionVector(1);
            return XMFLOAT3(-under.x, -under.y, -under.z);
        }
    
	// ========================

	auto operator <=>(const Transform&) const = default;

private:

    // ��]�s��̍s���擾���Đ��K������w���p�[�֐�
    XMFLOAT3 GetDirectionVector(int row) const {
        XMMATRIX rotationMatrix = DirectX::SimpleMath::Matrix::CreateFromQuaternion(rotation);
        XMFLOAT3 dir;
        XMStoreFloat3(&dir, rotationMatrix.r[row]);
        return VectorNormalize(dir);
    }

    // ��]�s�񂩂�I�C���[�p�𒊏o����w���p�[�֐�
    void ExtractPitchYawRoll(const DirectX::SimpleMath::Matrix& matrix, float& pitch, float& yaw, float& roll) const {
        yaw = atan2f(matrix._23, matrix._33);
        pitch = -asinf(matrix._13);
        roll = atan2f(matrix._12, matrix._11);
    }
};

#endif // !TRANSFORM_H