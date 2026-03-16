#include <Geode/Bindings.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <vector>
#include <string>
#include "ListManager.h"
#include "EffectsManager.h"
#include "ParticleManager.h"

using namespace geode::prelude;

class $modify(GrDInfoLayer, LevelInfoLayer) {

    struct Fields {
        bool m_hasBeenOpened = false;
    };

    void updateDifficultyFace() {

        int aredlPos = ListManager::getPositionOfID(m_level->m_levelID);
        if (aredlPos == -1 || aredlPos > ListManager::getMaxListSize()) {
            return;
        }

        CCSprite* originalIcon = nullptr;
        bool iconFound = false;

        for (auto obj : CCArrayExt<CCNode*>(this->getChildren())) {
            if (auto newObj = typeinfo_cast<CCSprite*>(obj)) {
                if (newObj->getPosition() == m_difficultySprite->getPosition()
                && newObj->getZOrder() == 3) {
                    originalIcon = newObj;
                    iconFound = true;
                    break;
                }
            }
        }

        if (originalIcon == nullptr || !iconFound) {
            return;
        }

        originalIcon->setVisible(false);

        if (m_fields->m_hasBeenOpened) {
            return;
        }

        CCSprite* newIcon = ListManager::getSpriteFromPosition(aredlPos, true);
        newIcon->setID("grd-difficulty");

        auto newPos = originalIcon->getPosition();
        newIcon->setPosition(newPos);
        newIcon->setZOrder(originalIcon->getZOrder() + 10);

        if (originalIcon->getChildren() && originalIcon->getChildrenCount() > 0) {
            std::vector<CCSprite*> ringsToMove;

            for (auto iconObj : CCArrayExt<CCNode*>(originalIcon->getChildren())) {
                if (auto ringObj = typeinfo_cast<CCSprite*>(iconObj)) {
                    ringsToMove.push_back(ringObj);
                }
            }

            for (CCSprite* ringObj : ringsToMove) {
                ringObj->retain();
                ringObj->removeFromParent();

                ringObj->setTag(69420);
                this->addChild(ringObj);
                ringObj->setPosition(newPos);
                ringObj->setZOrder(originalIcon->getZOrder() + 9);

                ringObj->release();
            }
        }

        this->addChild(newIcon);

        auto tier = ListManager::getTierForPosition(aredlPos);
        if (tier) {
            if (tier->id == 5 || tier->id == 4) { // Grandpa (5) и Infinite (4)
                EffectsManager::infinityBackground(this, aredlPos);
                EffectsManager::addInfinitySymbol(newIcon->getPosition(), this, aredlPos);

                if (!Mod::get()->getSettingValue<bool>("particles-disable")) {
                    bool isGrandpa = (tier->id == 5 && !Mod::get()->getSettingValue<bool>("grandpa-demon-disable"));
                    auto particle1 = ParticleManager::infiniteParticles1(50, isGrandpa);
                    particle1->setPosition({newIcon->getPositionX(), newIcon->getPositionY() + 5.f});
                    this->addChild(particle1);

                    auto particle2 = ParticleManager::infiniteParticles2(50);
                    particle2->setPosition({newIcon->getPositionX(), newIcon->getPositionY() + 5.f});
                    this->addChild(particle2);
                }
            } else if (tier->id == 3) { // Mythical (3)
                EffectsManager::mythicalBackground(this, aredlPos);
                if (!Mod::get()->getSettingValue<bool>("particles-disable")) {
                    auto particle = ParticleManager::mythicalParticles(50);
                    particle->setPosition({newIcon->getPositionX(), newIcon->getPositionY() + 5.f});
                    this->addChild(particle);
                }
            } else if (tier->id == 2) { // Legendary (2)
                EffectsManager::legendaryBackground(this, aredlPos);
                if (!Mod::get()->getSettingValue<bool>("particles-disable")) {
                    auto particle = ParticleManager::legendaryParticles(50);
                    particle->setPosition({newIcon->getPositionX(), newIcon->getPositionY() + 5.f});
                    this->addChild(particle);
                }
            }
        }

        m_fields->m_hasBeenOpened = true;
    }

    void updateLabelValues() {
        LevelInfoLayer::updateLabelValues();

        if (ListManager::demonIDList.size() == 0) {
            return;
        }

        if (this->m_level->m_stars != 10) {
            return;
        }

        updateDifficultyFace();
    }
};