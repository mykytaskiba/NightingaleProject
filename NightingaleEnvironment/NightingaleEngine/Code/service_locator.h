#pragma once
#include "nightingale_assert.h"

template <typename TService>
class ServiceLocator {
private:
    inline static TService* s_pService{ nullptr };

public:
    static void provide(TService* pService) {
        assert(s_pService == nullptr);
        s_pService = pService;
    }

    static bool hasService() {
        return s_pService != nullptr;
    }

    static TService* retrieve() {
        assert(s_pService != nullptr);
        return s_pService;
    }

    static void clear() {
        assert(s_pService != nullptr);
        s_pService = nullptr;
    }
};