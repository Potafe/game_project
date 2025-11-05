#include "player/Health.h"

Health::Health()
    : m_maxHearts(5)
    , m_currentHearts(5) 
{

}

Health::Health(int maxHearts)
    : m_maxHearts(maxHearts)
    , m_currentHearts(maxHearts)
{

}

void Health::SetMaxHearts(int maxHearts) {
    m_maxHearts = maxHearts;
    if (m_currentHearts > maxHearts) {
        m_currentHearts = m_maxHearts;
    }
}

void Health::SetCurrentHearts(int hearts) {
    m_currentHearts = hearts;

    if (m_currentHearts < 0) {
        m_currentHearts = 0;
    }

    if (m_currentHearts > m_maxHearts) {
        m_currentHearts = m_maxHearts;
    }
}

void Health::TakeDamage(int damage) {
    m_currentHearts -= damage;
    if (m_currentHearts < 0) {
        m_currentHearts = 0;
    }
}

void Health::Heal(int healAmount) {
    m_currentHearts += healAmount;

    if (m_currentHearts > m_maxHearts) {
        m_currentHearts = m_maxHearts;
    }
}

float Health::GetHealthPercentage() const {
    if (m_maxHearts == 0) return 0.0f;
    return static_cast<float>(m_currentHearts) / static_cast<float>(m_maxHearts);
}
