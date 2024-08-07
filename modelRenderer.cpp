
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include "main.h"
#include "renderer.h"
#include "modelRenderer.h"
#include <iostream>



std::unordered_map<std::string, MODEL*> ModelRenderer::m_ModelPool;


void ModelRenderer::DrawModel()
{
    // ���_�o�b�t�@�ݒ�
    UINT stride = sizeof(VERTEX_3D);
    UINT offset = 0;
    Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_Model->VertexBuffer, &stride, &offset);

    // �C���f�b�N�X�o�b�t�@�ݒ�
    Renderer::GetDeviceContext()->IASetIndexBuffer(m_Model->IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // �v���~�e�B�u�g�|���W�ݒ�
    Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    for (unsigned int i = 0; i < m_Model->SubsetNum; i++)
    {
        // �}�e���A���ݒ�
        Renderer::SetMaterial(m_Model->SubsetArray[i].Material.Material);

        // �e�N�X�`���ݒ�
        if (m_Model->SubsetArray[i].Material.Texture) {
            Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Model->SubsetArray[i].Material.Texture);
            std::cout << "Texture set for subset " << i << std::endl;
        }
        else {
            std::cout << "No texture set for subset " << i << std::endl;
        }

        // �|���S���`��
        Renderer::GetDeviceContext()->DrawIndexed(m_Model->SubsetArray[i].IndexNum, m_Model->SubsetArray[i].StartIndex, 0);
    }
}

void ModelRenderer::DrawModel(const char* modelName)
{
    // �w�肳�ꂽ���f�������[�h����Ă��邩�m�F
    if (m_ModelPool.count(modelName) == 0)
    {
        return;
    }

    // ���f���̃|�C���^���擾
    MODEL* model = m_ModelPool[modelName];

    // ���_�o�b�t�@�ݒ�
    UINT stride = sizeof(VERTEX_3D);
    UINT offset = 0;
    Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &model->VertexBuffer, &stride, &offset);

    // �C���f�b�N�X�o�b�t�@�ݒ�
    Renderer::GetDeviceContext()->IASetIndexBuffer(model->IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // �v���~�e�B�u�g�|���W�ݒ�
    Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // �T�u�Z�b�g���Ƃɕ`��
    for (unsigned int i = 0; i < model->SubsetNum; i++)
    {
        // �}�e���A���ݒ�
        Renderer::SetMaterial(model->SubsetArray[i].Material.Material);

        // �e�N�X�`���ݒ�
        if (model->SubsetArray[i].Material.Texture)
        {
            Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &model->SubsetArray[i].Material.Texture);
            std::cout << "Texture set for subset " << i << " of model " << modelName << std::endl;
        }
        else
        {
            std::cout << "No texture set for subset " << i << " of model " << modelName << std::endl;
        }

        // �|���S���`��
        Renderer::GetDeviceContext()->DrawIndexed(model->SubsetArray[i].IndexNum, model->SubsetArray[i].StartIndex, 0);
    }
}


void ModelRenderer::Preload(const char* FileName)
{
    if (m_ModelPool.count(FileName) > 0)
    {
        return;
    }

    MODEL* model = new MODEL;
    LoadModel(FileName, model);

    m_ModelPool[FileName] = model;
}

void ModelRenderer::UnloadAll()
{
    for (std::pair<const std::string, MODEL*> pair : m_ModelPool)
    {
        pair.second->VertexBuffer->Release();
        pair.second->IndexBuffer->Release();

        for (unsigned int i = 0; i < pair.second->SubsetNum; i++)
        {
            if (pair.second->SubsetArray[i].Material.Texture)
                pair.second->SubsetArray[i].Material.Texture->Release();
        }

        delete[] pair.second->SubsetArray;
        delete pair.second;
    }

    m_ModelPool.clear();
}

void ModelRenderer::Load(const char* FileName)
{
    if (m_ModelPool.count(FileName) > 0)
    {
        m_Model = m_ModelPool[FileName];
        return;
    }

    m_Model = new MODEL;
    LoadModel(FileName, m_Model);

    m_ModelPool[FileName] = m_Model;
}

