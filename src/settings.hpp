#pragma once

#include <nwo5.silly-api/include/settings/include.hpp>

using namespace nwo5::settings::prelude;

namespace Settings {
    inline Setting<bool> enabled{"enabled"};
    inline Setting<bool> useCustomImage{"use-custom-image"};
    inline Setting<std::filesystem::path> customImage{"custom-image"};
    inline Setting<bool> spin{"spin"};
    inline Setting<float> scale{"scale"};
    inline Setting<float> speed{"speed"};
    inline Setting<cocos2d::ccColor3B> color{"color"};
    inline Setting<bool> chroma{"chroma"};
    inline Setting<float> chromaSpeed{"chroma-speed"};
}