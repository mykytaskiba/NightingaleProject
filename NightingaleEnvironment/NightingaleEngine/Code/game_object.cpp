#include "pch.h"
#include "game_object.h"
#include "engine_functions.h"


Transform const& GameObject::getRenderTransform() const
{   
    //TO DO: Not sure what to do here, this is just a work around at this 
    if (m_pPhysicsBody == nullptr) {
        return m_transform;
    }
    if (!EngineFunctions::physics().getInterpolateBetweenFrames()) {
        return m_transform;
 }
    return m_renderTransform;
}

void GameObject::sync_physics_to_gameobject()
{
    if (m_pPhysicsBody == nullptr) {
        return;
    }
    m_pPhysicsBody->setPosition(m_transform.position);
}

void GameObject::sync_gameobject_to_physics()
{
    if (m_pPhysicsBody == nullptr) {
        return;
    }
    m_transform.position = m_pPhysicsBody->getPosition();

    if (EngineFunctions::physics().getInterpolateBetweenFrames()) {
        m_renderTransform.position = m_transform.position + m_pPhysicsBody->getVelocity() * EngineFunctions::physics().getAccumulatedTime();
    }
}

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