void ModelRenderer::LoadModel(const char* FileName, MODEL* Model)
{
    MODEL_OBJ modelObj;
    LoadObj(FileName, &modelObj);

    // ���_�o�b�t�@����
    {
        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(bd));
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(VERTEX_3D) * modelObj.VertexNum;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = 0;

        D3D11_SUBRESOURCE_DATA sd;
        ZeroMemory(&sd, sizeof(sd));
        sd.pSysMem = modelObj.VertexArray;

        HRESULT hr = Renderer::GetDevice()->CreateBuffer(&bd, &sd, &Model->VertexBuffer);
        if (FAILED(hr)) {
            std::cout << "Failed to create vertex buffer" << std::endl;
        }
    }

    // �C���f�b�N�X�o�b�t�@����
    {
        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(bd));
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(unsigned int) * modelObj.IndexNum;
        bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bd.CPUAccessFlags = 0;

        D3D11_SUBRESOURCE_DATA sd;
        ZeroMemory(&sd, sizeof(sd));
        sd.pSysMem = modelObj.IndexArray;

        HRESULT hr = Renderer::GetDevice()->CreateBuffer(&bd, &sd, &Model->IndexBuffer);
        if (FAILED(hr)) {
            std::cout << "Failed to create index buffer" << std::endl;
        }
    }

    // �T�u�Z�b�g�ݒ�
    {
        Model->SubsetArray = new SUBSET[modelObj.SubsetNum];
        Model->SubsetNum = modelObj.SubsetNum;

        for (unsigned int i = 0; i < modelObj.SubsetNum; i++)
        {
            Model->SubsetArray[i].StartIndex = modelObj.SubsetArray[i].StartIndex;
            Model->SubsetArray[i].IndexNum = modelObj.SubsetArray[i].IndexNum;

            Model->SubsetArray[i].Material.Material = modelObj.SubsetArray[i].Material.Material;

            Model->SubsetArray[i].Material.Texture = nullptr;

            // �e�N�X�`���ǂݍ���
            TexMetadata metadata;
            ScratchImage image;
            wchar_t wc[256];
            mbstowcs(wc, modelObj.SubsetArray[i].Material.TextureName, sizeof(wc));

            std::wcout << L"Attempting to load texture: " << wc << std::endl;
            HRESULT hr = LoadFromWICFile(wc, WIC_FLAGS_NONE, &metadata, image);
            if (SUCCEEDED(hr)) {
                hr = CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &Model->SubsetArray[i].Material.Texture);
                if (SUCCEEDED(hr)) {
                    std::cout << "Successfully loaded texture: " << modelObj.SubsetArray[i].Material.TextureName << std::endl;
                }
                else {
                    std::cout << "Failed to create shader resource view for texture: " << modelObj.SubsetArray[i].Material.TextureName << std::endl;
                }
            }
            else {
                std::wcout << L"Failed to load texture: " << wc << std::endl;
            }

            if (Model->SubsetArray[i].Material.Texture)
                Model->SubsetArray[i].Material.Material.TextureEnable = true;
            else
                Model->SubsetArray[i].Material.Material.TextureEnable = false;

            std::cout << "Subset " << i << " uses texture " << modelObj.SubsetArray[i].Material.TextureName << std::endl;
        }
    }

    delete[] modelObj.VertexArray;
    delete[] modelObj.IndexArray;
    delete[] modelObj.SubsetArray;
}




