#include "transform.h"
#include "renderer.h"
#include "orientedbox.h"

#include "modelRenderer.h"
#include "boxcollision.h"
#include "gameobject.h"

void OrientedBox::Init()
{
    m_Transform = Parent->GetComponent<Transform>();

    m_Collision = BoundingOrientedBox(m_Transform->position,m_Transform->scale,m_Transform->rotation);

   
    Parent->GetComponent<ModelRenderer>()->Load("asset\\model\\BoxCollision.obj");

    Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
        "shader\\unlitTextureVS.cso");

    Renderer::CreatePixelShader(&m_PixelShader,
        "shader\\unlitTexturePS.cso");
}

void OrientedBox::Uninit()
{
    m_VertexShader->Release();
    m_VertexLayout->Release();
    m_PixelShader->Release();
}

void OrientedBox::Update(const float& dt)
{
    
    m_Collision.Center = m_Transform->position;
    if (m_transform) {
        m_Collision.Extents = m_Transform->scale;
    }
    else {
        m_Collision.Extents = m_Scale;
    }
   
    m_Collision.Orientation  = m_Transform->rotation;

}

void OrientedBox::Draw()
{
    using namespace DirectX::SimpleMath;
    

    // 入力レイアウト
    Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

    // シェーダー設定
    Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
    Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

    XMMATRIX world, scl, rot, trans;
    scl = DirectX::XMMatrixScaling(m_Collision.Extents.x * 2, m_Collision.Extents.y * 2, m_Collision.Extents.z * 2);
    rot = Matrix::CreateFromQuaternion(m_Collision.Orientation);
    trans = DirectX::XMMatrixTranslation(m_Collision.Center.x, m_Collision.Center.y, m_Collision.Center.z);
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
        return this->m_Collision.Intersects(otherBox->GetCollision());
    }
    case CollisionType::OBB: { // 相手の当たり判定の種類ががOBBの場合

        // OrientedBoxクラスにキャスト
        auto otherBox = static_cast<OrientedBox*>(other);

        // キャストできたかチェック 出来ていなかったら return false
        if (!otherBox) return false;

        // 交差しているかのチェックして結果をリターン
        return this->m_Collision.Intersects(otherBox->GetCollision());
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
        return this->m_Collision.Contains(otherBox->GetCollision());   
    }
    case CollisionType::OBB: {
        auto otherBox = static_cast<OrientedBox*>(other);
        if (!otherBox) return false;
        return this->m_Collision.Contains(otherBox->GetCollision());
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
