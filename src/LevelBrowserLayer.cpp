#include <Geode/Bindings.hpp>
#include <Geode/modify/LevelBrowserLayer.hpp>
#include "ListManager.h"

class $modify(GrDLevelBrowserLayer, LevelBrowserLayer) {
    struct Fields {
        int m_currentPage = 0;
    };

    bool init(GJSearchObject* p0) {
        if (!ListManager::isSupremeSearching || p0->m_searchType != SearchType::Type19) {
            LevelBrowserLayer::init(p0);
            return true;
        }

        this->m_fields->m_currentPage = 0;
        int maxR = ListManager::currentSearchMax;
        int minR = std::max(ListManager::currentSearchMin, maxR - 9);

        LevelBrowserLayer::init(ListManager::getSearchObject(maxR, minR));
        return true;
    }

    void loadLevelsFinished(cocos2d::CCArray* p0, char const* p1, int p2) {
        LevelBrowserLayer::loadLevelsFinished(p0, p1, p2);
        if (!ListManager::isSupremeSearching || this->m_searchObject->m_searchType != SearchType::Type19) return;

        hideStuff();
        this->m_leftArrow->setVisible(this->m_fields->m_currentPage > 0);

        int totalLevels = ListManager::currentSearchMax - ListManager::currentSearchMin + 1;
        int maxPages = (totalLevels - 1) / 10;
        this->m_rightArrow->setVisible(this->m_fields->m_currentPage < maxPages);
    }

    void onNextPage(CCObject* sender) {
        LevelBrowserLayer::onNextPage(sender);
        if (!ListManager::isSupremeSearching || this->m_searchObject->m_searchType != SearchType::Type19) return;

        int totalLevels = ListManager::currentSearchMax - ListManager::currentSearchMin + 1;
        int maxPages = (totalLevels - 1) / 10;

        if (this->m_fields->m_currentPage < maxPages) {
            this->m_fields->m_currentPage += 1;
            nextBtnActions();
        }
    }

    void onPrevPage(CCObject* sender) {
        LevelBrowserLayer::onPrevPage(sender);
        if (!ListManager::isSupremeSearching || this->m_searchObject->m_searchType != SearchType::Type19) return;

        if (this->m_fields->m_currentPage > 0) {
            this->m_fields->m_currentPage -= 1;
            nextBtnActions();
        }
    }

    void nextBtnActions() {
        hideStuff();
        int maxR = ListManager::currentSearchMax - this->m_fields->m_currentPage * 10;
        int minR = std::max(ListManager::currentSearchMin, maxR - 9);
        LevelBrowserLayer::loadPage(ListManager::getSearchObject(maxR, minR));
    }

    void hideStuff() {
        if (this->m_pageBtn) this->m_pageBtn->setVisible(false);
        int start = this->m_fields->m_currentPage * 10 + 1;
        int totalLevels = ListManager::currentSearchMax - ListManager::currentSearchMin + 1;
        int end = std::min(start + 9, totalLevels);

        if (this->m_countText) {
            this->m_countText->setString(fmt::format("{} to {} of {}", start, end, totalLevels).c_str());
        }
    }
};