#include "weapons/Hammer.h"
#include <iostream>

using namespace std;

const float Hammer::CHARGE_RATE = 0.5f; // Charges in 2 seconds

Hammer::Hammer()
    : m_chargeLevel(1.0f) 
{ 

}

void Hammer::Use() {
    if (IsCharged()) {
        Weapon::Use();
        m_chargeLevel = 0.0f; // Reset charge after use
        cout << "Hammer used!" << endl;
    } else {
        cout << "Hammer not charged yet!" << endl;
    }
}

void Hammer::Charge(float deltaTime) {
    if (m_chargeLevel < 1.0f) {
        m_chargeLevel += CHARGE_RATE * deltaTime;
        if (m_chargeLevel > 1.0f) {
            m_chargeLevel = 1.0f;
        }
    }
}