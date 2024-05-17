#pragma once

/// <summary>
/// 位置・回転・スケールのデータ構造体
/// </summary>
struct Transform
{
	XMFLOAT3 position{};
	XMFLOAT3 rotation{};
	XMFLOAT3 scale = { 1.0f,1.0f,1.0f };

	auto operator <=>(const Transform&) const = default;
};


class GameObject
{
public:
	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	GameObject();
	GameObject(const Transform& transform);
	virtual  ~GameObject();

protected:
	Transform _transform{};
};