// ���f���Ǎ�
void ModelRenderer::LoadObj(const char* FileName, MODEL_OBJ* ModelObj)
{
    char dir[MAX_PATH];
    strcpy(dir, FileName);
    PathRemoveFileSpec(dir);

    XMFLOAT3* positionArray;
    XMFLOAT3* normalArray;
    XMFLOAT2* texcoordArray;

    unsigned int positionNum = 0;
    unsigned int normalNum = 0;
    unsigned int texcoordNum = 0;
    unsigned int vertexNum = 0;
    unsigned int indexNum = 0;
    unsigned int in = 0;
    unsigned int subsetNum = 0;

    MODEL_MATERIAL* materialArray = nullptr;
    unsigned int materialNum = 0;

    char str[256];
    char* s;
    char c;

    FILE* file;
    file = fopen(FileName, "rt");
    assert(file);

    // �v�f���J�E���g
    while (true)
    {
        fscanf(file, "%s", str);

        if (feof(file) != 0)
            break;

        if (strcmp(str, "v") == 0)
        {
            positionNum++;
        }
        else if (strcmp(str, "vn") == 0)
        {
            normalNum++;
        }
        else if (strcmp(str, "vt") == 0)
        {
            texcoordNum++;
        }
        else if (strcmp(str, "usemtl") == 0)
        {
            subsetNum++;
        }
        else if (strcmp(str, "f") == 0)
        {
            in = 0;

            do
            {
                fscanf(file, "%s", str);
                vertexNum++;
                in++;
                c = fgetc(file);
            } while (c != '\n' && c != '\r');

            // �l�p�͎O�p�ɕ���
            if (in == 4)
                in = 6;

            indexNum += in;
        }
    }

    // �������m��
    positionArray = new XMFLOAT3[positionNum];
    normalArray = new XMFLOAT3[normalNum];
    texcoordArray = new XMFLOAT2[texcoordNum];

    ModelObj->VertexArray = new VERTEX_3D[vertexNum];
    ModelObj->VertexNum = vertexNum;

    ModelObj->IndexArray = new unsigned int[indexNum];
    ModelObj->IndexNum = indexNum;

    ModelObj->SubsetArray = new SUBSET[subsetNum];
    ModelObj->SubsetNum = subsetNum;

    // �v�f�Ǎ�
    XMFLOAT3* position = positionArray;
    XMFLOAT3* normal = normalArray;
    XMFLOAT2* texcoord = texcoordArray;

    unsigned int vc = 0;
    unsigned int ic = 0;
    unsigned int sc = 0;

    fseek(file, 0, SEEK_SET);

    while (true)
    {
        fscanf(file, "%s", str);

        if (feof(file) != 0)
            break;

        if (strcmp(str, "mtllib") == 0)
        {
            // �}�e���A���t�@�C��
            fscanf(file, "%s", str);

            char path[256];
            strcpy(path, dir);
            strcat(path, "\\");
            strcat(path, str);

            LoadMaterial(path, &materialArray, &materialNum);
        }
        else if (strcmp(str, "o") == 0)
        {
            // �I�u�W�F�N�g��
            fscanf(file, "%s", str);
        }
        else if (strcmp(str, "v") == 0)
        {
            // ���_���W
            fscanf(file, "%f", &position->x);
            fscanf(file, "%f", &position->y);
            fscanf(file, "%f", &position->z);
            position++;
        }
        else if (strcmp(str, "vn") == 0)
        {
            // �@��
            fscanf(file, "%f", &normal->x);
            fscanf(file, "%f", &normal->y);
            fscanf(file, "%f", &normal->z);
            normal++;
        }
        else if (strcmp(str, "vt") == 0)
        {
            // �e�N�X�`�����W
            fscanf(file, "%f", &texcoord->x);
            fscanf(file, "%f", &texcoord->y);
            texcoord->x = 1.0f - texcoord->x;
            texcoord->y = 1.0f - texcoord->y;
            texcoord++;
        }
        else if (strcmp(str, "usemtl") == 0)
        {
            // �}�e���A��
            fscanf(file, "%s", str);

            if (sc != 0)
                ModelObj->SubsetArray[sc - 1].IndexNum = ic - ModelObj->SubsetArray[sc - 1].StartIndex;

            ModelObj->SubsetArray[sc].StartIndex = ic;

            for (unsigned int i = 0; i < materialNum; i++)
            {
                if (strcmp(str, materialArray[i].Name) == 0)
                {
                    ModelObj->SubsetArray[sc].Material.Material = materialArray[i].Material;
                    strcpy(ModelObj->SubsetArray[sc].Material.TextureName, materialArray[i].TextureName);
                    strcpy(ModelObj->SubsetArray[sc].Material.Name, materialArray[i].Name);

                    std::cout << "Using material " << materialArray[i].Name << " with texture " << materialArray[i].TextureName << std::endl;
                    break;
                }
            }

            sc++;
        }
        else if (strcmp(str, "f") == 0)
        {
            // ��
            in = 0;

            do
            {
                fscanf(file, "%s", str);

                s = strtok(str, "/");
                ModelObj->VertexArray[vc].Position = positionArray[atoi(s) - 1];
                if (s[strlen(s) + 1] != '/')
                {
                    // �e�N�X�`�����W�����݂��Ȃ��ꍇ������
                    s = strtok(nullptr, "/");
                    ModelObj->VertexArray[vc].TexCoord = texcoordArray[atoi(s) - 1];
                }
                s = strtok(nullptr, "/");
                ModelObj->VertexArray[vc].Normal = normalArray[atoi(s) - 1];

                ModelObj->VertexArray[vc].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

                ModelObj->IndexArray[ic] = vc;
                ic++;
                vc++;

                in++;
                c = fgetc(file);
            } while (c != '\n' && c != '\r');

            // �l�p�͎O�p�ɕ���
            if (in == 4)
            {
                ModelObj->IndexArray[ic] = vc - 4;
                ic++;
                ModelObj->IndexArray[ic] = vc - 2;
                ic++;
            }
        }
    }

    if (sc != 0)
        ModelObj->SubsetArray[sc - 1].IndexNum = ic - ModelObj->SubsetArray[sc - 1].StartIndex;

    fclose(file);

    delete[] positionArray;
    delete[] normalArray;
    delete[] texcoordArray;
    delete[] materialArray;
}


