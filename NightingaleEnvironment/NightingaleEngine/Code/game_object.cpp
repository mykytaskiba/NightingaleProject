#include "pch.h"
#include "game_object.h"
#include "engine_functions.h"
#include "ngphys.h"
#include "ngrender.h"
#include "renderer.h"
#include "json.hpp"

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

#include "loader.h"
#include "ngjson.h"
void GameObject::jsonOperation()
{
    nlohmann::json jsonObject;
    JSONOperation operation{ JSONMode::Serialize,jsonObject };

    operation.serialize_only("factory_key", getFactoryKey());
    operation.link("alias", m_alias);

    Loader::saveToFile("jsonTest/json.json", jsonObject, FileCreationFlags::CreateDirectory | FileCreationFlags::Overwrite);
}


GameObject::GameObject()
{
    m_guid = GUID::Generate();
}

GameObject::GameObject(std::string const& alias) : m_alias(alias)
{
    m_guid = GUID::Generate();
}

