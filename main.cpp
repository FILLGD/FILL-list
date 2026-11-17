
#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/ui/ScrollLayer.hpp>
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

// DİKKAT: Bu adresin gerçekten JSON döndürmesi gerekiyor.
static const std::string API_URL =
    "https://fillgdlist.netlify.app/api/levels";

class LeaderboardPopup : public Popup<> {
protected:
    EventListener<web::WebTask> m_listener;

    ScrollLayer* m_scroll = nullptr;
    CCLabelBMFont* m_status = nullptr;

    bool setup() override {
        this->setTitle("FILL List");

        auto size = m_mainLayer->getContentSize();

        // Kaydırılabilir liste alanı
        m_scroll = ScrollLayer::create({
            size.width - 40.f,
            size.height - 95.f
        });

        m_scroll->setPosition({20.f, 35.f});

        m_scroll->m_contentLayer->setLayout(
            ColumnLayout::create()
                ->setGap(5.f)
                ->setAxisReverse(true)
                ->setCrossAxisAlignment(AxisAlignment::Start)
                ->setAutoGrowAxis(0.f)
        );

        m_mainLayer->addChild(m_scroll);

        // Durum yazısı
        m_status = CCLabelBMFont::create(
            "Yukleniyor...",
            "bigFont.fnt"
        );

        m_status->setScale(0.4f);
        m_status->setPosition({
            size.width / 2.f,
            15.f
        });

        m_mainLayer->addChild(m_status);

        // Web dinleyicisi
        m_listener.bind([this](web::WebTask::Event* event) {
            auto response = event->getValue();

            if (!response) {
                return;
            }

            if (!response->ok()) {
                m_status->setString("Baglanti hatasi");
                return;
            }

            auto json = response->json();

            if (!json) {
                m_status->setString("JSON okunamadi");
                return;
            }

            this->render(json.unwrap());
        });

        this->refresh(0.f);

        // Otomatik yenileme
        auto seconds =
            Mod::get()->getSettingValue<int64_t>(
                "refresh-seconds"
            );

        if (seconds < 5) {
            seconds = 15;
        }

        this->schedule(
            schedule_selector(LeaderboardPopup::refresh),
            static_cast<float>(seconds)
        );

        return true;
    }

    void refresh(float) {
        if (m_status) {
            m_status->setString("Liste yenileniyor...");
        }

        m_listener.setFilter(
            web::WebRequest().get(API_URL)
        );
    }

    void render(matjson::Value const& data) {
        auto array = data.asArray();

        if (!array) {
            m_status->setString("Gecersiz JSON formati");
            return;
        }

        auto content = m_scroll->m_contentLayer;

        content->removeAllChildren();

        auto width = m_scroll->getContentSize().width;

        int count = 0;

        for (auto const& item : array.unwrap()) {
            count++;

            auto rank =
                item["rank"].asInt().unwrapOr(count);

            auto name =
                item["name"].asString().unwrapOr("?");

            auto points =
                item["points"].asInt().unwrapOr(0);

            // Creator bilgisi string veya array olabilir.
            std::string creators = "?";

            if (auto value = item["creators"].asString()) {
                creators = value.unwrap();
            }
            else if (auto values = item["creators"].asArray()) {
                creators.clear();

                for (auto const& creator : values.unwrap()) {
                    if (!creators.empty()) {
                        creators += " & ";
                    }

                    creators +=
                        creator.asString().unwrapOr("?");
                }
            }

            // Level satırı
            auto row = CCNode::create();

            row->setContentSize({
                width,
                42.f
            });

            // Level adı
            auto title = CCLabelBMFont::create(
                fmt::format(
                    "#{} {}",
                    rank,
                    name
                ).c_str(),
                "bigFont.fnt"
            );

            title->setAnchorPoint({0.f, 0.5f});
            title->setPosition({6.f, 29.f});
            title->limitLabelWidth(
                width - 100.f,
                0.5f,
                0.2f
            );

            row->addChild(title);

            // Creator
            auto creatorLabel = CCLabelBMFont::create(
                fmt::format(
                    "by {}",
                    creators
                ).c_str(),
                "chatFont.fnt"
            );

            creatorLabel->setAnchorPoint({0.f, 0.5f});
            creatorLabel->setPosition({6.f, 10.f});
            creatorLabel->setOpacity(180);

            creatorLabel->limitLabelWidth(
                width - 100.f,
                0.5f,
                0.2f
            );

            row->addChild(creatorLabel);

            // Puan
            auto pointsLabel = CCLabelBMFont::create(
                fmt::format(
                    "{} pts",
                    points
                ).c_str(),
                "goldFont.fnt"
            );

            pointsLabel->setScale(0.45f);
            pointsLabel->setAnchorPoint({1.f, 0.5f});
            pointsLabel->setPosition({
                width - 6.f,
                21.f
            });

            row->addChild(pointsLabel);

            content->addChild(row);
        }

        content->updateLayout();
        m_scroll->moveToTop();

        m_status->setString(
            count == 0
                ? "Henuz level yok"
                : fmt::format("{} level - Canli", count).c_str()
        );
    }

public:
    static LeaderboardPopup* create() {
        auto popup = new LeaderboardPopup();

        if (popup->initAnchored(340.f, 250.f)) {
            popup->autorelease();
            return popup;
        }

        delete popup;
        return nullptr;
    }
};

class $modify(FillMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) {
            return false;
        }

        auto menu = this->getChildByID("bottom-menu");

        if (!menu) {
            log::warn("FILL: bottom-menu bulunamadi");
            return true;
        }

        auto sprite = ButtonSprite::create("FILL");

        auto button = CCMenuItemSpriteExtra::create(
            sprite,
            this,
            menu_selector(FillMenuLayer::onFill)
        );

        button->setID("fill-leaderboard-button"_spr);

        menu->addChild(button);
        menu->updateLayout();

        return true;
    }

    void onFill(CCObject*) {
        auto popup = LeaderboardPopup::create();

        if (popup) {
            popup->show();
        }
    }
};
