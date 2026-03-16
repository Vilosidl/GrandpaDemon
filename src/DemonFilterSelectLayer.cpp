#include <Geode/Bindings.hpp>
#include <Geode/modify/DemonFilterSelectLayer.hpp>
#include "ListManager.h"

class $modify(GrdDemonFilterSelectLayer, DemonFilterSelectLayer) {

    struct Fields {
        CCSprite* m_demon0Spr = nullptr;
        CCSprite* m_demon1Spr = nullptr;
        CCSprite* m_demon2Spr = nullptr;
        CCSprite* m_demon3Spr = nullptr;
        CCSprite* m_demon4Spr = nullptr;
        CCSprite* m_demon5Spr = nullptr;
    };


    static void onModify(auto &self) {
        (void)self.setHookPriority("DemonFilterSelectLayer::init", -1);
    }

    bool init() {

        DemonFilterSelectLayer::init();

        if (ListManager::demonIDList.empty()) {
            return true;
        }

        handleTouchPriority(this);
        CCLayer* layer = nullptr;

        for (auto layerObj : CCArrayExt<CCNode*>(this->getChildren())) {
            if (auto newObj = typeinfo_cast<CCLayer*>(layerObj)) {
                layer = newObj;
            }
        }

        if (!layer) return true;

        CCScale9Sprite* s9spr = nullptr;
        CCLabelBMFont* label = nullptr;
        CCMenu* menu = nullptr;

        // Geode v5: CCArrayExt
        for (auto obj : CCArrayExt<CCNode*>(layer->getChildren())) {
            if (auto newObj = typeinfo_cast<CCScale9Sprite*>(obj)) {
                s9spr = newObj;
            } else if (auto newObj2 = typeinfo_cast<CCLabelBMFont*>(obj)) {
                label = newObj2;
            } else if (auto newObj3 = typeinfo_cast<CCMenu*>(obj)) {
                menu = newObj3;
            }
        }

        if (!menu || !s9spr || !label) return true;

        CCMenuItemSpriteExtra* okButton = nullptr;

        for (auto obj_ok : CCArrayExt<CCNode*>(menu->getChildren())) {
            if (auto newObj = typeinfo_cast<CCMenuItemSpriteExtra*>(obj_ok)) {
                if (newObj->getPositionY() < 0) {
                    okButton = newObj;
                    break;
                }
            }
        }

        auto csize = s9spr->getContentSize();
        s9spr->setContentSize({csize.width, csize.height * 1.5f});
        auto newcsize = s9spr->getContentSize();
        label->setPositionY(newcsize.height + 0.f);
        label->setScale(0.8f);
        menu->setPositionY(newcsize.height * 0.82f);
        if (okButton) okButton->setPositionY(-160.f);

        handleTouchPriority(this);

        auto newMenu = CCMenu::create();
        layer->addChild(newMenu);
        newMenu->setPosition({layer->getContentSize().width / 2 - 3, layer->getContentSize().height * 0.38f});
        newMenu->setLayout(RowLayout::create()
        ->setGap(-1.f)
        ->setAxisAlignment(AxisAlignment::Center));

        auto instantSearch = CCLabelBMFont::create("Instant Search", "bigFont.fnt");
        instantSearch->setPosition({label->getPositionX(), layer->getContentSize().height * 0.52f});
        instantSearch->setScale(0.8f);
        layer->addChild(instantSearch);

        auto demon0Spr = CCSprite::createWithSpriteFrameName("GrD_demon0_text.png"_spr);
        this->m_fields->m_demon0Spr = demon0Spr;
        auto demon0Btn = CCMenuItemSpriteExtra::create(demon0Spr, this, menu_selector(GrdDemonFilterSelectLayer::onButton0));
        newMenu->addChild(demon0Btn);

        auto demon1Spr = CCSprite::createWithSpriteFrameName("GrD_demon1_text.png"_spr);
        this->m_fields->m_demon1Spr = demon1Spr;
        auto demon1Btn = CCMenuItemSpriteExtra::create(demon1Spr, this, menu_selector(GrdDemonFilterSelectLayer::onButton1));
        newMenu->addChild(demon1Btn);

        auto demon2Spr = CCSprite::createWithSpriteFrameName("GrD_demon2_text.png"_spr);
        this->m_fields->m_demon2Spr = demon2Spr;
        auto demon2Btn = CCMenuItemSpriteExtra::create(demon2Spr, this, menu_selector(GrdDemonFilterSelectLayer::onButton2));
        newMenu->addChild(demon2Btn);

        auto demon3Spr = CCSprite::createWithSpriteFrameName("GrD_demon3_text.png"_spr);
        this->m_fields->m_demon3Spr = demon3Spr;
        auto demon3Btn = CCMenuItemSpriteExtra::create(demon3Spr, this, menu_selector(GrdDemonFilterSelectLayer::onButton3));
        newMenu->addChild(demon3Btn);

        auto demon4Spr = CCSprite::createWithSpriteFrameName("GrD_demon4_text.png"_spr);
        this->m_fields->m_demon4Spr = demon4Spr;
        auto demon4Btn = CCMenuItemSpriteExtra::create(demon4Spr, this, menu_selector(GrdDemonFilterSelectLayer::onButton4));
        newMenu->addChild(demon4Btn);

        if (!(Mod::get()->getSettingValue<bool>("grandpa-demon-disable"))) {
            auto demon5Spr = CCSprite::createWithSpriteFrameName("GrD_demon5_text.png"_spr);
            this->m_fields->m_demon5Spr = demon5Spr;
            auto demon5Btn = CCMenuItemSpriteExtra::create(demon5Spr, this, menu_selector(GrdDemonFilterSelectLayer::onButton5));
            newMenu->addChild(demon5Btn);
        }

        newMenu->updateLayout();

        return true;
    }

    void handleButtonPress(int tierID) {
        ListManager::filterType = tierID;
        auto tier = ListManager::getTierByID(tierID);
        if (!tier) return;

        ListManager::currentSearchMax = tier->maxRank;
        ListManager::currentSearchMin = tier->minRank;

        int totalLevels = tier->maxRank - tier->minRank + 1;

        if (totalLevels > 100) {
            ListManager::isSupremeSearching = true;
            auto browserLayer = LevelBrowserLayer::create(ListManager::getSearchObject(tier->maxRank, std::max(tier->minRank, tier->maxRank - 9)));
            geode::cocos::switchToScene(browserLayer);
        } else {
            ListManager::isSupremeSearching = false;
            auto browserLayer = LevelBrowserLayer::create(ListManager::getSearchObject(tier->maxRank, tier->minRank));
            geode::cocos::switchToScene(browserLayer);
        }
    }

    void onButton0(CCObject* sender) { handleButtonPress(0); }
    void onButton1(CCObject* sender) { handleButtonPress(1); }
    void onButton2(CCObject* sender) { handleButtonPress(2); }
    void onButton3(CCObject* sender) { handleButtonPress(3); }
    void onButton4(CCObject* sender) { handleButtonPress(4); }
    void onButton5(CCObject* sender) { handleButtonPress(5); }
};