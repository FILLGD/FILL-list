
#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

namespace {
    constexpr const char* FILL_URL =
        "https://fillgdlist.netlify.app/?embed=rankings";
}

class $modify(FillMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) {
            return false;
        }

        auto menu = this->getChildByID("bottom-menu");

        if (!menu) {
            log::warn("FILL List: bottom-menu not found");
            return true;
        }

        auto sprite = ButtonSprite::create("FILL");

        auto button = CCMenuItemSpriteExtra::create(
            sprite,
            this,
            menu_selector(FillMenuLayer::onFill)
        );

        button->setID("fill-list-button");

        menu->addChild(button);
        menu->updateLayout();

        return true;
    }

    void onFill(CCObject*) {
        web::openLinkInBrowser(FILL_URL);
    }
};
