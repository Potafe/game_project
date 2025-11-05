#pragma once
#include <vector>

class Health {
public:
    Health();
    Health(int maxHearts);

    void SetMaxHearts(int maxHearts);
    int GetMaxHearts() const { return m_maxHearts; }

    void SetCurrentHearts(int maxHearts);
    int GetCurrentHearts() const { return m_currentHearts; }

    void TakeDamage(int damage);
    void Heal(int healAmount);

    bool IsDead() const { return m_currentHearts <= 0; }
    bool IsFull() const { return m_currentHearts == m_maxHearts; }

    // for rendering
    float GetHealthPercentage() const;

private:
    int m_maxHearts;
    int m_currentHearts;
};