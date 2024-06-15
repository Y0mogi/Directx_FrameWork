#pragma once
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "component.h"
#include "quaternion.h"
#include "main.h"


/// <summary>
/// �I�u�W�F�N�g�̈ړ��A��]�A�X�P�[�����Ǘ�����R���|�[�l���g
/// </summary>
class Transform :public Component
{
public:

	// ==========��{�����o=========
	XMFLOAT3 position{ 0.0f,0.0f,0.0f };
    Quaternion rotation;
	XMFLOAT3 scale{ 1.0f,1.0f,1.0f };
    
	// =============================	

public:

	// �g��������
	void Update() override {
      
	};

    
    void CompInfo()override {
        using namespace ImGui;
        SeparatorText("Transform");
        
        DragFloat3("Position", reinterpret_cast<float*>(&position));

        Text("Rotation is Can only display (2024/06/14)");
        XMFLOAT4 rotF4 = rotation.ToXMFloat4();
        DragFloat3("Rotation", reinterpret_cast<float*>(&rotF4));
        //rotation.FromEulerAngles(rotF4.x, rotF4.y, rotF4.z);

        DragFloat3("Scale", reinterpret_cast<float*>(&scale));
        
        Separator();
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


    // === ��] ===
    
    // �I�C���[�p�ł̉�]�̐ݒ�
    void SetEulerAngles(const XMFLOAT3& angle) {
        rotation = Quaternion::FromEulerAngles(angle.x, angle.y, angle.z);
    }
    void SetEulerAngles(float pitch, float yaw, float roll) {
        rotation = Quaternion::FromEulerAngles(pitch, yaw, roll);
    }
    
    /// <summary>
    /// �I�C���[�p�ł̉�]���擾���܂��B
    /// </summary>
    XMFLOAT3 GetEulerAngles() const {
        return ToXMFLOAT3(XMQuaternionRotationMatrix(rotation.ToRotationMatrix()));
    }

    // ���]
    void Rotate(const XMFLOAT3& eulerAngles = {}) {
        Quaternion deltaRotation = Quaternion::FromEulerAngles(eulerAngles.x, eulerAngles.y, eulerAngles.z);
        rotation = rotation * deltaRotation;
    }
    void Rotate(float pitch = 0.0f, float yaw = 0.0f, float roll = 0.0f) {
        Quaternion deltaRotation = Quaternion::FromEulerAngles(pitch, yaw, roll);
        rotation = rotation * deltaRotation;
    }

    /// ���]
    void RotateAround(const XMFLOAT3& point = {}, const XMFLOAT3& axis = {}, float angle ={}) {
        // ���s�ړ�
        XMVECTOR pointVector = XMLoadFloat3(&point);
        XMVECTOR posVector = XMLoadFloat3(&position);
        posVector -= pointVector;

        // ��]
        Quaternion deltaRotation = Quaternion::AngleAxis(angle, axis);
        posVector = XMVector3Rotate(posVector, deltaRotation.ToXMVector());

        // ���s�ړ��̕���
        posVector += pointVector;
        XMStoreFloat3(&position, posVector);

        // ��]�̓K�p
        rotation = deltaRotation * rotation;
    }
    // ========================

	auto operator <=>(const Transform&) const = default;

private:

	// ��]�s��̍s���擾���Đ��K������w���p�[�֐�
	XMFLOAT3 GetDirectionVector(int row) const {
        XMMATRIX rotationMatrix = rotation.ToRotationMatrix();
        XMFLOAT3 dir;
        XMStoreFloat3(&dir, rotationMatrix.r[row]);
        return VectorNormalize(dir);
	}
};

#endif // !TRANSFORM_H