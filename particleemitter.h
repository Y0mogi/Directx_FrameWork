#pragma once
#include "component.h"

/// <summary>
/// �p�[�e�B�N���𔭐�������N���X
/// </summary>
class ParticleEmitter : public Component{
public:
	void Init() override;
	void Uninit()override;
	void Update()override;
	void Draw()	override;
private:
	static const unsigned short PARTICLE_MAX = 1000;			// �p�[�e�B�N���ő吔
	std::array<class Particle_Base*, PARTICLE_MAX> m_Particles;	// �p�[�e�B�N��

	ID3D11Buffer*				m_VertexBuffer{};	// ���_�o�b�t�@
	ID3D11ShaderResourceView*	m_Texture{};		// �e�N�X�`�����\�[�X
	ID3D11VertexShader*			m_VertexShader{};	// ���_�V�F�[�_�[
	ID3D11PixelShader*			m_PixelShader{};	// �s�N�Z���V�F�[�_�[
	ID3D11InputLayout*			m_VertexLayout{};	// ���_���C�A�E�g
};