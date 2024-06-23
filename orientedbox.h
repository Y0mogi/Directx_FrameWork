#pragma once
#include "collision_base.h"
#include "component.h"

class OrientedBox :public Component,public Collision_Base
{
public:
    void Init() override;
    void Uninit() override;
    void Update() override;
    void Draw()override;
    void CompInfo() override;

    bool Intersects(Collision_Base* other) const override;
    bool Contains(Collision_Base* other)const override;

    OrientedBox() { SetCollisionType(CollisionType::OBB); }
    ~OrientedBox() override {}

    BoundingOrientedBox& GetCollision() { return _collision; }
    
private: 
    BoundingOrientedBox _collision;
    Transform*          _transform{};
    XMFLOAT3            _offset{0,2,0};

    ID3D11VertexShader* _VertexShader;
    ID3D11PixelShader*  _PixelShader;
    ID3D11InputLayout*  _VertexLayout;
};

