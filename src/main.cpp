#include <Geode/modify/LoadingCircle.hpp>
#include <nwo5.silly-api/include/include.hpp>
#include <alphalaneous.alphas_geode_utils/include/ObjectModify.hpp>
#include <Geode/ui/LoadingSpinner.hpp>
#include <asp/fs.hpp>
#include "settings.hpp"

using namespace geode::prelude;
using namespace alpha::utils;
using namespace nwo5::prelude;

class CustomLoadingSprite final : public CCSprite {
protected:
    bool init() {
        const auto texture = Settings::useCustomImage && asp::fs::exists(Settings::customImage)
            ? string::pathToString(Settings::customImage.get())
            : "loadingCircle.png";

        if (!CCSprite::initWithFile(texture.c_str())) {
            return false;
        }

        Setup(this)
            .id("custom-spinner"_spr)
            .scale(Settings::scale)
            .blendFunc({GL_SRC_ALPHA, GL_ONE});

        if (Settings::spin) {
            this->runAction(
                CCRepeatForever::create(CCRotateBy::create(Settings::speed, 360.0f))
            );
        }

        scheduleUpdate();

        return true;
    }

    void update(float = float{}) {
        // loadingspinner
        if (auto spr = this->getParent()->getChildByType<CCSprite*>(0); spr && spr != this) {
            spr->setDontDraw(true);

            Setup(this)
                .visible(this->getParent())
                .pos(spr)
                .opacity(spr)
                .scaleToFit(ccMax(spr->getScaledContentSize()) * Settings::scale);
        }
        // loadingcirclesprite
        else if (auto parent = typeinfo_cast<LoadingCircleSprite*>(this->getParent())) {
            Setup(this)
                .opacity(parent->getOpacity());
        }

        this->setColor(
            Settings::chroma ? nwo5::utils::getChroma<ccColor3B>(Settings::chromaSpeed, 0.0f, 0.5f) : Settings::color
        );
    }

public:
    static CustomLoadingSprite* create() {
        auto ret = new CustomLoadingSprite;

        if (!ret->init()) {
            delete ret;

            return nullptr;
        }

        ret->autorelease();

        return ret;
    }

    void forceUpdate() {
        if (this->getParent()) {
            this->update();
        }
    }
};

class $modify(LoadingCircle) {
    static void onModify(auto& pSelf) {
        (void)pSelf.setHookPriority("LoadingCircle::init", Priority::Replace);
        (void)pSelf.setHookPriority("LoadingCircle::show", Priority::Replace);
    }

    bool init() {
        if (!Settings::enabled) {
            return LoadingCircle::init();
        }

        if (!CCLayerColor::initWithColor({0, 0, 0, 0})) {
            return false;
        }

        m_sprite = Setup(CustomLoadingSprite::create())
            .pos(CCDirector::get()->getWinSize() / 2)
            .scale(Settings::scale)
            .parent(this)
            .blendFunc({GL_SRC_ALPHA, GL_ONE});
        
        return true;
    }
    
    void show() {
        if (!Settings::enabled) {
            return LoadingCircle::show();
        }

        CCNode* parent = m_parentLayer;

        if (!parent) {
            parent = CCDirector::get()->getRunningScene();
        }

        parent->addChild(this, 105);

        if (m_fade) {
            this->setOpacity(0);

            this->runAction(CCFadeTo::create(0.4f, 150));
        }

        m_sprite->setOpacity(0);

        auto action = CCFadeTo::create(0.4f, 200);
        action->setTag(0);

        m_sprite->runAction(action);
    }
};

class $nodeModify(LoadingCircleSprite) {
    void modify() {
        if (!Settings::enabled) {
            return;
        }

        reinterpret_cast<CCSprite*>(this)->setDontDraw(true);

        this->stopAllActions();

        auto spr = *Setup(CustomLoadingSprite::create())
            .scale(Settings::scale)
            .parent(this)
            .center();
        
        spr->forceUpdate();
    }
};

class $nodeModify(geode::LoadingSpinner) {
    void modify() {
        if (!Settings::enabled) {
            return;
        }

        auto spr = CustomLoadingSprite::create();
        this->addChild(spr);

        spr->forceUpdate();
    }
};

$on_mod(Loaded) {
    SettingsManager::get()->load();
}