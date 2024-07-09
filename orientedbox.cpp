#include "transform.h"
#include "renderer.h"
#include "orientedbox.h"

#include "modelRenderer.h"
#include "boxcollision.h"
#include "gameobject.h"

void OrientedBox::Init()
{
    _transform = Parent->GetComponent<Transform>();

    _collision = BoundingOrientedBox(_transform->position,_transform->scale,_transform->rotation);

   
    Parent->GetComponent<ModelRenderer>()->Load("asset\\model\\BoxCollision.obj");

    Renderer::CreateVertexShader(&_vertexShader, &_vertexLayout,
        "shader\\unlitTextureVS.cso");

    Renderer::CreatePixelShader(&_pixelShader,
        "shader\\unlitTexturePS.cso");
}

void OrientedBox::Uninit()
{
    _vertexShader->Release();
    _vertexLayout->Release();
    _pixelShader->Release();
}

void OrientedBox::Update()
{
    
    _collision.Center = _transform->position;
    _collision.Extents = _transform->scale;
    _collision.Orientation  = _transform->rotation;

}

void OrientedBox::Draw()
{
    using namespace DirectX::SimpleMath;
    

    // 入力レイアウト
    Renderer::GetDeviceContext()->IASetInputLayout(_vertexLayout);

    // シェーダー設定
    Renderer::GetDeviceContext()->VSSetShader(_vertexShader, NULL, 0);
    Renderer::GetDeviceContext()->PSSetShader(_pixelShader, NULL, 0);

    XMMATRIX world, scl, rot, trans;
    scl = DirectX::XMMatrixScaling(_transform->scale.x * 2, _transform->scale.y * 2, _transform->scale.z * 2);
    rot = Matrix::CreateFromQuaternion(_transform->rotation);
    trans = DirectX::XMMatrixTranslation(_collision.Center.x, _collision.Center.y, _collision.Center.z);
    world = scl * rot * trans;
    Renderer::SetWorldMatrix(world);

    Renderer::BeginWireFrame();

    Parent->GetComponent<ModelRenderer>()->DrawModel("asset\\model\\BoxCollision.obj");

    Renderer::EndWireFrame();
}

void OrientedBox::CompInfo()
{
    using namespace ImGui;
    SeparatorText("OrientedBox");

    if (TreeNode("OBBInfo")) {
        
        SeparatorText("IsHit");
        Text("%s", IsHit() ? "True" : "False");

        SeparatorText("IsActive");
        Text("%s", IsActive() ? "True" : "False");

        TreePop();
    }
}

bool OrientedBox::Intersects(Collision_Base* other) const
{
    // 相手の当たり判定の種類を取得
    switch (other->GetCollisionType()) { 
    case CollisionType::AABB: { // 相手の当たり判定の種類ががAABBの場合

        // BoxCollisionクラスにキャスト
        auto otherBox = static_cast<BoxCollision*>(other);

        // キャストできたかチェック 出来ていなかったら return false
        if (!otherBox) return false;

        // 交差しているかのチェックして結果をリターン 
        return this->_collision.Intersects(otherBox->GetCollision());
    }
    case CollisionType::OBB: { // 相手の当たり判定の種類ががOBBの場合

        // OrientedBoxクラスにキャスト
        auto otherBox = static_cast<OrientedBox*>(other);

        // キャストできたかチェック 出来ていなかったら return false
        if (!otherBox) return false;

        // 交差しているかのチェックして結果をリターン
        return this->_collision.Intersects(otherBox->GetCollision());
    }

    case CollisionType::Sphere: {
        // 未実装
        return false;
    }

    case CollisionType::Ray: {
        // 未実装
        return false;
    }
    default:
        return false;
    }
    return false;
}

bool OrientedBox::Contains(Collision_Base* other) const
{
    // 相手の当たり判定の種類を取得
    switch (other->GetCollisionType()) {    
    case CollisionType::AABB: {

        // BoxCollisionにキャスト
        auto otherBox = static_cast<BoxCollision*>(other);      

        // キャストできたかチェック
        if (!otherBox) return false;                            

        // 内包しているかのチェックして結果をリターン
        return this->_collision.Contains(otherBox->GetCollision());   
    }
    case CollisionType::OBB: {
        auto otherBox = static_cast<OrientedBox*>(other);
        if (!otherBox) return false;
        return this->_collision.Contains(otherBox->GetCollision());
    }
    case CollisionType::Sphere: {

        return false;
    }

    case CollisionType::Ray: {

        return false;
    }
    default:
        return false;
    }
    return false;
}