// �}�e���A���ǂݍ���
void ModelRenderer::LoadMaterial(const char* FileName, MODEL_MATERIAL** MaterialArray, unsigned int* MaterialNum)
{
    char dir[MAX_PATH];
    strcpy(dir, FileName);
    PathRemoveFileSpec(dir);

    char str[256];

    FILE* file;
    file = fopen(FileName, "rt");
    assert(file);

    MODEL_MATERIAL* materialArray;
    unsigned int materialNum = 0;

    // �v�f���J�E���g
    while (true)
    {
        fscanf(file, "%s", str);

        if (feof(file) != 0)
            break;

        if (strcmp(str, "newmtl") == 0)
        {
            materialNum++;
        }
    }

    // �������m��
    materialArray = new MODEL_MATERIAL[materialNum];

    // �v�f�Ǎ�
    int mc = -1;

    fseek(file, 0, SEEK_SET);

    while (true)
    {
        fscanf(file, "%s", str);

        if (feof(file) != 0)
            break;

        if (strcmp(str, "newmtl") == 0)
        {
            // �}�e���A����
            mc++;
            fscanf(file, "%s", materialArray[mc].Name);
            strcpy(materialArray[mc].TextureName, "");

            materialArray[mc].Material.Emission.x = 0.0f;
            materialArray[mc].Material.Emission.y = 0.0f;
            materialArray[mc].Material.Emission.z = 0.0f;
            materialArray[mc].Material.Emission.w = 0.0f;
        }
        else if (strcmp(str, "Ka") == 0)
        {
            // �A���r�G���g
            fscanf(file, "%f", &materialArray[mc].Material.Ambient.x);
            fscanf(file, "%f", &materialArray[mc].Material.Ambient.y);
            fscanf(file, "%f", &materialArray[mc].Material.Ambient.z);
            materialArray[mc].Material.Ambient.w = 1.0f;
        }
        else if (strcmp(str, "Kd") == 0)
        {
            // �f�B�t���[�Y
            fscanf(file, "%f", &materialArray[mc].Material.Diffuse.x);
            fscanf(file, "%f", &materialArray[mc].Material.Diffuse.y);
            fscanf(file, "%f", &materialArray[mc].Material.Diffuse.z);
            materialArray[mc].Material.Diffuse.w = 1.0f;
        }
        else if (strcmp(str, "Ks") == 0)
        {
            // �X�y�L����
            fscanf(file, "%f", &materialArray[mc].Material.Specular.x);
            fscanf(file, "%f", &materialArray[mc].Material.Specular.y);
            fscanf(file, "%f", &materialArray[mc].Material.Specular.z);
            materialArray[mc].Material.Specular.w = 1.0f;
        }
        else if (strcmp(str, "Ns") == 0)
        {
            // �X�y�L�������x
            fscanf(file, "%f", &materialArray[mc].Material.Shininess);
        }
        else if (strcmp(str, "d") == 0)
        {
            // �A���t�@
            fscanf(file, "%f", &materialArray[mc].Material.Diffuse.w);
        }
        else if (strcmp(str, "map_Kd") == 0)
        {
            // �e�N�X�`��
            fscanf(file, "%s", str);

            char path[256];
            strcpy(path, dir);
            strcat(path, "\\");
            strcat(path, str);

            strcpy(materialArray[mc].TextureName, path);

            std::cout << "Material " << materialArray[mc].Name << " has texture " << materialArray[mc].TextureName << std::endl;
        }
    }

    fclose(file);

    *MaterialArray = materialArray;
    *MaterialNum = materialNum;
}

