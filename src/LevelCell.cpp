#include <Geode/Bindings.hpp>
#include <Geode/modify/LevelCell.hpp>
#include <vector>
#include "ListManager.h"
#include "EffectsManager.h"

using namespace geode::prelude;

class $modify(LevelCell) {

    void loadCustomLevelCell() {
        LevelCell::loadCustomLevelCell();

        if (ListManager::demonIDList.size() == 0) {
            return;
        }

        if (m_level->m_stars != 10) {
            return;
        }

        int aredlPos = ListManager::getPositionOfID(m_level->m_levelID);
        if (aredlPos == -1 || aredlPos > ListManager::getMaxListSize()) {
            return;
        }

        CCSprite* originalIcon = nullptr;

        for (auto obj : CCArrayExt<CCNode*>(m_mainLayer->getChildren())) {
            if (auto newObj = typeinfo_cast<CCNode*>(obj)) {
                if (newObj->getZOrder() == 2) {
                    newObj->setID("grd-demon-icon-layer");

                    for (auto obj2 : CCArrayExt<CCNode*>(newObj->getChildren())) {
                        if (auto newObj2 = typeinfo_cast<CCSprite*>(obj2)) {
                            if (newObj2->getZOrder() == 3) {
                                originalIcon = newObj2;
                                break;
                            }
                        }
                    }
                }
            }
        }

        if (originalIcon == nullptr) {
            return;
        }

        CCSprite* newIcon = ListManager::getSpriteFromPosition(aredlPos, false);
        auto layer = m_mainLayer->getChildByID("grd-demon-icon-layer");

        auto newPos = originalIcon->getPosition();
        newIcon->setPosition(originalIcon->getPosition());
        newIcon->setZOrder(originalIcon->getZOrder() + 25);

        if (originalIcon->getChildren()) {
            std::vector<CCSprite*> clearRings;
            for (auto clearObj : CCArrayExt<CCNode*>(originalIcon->getChildren())) {
                if (auto newObj = typeinfo_cast<CCSprite*>(clearObj)) {
                    if (newObj->getTag() == 69420) {
                        clearRings.push_back(newObj);
                    }
                }
            }
            for (auto ring : clearRings) {
                ring->removeFromParentAndCleanup(true);
            }
        }

        if (originalIcon->getChildren()) {
            std::vector<CCSprite*> moveRings;
            for (auto iconObj : CCArrayExt<CCNode*>(originalIcon->getChildren())) {
                if (auto newObj = typeinfo_cast<CCSprite*>(iconObj)) {
                    moveRings.push_back(newObj);
                }
            }

            for (auto ring : moveRings) {
                ring->retain();
                ring->removeFromParent();

                ring->setTag(69420);
                layer->addChild(ring);
                ring->setPosition(newPos);

                ring->release();
            }
        }

        originalIcon->setVisible(false);
        layer->addChild(newIcon);

        auto tier = ListManager::getTierForPosition(aredlPos);
        if (tier && (tier->id == 5 || tier->id == 4)) {
            EffectsManager::addInfinitySymbol(newIcon->getPosition(), layer, aredlPos);
        }
    }
};