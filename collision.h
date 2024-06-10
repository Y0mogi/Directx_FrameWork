#pragma once

#include "component.h"
#include "gameobject.h"
#include "transform.h"
#include <DirectXMath.h>
#include <algorithm>

class OBBComponent : public Component
{
public:
    XMFLOAT3 center;     // OBB�̒��S
    XMFLOAT3 extents;    // �e���ɉ��������a�x�N�g��
    XMFLOAT3X3 axis;     // OBB�̉�]�s��i�e���̕����j

    OBBComponent() : center(0.0f,0.0f,0.0f), extents(1.0f, 1.0f, 1.0f) {
        XMStoreFloat3x3(&axis, XMMatrixIdentity());
    }

    void Init(){};
    void Uninit() {};
    void Update() 
    {
        center = Parent->GetComponent<Transform>()->position;
    };
    void Draw() {};

    bool CheckCollision(const OBBComponent& other) const {
        // OBB�̏Փ˔��������
        return OBBvsOBB(*this, other);
    }

private:
    bool OBBvsOBB(const OBBComponent& a, const OBBComponent& b) const {
        // OBB���m�̏Փ˔�����s��

        // �e���̃Z�b�g
        XMVECTOR A0 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&a.axis.m[0]));
        XMVECTOR A1 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&a.axis.m[1]));
        XMVECTOR A2 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&a.axis.m[2]));

        XMVECTOR B0 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&b.axis.m[0]));
        XMVECTOR B1 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&b.axis.m[1]));
        XMVECTOR B2 = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&b.axis.m[2]));

        // ���S�ԃx�N�g��
        XMVECTOR D = XMLoadFloat3(&b.center) - XMLoadFloat3(&a.center);

        // �e���ɓ��e���ꂽ���a�̘a
        float ra, rb;
        XMFLOAT3 axis;

        // a�̊e���ɂ��Ă̌���
        for (int i = 0; i < 3; ++i) {
            axis = XMFLOAT3(a.axis.m[i][0], a.axis.m[i][1], a.axis.m[i][2]);
            ra = a.extents.x * fabsf(axis.x) + a.extents.y * fabsf(axis.y) + a.extents.z * fabsf(axis.z);
            rb = b.extents.x * fabsf(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&axis), B0))) +
                b.extents.y * fabsf(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&axis), B1))) +
                b.extents.z * fabsf(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&axis), B2)));
            if (fabsf(XMVectorGetX(XMVector3Dot(D, XMLoadFloat3(&axis)))) > ra + rb) return false;
        }

        // b�̊e���ɂ��Ă̌���
        for (int i = 0; i < 3; ++i) {
            axis = XMFLOAT3(b.axis.m[i][0], b.axis.m[i][1], b.axis.m[i][2]);
            ra = a.extents.x * fabsf(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&axis), A0))) +
                a.extents.y * fabsf(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&axis), A1))) +
                a.extents.z * fabsf(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&axis), A2)));
            rb = b.extents.x * fabsf(axis.x) + b.extents.y * fabsf(axis.y) + b.extents.z * fabsf(axis.z);
            if (fabsf(XMVectorGetX(XMVector3Dot(D, XMLoadFloat3(&axis)))) > ra + rb) return false;
        }

        // a��b�̌������鎲�ɂ��Ă̌���
        XMVECTOR axisA[3] = { A0, A1, A2 };
        XMVECTOR axisB[3] = { B0, B1, B2 };

        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                // ���̊O��
                XMVECTOR cross = XMVector3Cross(axisA[i], axisB[j]);

                if (XMVector3Equal(cross, XMVectorZero())) continue;  // �O�ς��[���i�������s�j�̏ꍇ���X�L�b�v

                axis = XMFLOAT3(XMVectorGetX(cross), XMVectorGetY(cross), XMVectorGetZ(cross));
                ra = a.extents.x * fabsf(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&axis), A0))) +
                    a.extents.y * fabsf(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&axis), A1))) +
                    a.extents.z * fabsf(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&axis), A2)));
                rb = b.extents.x * fabsf(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&axis), B0))) +
                    b.extents.y * fabsf(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&axis), B1))) +
                    b.extents.z * fabsf(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&axis), B2)));
                if (fabsf(XMVectorGetX(XMVector3Dot(D, XMLoadFloat3(&axis)))) > ra + rb) return false;
            }
        }

        return true;
    }
};
