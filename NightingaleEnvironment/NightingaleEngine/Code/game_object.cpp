#include "pch.h"
#include "game_object.h"


GameObject::GameObject() :
    m_components(),
    m_children(),
    m_pParent(nullptr),
    m_pRenderNode(nullptr),
    m_transform()
{
}


void GameObject::execute(TGameObjectFunc functor)
{
    (*functor)(*this);

    for (auto it = m_children.begin(); it != m_children.end(); ++it) {
        (*it)->execute(functor);
    }
}
