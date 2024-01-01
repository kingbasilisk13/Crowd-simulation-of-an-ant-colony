#pragma once

class Food
{
public:
	Food(Elite::Vector2 pos, int amount);
	virtual ~Food();

	void Render(float dt) const;
	
	int GetAmount() const { return m_amount; }

	int TakeFood();
	
	Elite::Vector2 GetPosition() const { return m_position; }
private:

	Elite::Vector2 m_position;
	int m_amount;
};