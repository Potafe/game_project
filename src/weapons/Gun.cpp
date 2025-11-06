#include "weapons/Gun.h"
#include "weapons/Bullet.h"
#include <iostream>
#include <algorithm>

using namespace std;

Gun::Gun()
    : m_ammo(MAX_AMMO)
{

}

void Gun::Use() {
    if (m_ammo > 0) {
        Weapon::Use();
        m_ammo--;
        cout << "Ammo left: " << m_ammo << endl;
    } else {
        cout << "Out of ammo" << endl;
    }
}

void Gun::Update(float deltaTime) {
    Weapon::Update(deltaTime);
    
    // Update all bullets
    for (auto& bullet : m_bullets) {
        bullet->Update(deltaTime);
    }
    
    // Remove inactive bullets
    m_bullets.erase(
        remove_if(m_bullets.begin(), m_bullets.end(),
            [](const unique_ptr<Bullet>& bullet) {
                return !bullet->IsActive();
            }),
        m_bullets.end()
    );
}

void Gun::Reload() {
    m_ammo = MAX_AMMO;
    cout << "Reloaded Ammo: " << m_ammo << endl;
}