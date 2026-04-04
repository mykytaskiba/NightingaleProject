#pragma once
#include "defines.h"

template <typename Resource>
class AssetManager {

private:

    static std::map<string, Resource*> m_assetMap;

public:

    static bool has(string const& key) {
        auto it = m_assetMap.find(key);
        return (it != m_assetMap.end());
    }

    static Resource* retrieve(string const& key) {
        if (has(key)) {
            return m_assetMap[key];
        }
        return nullptr;
    }

    static bool add(string const& key, Resource* value) {
        if (has(key)) {
            return false;
        }

        m_assetMap[key] = value;
        return true;
    }

    
    template <typename Functor>
    static void execute(Functor func) {
        for (auto& it : m_assetMap) {
            func(it.second);
        }
    }

    template <typename Functor>
    static void execute_key(Functor func) {
        for (auto& it : m_assetMap) {
            func(it.first, it.second);
        }
    }

};

template <typename Resource>
std::map<string, Resource*> AssetManager<Resource>::m_assetMap = std::map<string, Resource*>();


