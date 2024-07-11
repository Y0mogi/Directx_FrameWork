#pragma once

/// <summary>
/// パーティクルを発生させるクラス
/// </summary>
class ParticleEmitter {
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();


private:
	std::list<ParticleBase*> m_Particles;
};