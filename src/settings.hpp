#pragma once

#include <nwo5.silly-api/cache/include.hpp>

using namespace nwo5::cache::prelude;

namespace Settings {
    inline SettingCache<bool> enabled{"enabled"};
    inline SettingCache<bool> useCustomImage{"use-custom-image"};
    inline SettingCache<asp::fs::Path> customImage{"custom-image"};
    inline SettingCache<bool> spin{"spin"};
    inline SettingCache<float> scale{"scale"};
    inline SettingCache<float> speed{"speed"};
    inline SettingCache<cocos2d::ccColor3B> color{"color"};
    inline SettingCache<bool> chroma{"chroma"};
    inline SettingCache<float> chromaSpeed{"chroma-speed"};
}