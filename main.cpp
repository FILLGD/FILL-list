
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

        if (!sprite) {
            log::error("FILL List: button sprite could not be created");
            return true;
        }

        auto button = CCMenuItemSpriteExtra::create(
            sprite,
            this,
            menu_selector(FillMenuLayer::onFill)
        );

        if (!button) {
            log::error("FILL List: button could not be created");
            return true;
        }

        button->setID("fill-list-button");

        menu->addChild(button);
        menu->updateLayout();

        return true;
    }

    void onFill(CCObject*) {
        web::openLinkInBrowser(FILL_URL);
    }
};
