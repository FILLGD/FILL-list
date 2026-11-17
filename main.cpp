
#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

static constexpr char const* RANKINGS_URL =
    "https://fillgdlist.netlify.app/?embed=rankings";

class $modify(FillMenuLayer, MenuLayer) {

    bool init() {
        if (!MenuLayer::init()) {
            return false;
        }

        auto menu = this->getChildByID("bottom-menu");

        if (!menu) {
            log::warn("FILL: bottom-menu bulunamadi!");
            return true;
        }

        auto sprite = ButtonSprite::create("FILL");

        auto button = CCMenuItemSpriteExtra::create(
            sprite,
            this,
            menu_selector(FillMenuLayer::onFill)
        );

        button->setID("fill-leaderboard-button");

        menu->addChild(button);
        menu->updateLayout();

        return true;
    }

    void onFill(CCObject*) {
        createQuickPopup(
            "FILL List",
            "Rankings sayfasini acmak istiyor musun?",
            "Iptal",
            "Ac",
            [](auto, bool confirmed) {
                if (confirmed) {
                    web::openLinkInBrowser(RANKINGS_URL);
                }
            }
        );
    }
};
