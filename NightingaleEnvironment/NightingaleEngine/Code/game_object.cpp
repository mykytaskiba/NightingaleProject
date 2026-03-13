#include "pch.h"
#include "game_object.h"


void GameObject::execute_on_hierarchy(TGameObjectFunc functor)
{
    (*functor)(*this);

    for (auto it = m_children.begin(); it != m_children.end(); ++it) {
        (*it)->execute_on_hierarchy(functor);
    }
}

void GameObject::sync_to_physics()
{
    if (m_pPhysicsBody == nullptr) {
        return;
    }
    m_transform.position = m_pPhysicsBody->position;
}
