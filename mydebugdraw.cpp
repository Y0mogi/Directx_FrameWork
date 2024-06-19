#include "mydebugdraw.h"
#include <algorithm>

#include "main.h"
#include "renderer.h"



using namespace DirectX;

namespace
{
    inline void DrawCube(ID3D11Buffer* _vertex,
        ID3D11InputLayout* layout,
        ID3D11VertexShader* vers,
        CXMMATRIX matWorld,
        FXMVECTOR color)
    {
        static const XMVECTORF32 s_verts[8] =
        {
            { { { -1.f, -1.f, -1.f, 0.f } } },
            { { {  1.f, -1.f, -1.f, 0.f } } },
            { { {  1.f, -1.f,  1.f, 0.f } } },
            { { { -1.f, -1.f,  1.f, 0.f } } },
            { { { -1.f,  1.f, -1.f, 0.f } } },
            { { {  1.f,  1.f, -1.f, 0.f } } },
            { { {  1.f,  1.f,  1.f, 0.f } } },
            { { { -1.f,  1.f,  1.f, 0.f } } }
        };

        D3D11_MAPPED_SUBRESOURCE msr;
        Renderer::GetDeviceContext()->Map(_vertex, 0,
            D3D11_MAP_WRITE_DISCARD, 0, &msr);

        VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

        for (size_t i = 0; i < 8; i++) {
            vertex[i].Position = ToXMFLOAT3(s_verts[i]);
        }
        
        static const UINT s_indices[] = {
            0, 1,
            1, 2,
            2, 3,
            3, 0,
            4, 5,
            5, 6,
            6, 7,
            7, 4,
            0, 4,
            1, 5,
            2, 6,
            3, 7
        };

        for (size_t i = 0; i < 8; ++i)
        {
            const XMVECTOR v = XMVector3Transform(s_verts[i], matWorld);
            XMStoreFloat3(&vertex[i].Position, v);
            vertex[i].Normal = { 0.0,0.0,0.0 };
            vertex[i].TexCoord = { 0.0,0.0 };
            XMStoreFloat4(&vertex[i].Diffuse, color);
        }

        vertex[0].Position = vertex[1].Position;
        vertex[1].Position = vertex[2].Position;
        vertex[2].Position = vertex[3].Position;
        vertex[3].Position = vertex[0].Position;
        vertex[4].Position = vertex[5].Position;
        vertex[5].Position = vertex[6].Position;
        vertex[6].Position = vertex[7].Position;
        vertex[7].Position = vertex[4].Position;
        vertex[0].Position = vertex[4].Position;
        vertex[1].Position = vertex[5].Position;
        vertex[2].Position = vertex[6].Position;
        vertex[3].Position = vertex[7].Position;

        Renderer::GetDeviceContext()->Unmap(_vertex, 0);

        // 入力レイアウト
        Renderer::GetDeviceContext()->IASetInputLayout(layout);

        // シェーダー設定
        Renderer::GetDeviceContext()->VSSetShader(vers, NULL, 0);


        // 頂点バッファ設定
        UINT stride = sizeof(VERTEX_3D);
        UINT offset = 0;

        // material設定
        MATERIAL material;
        ZeroMemory(&material, sizeof(material));
        material.Diffuse = ToXMFLOAT4(color);
        material.TextureEnable = true;
        Renderer::SetMaterial(material);

        Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &_vertex, &stride, &offset);

        // プリミティブトポロジ設定
        Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

        // ポリゴン描画
        Renderer::GetDeviceContext()->Draw(8,0);
       
    }
}

void DEBUG::Draw(const DirectX::BoundingSphere& sphere, DirectX::FXMVECTOR color)
{
		
}

void DEBUG::Draw(const DirectX::BoundingBox& box, DirectX::FXMVECTOR color)
{

    ID3D11Buffer* vertexB;
    ID3D11InputLayout* layout;
    ID3D11PixelShader* pix;
    ID3D11VertexShader* ver;


	XMMATRIX matWorld = XMMatrixScaling(box.Extents.x, box.Extents.y, box.Extents.z);
	const XMVECTOR position = XMLoadFloat3(&box.Center);
	matWorld.r[3] = XMVectorSelect(matWorld.r[3], position, g_XMSelect1110);

    Renderer::CreateVertexShader(&ver, &layout,
        "shader\\unlitTextureVS.cso");

    Renderer::CreatePixelShader(&pix,
        "shader\\unlitTexturePS.cso");

    VERTEX_3D vertex[8]{};

    // 頂点バッファ生成
    D3D11_BUFFER_DESC bd{};
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(VERTEX_3D) * 8;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    D3D11_SUBRESOURCE_DATA sd{};
    ZeroMemory(&sd, sizeof(sd));
    sd.pSysMem = vertex;

    Renderer::GetDevice()->CreateBuffer(&bd, &sd, &vertexB);


    DrawCube(vertexB,layout,ver,matWorld,color);
}

void DEBUG::Draw(const DirectX::BoundingOrientedBox& obb, DirectX::FXMVECTOR color)
{
	
}


