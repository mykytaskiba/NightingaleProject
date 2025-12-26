#pragma once
#include <assimp/Importer.hpp>
#include "defines.h"

class Animation;

class AnimationBuilder {
public:
    AnimationBuilder(aiScene const*);
    bool isSuccessful() const;
    Animation* getAnimation() const;


private:
    bool m_bSuccess{ false };
    Animation* m_pAnimation{ nullptr };

    uint m_numKeys{ 0 };

    void loadTimeKeys(aiNodeAnim const* animChannel);

    void loadChannel(aiNodeAnim const* animChannel, uint channelId);
};

