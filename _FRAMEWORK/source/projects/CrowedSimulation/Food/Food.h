#pragma once

class Food
{
public:
	Food(Elite::Vector2 pos, int amount);
	virtual ~Food();

	void Render(float dt) const;
	
	//public functions
	int TakeFood();
	
	//getters and setters
	Elite::Vector2 GetPosition() const { return m_position; };

	int GetAmount() const { return m_amount; };
private:

	Elite::Vector2 m_position;
	int m_amount;
};