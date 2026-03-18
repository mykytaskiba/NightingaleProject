#include "pch.h"
#include "game_object.h"


void GameObject::execute_on_hierarchy(TGameObjectFunc functor)
{
    (functor)(*this);

    execute_on_children(functor);
}

void GameObject::execute_on_children(TGameObjectFunc functor)
{
    for (auto it = m_children.begin(); it != m_children.end(); ++it) {
        (functor)(**it);
    }
}

GameObject::GameObject()
{
    m_guid = GUID::Generate();
}

GameObject::GameObject(std::string const& alias) : m_alias(alias)
{
    m_guid = GUID::Generate();
}

void GameObject::sync_to_physics()
{
    if (m_pPhysicsBody == nullptr) {
        return;
    }
    m_transform.position = m_pPhysicsBody->position;
}
