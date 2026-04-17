#include "pch.h"
#include "game_object.h"
#include "engine_functions.h"
#include "ngphys.h"
#include "ngrender.h"
#include "renderer.h"
#include "json.hpp"
#include "ngjson.h"

JSON_IMPL(GameObject)

void GameObject::sync_physics_to_gameobject()
{
    if (m_pPhysicsBody == nullptr) {
        return;
    }
    m_pPhysicsBody->getPosition() = m_transform.position;
    m_pPhysicsBody->getRotation() = m_transform.rotation;
}

void GameObject::sync_gameobject_to_physics()
{
    //TO DO: Refactor this!!
    if (m_pPhysicsBody == nullptr) {
        if (m_pRenderNode != nullptr) {
            m_pRenderNode->setNodePosition(m_transform);
        }
        return;
    }
    m_transform.position = m_pPhysicsBody->getPosition();
    m_transform.rotation = m_pPhysicsBody->getRotation();

    if (m_pRenderNode == nullptr) {
        return;
    }

    if (EngineFunctions::physics().getInterpolateBetweenFrames()) {

        Transform nodeTransform = m_transform;
        nodeTransform.position += m_pPhysicsBody->getVelocity() * EngineFunctions::physics().getAccumulatedTime();
        m_pRenderNode->setNodePosition(nodeTransform);
    }
    else {
        m_pRenderNode->setNodePosition(m_transform);
    }
}

void GameObject::execute_on_hierarchy(TGameObjectFunc functor)
{
    (functor)(*this);

    for (auto it = m_vChildren.begin(); it != m_vChildren.end(); ++it) {
        (*it)->execute_on_hierarchy(functor);
    }
}

void GameObject::execute_on_children(TGameObjectFunc functor)
{
    for (auto it = m_vChildren.begin(); it != m_vChildren.end(); ++it) {
        (functor)(**it);
    }
}

bool GameObject::removeChild(GameObject* pChild)
{
    auto itFound = std::find(m_vChildren.begin(), m_vChildren.end(), pChild);
    if (itFound == m_vChildren.end()) {
        return false; //we did not find the child
    }
    m_vChildren.erase(itFound);
    return true;
}

void GameObject::freeResources()
{
    if (m_pRenderNode != nullptr) {
        EngineFunctions::Renderer().removeRenderable(m_pRenderNode);
        delete m_pRenderNode;
        m_pRenderNode = nullptr;
    }
    if (m_pPhysicsBody != nullptr) {
        EngineFunctions::physics().removeBody(m_pPhysicsBody);
        delete m_pPhysicsBody;
        m_pPhysicsBody = nullptr;
    }

    assert(m_pRenderNode == nullptr);
    assert(m_pPhysicsBody == nullptr);
}

GameObject::GameObject()
{
    m_guid = GUID::Generate();
}

GameObject::GameObject(std::string const& alias) : m_alias(alias)
{
    m_guid = GUID::Generate();
}

