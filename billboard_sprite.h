#pragma once

class billboard_sprite
{
public:
	static void Init();
	static void Uninit();
	
	static void DrawBillboard(int texNo,const XMFLOAT3& pos, float Width, float Height, float U, float V, float UW, float VH);

private:
	static ID3D11Buffer* m_VertexBuffer;
};

