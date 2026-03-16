#ifndef LISTMANAGER_H
#define LISTMANAGER_H
#include <vector>
#include <string>

using namespace geode::prelude;

struct DemonTier {
    int id; // 0 = Supreme, 1 = Ultimate, 2 = Legendary, 3 = Mythical, 4 = Infinite, 5 = Grandpa
    int minRank;
    int maxRank;
    const char* spriteName;
    const char* textSpriteName;
};

class ListManager {
public:
    inline static std::vector<int> demonIDList;
    inline static bool firstTimeOpen;
    inline static int filterType;
    inline static bool isSupremeSearching;
    inline static int currentSearchMax = 0;
    inline static int currentSearchMin = 0;

    inline static std::vector<DemonTier> demonTiers = {
        {5, 0, 0, "GrD_demon5.png", "GrD_demon5_text.png"},           // Grandpa (Top 1)
        {4, 1, 49, "GrD_demon4.png", "GrD_demon4_text.png"},          // Infinite (Top 2-50)
        {3, 50, 149, "GrD_demon3.png", "GrD_demon3_text.png"},        // Mythical (Top 51-150)
        {2, 150, 249, "GrD_demon2.png", "GrD_demon2_text.png"},       // Legendary (Top 151-250)
        {1, 250, 499, "GrD_demon1.png", "GrD_demon1_text.png"},       // Ultimate (Top 251-500)
        {0, 500, 749, "GrD_demon0.png", "GrD_demon0_text.png"}        // Supreme (Top 501-750)
    };

    inline static DemonTier* getTierForPosition(int pos) {
        for (auto& tier : demonTiers) {
            if (pos >= tier.minRank && pos <= tier.maxRank) return &tier;
        }
        return nullptr;
    }

    inline static DemonTier* getTierByID(int id) {
        for (auto& tier : demonTiers) {
            if (tier.id == id) return &tier;
        }
        return nullptr;
    }

    inline static int getMaxListSize() {
        int max = 0;
        for (auto& tier : demonTiers) {
            if (tier.maxRank > max) max = tier.maxRank;
        }
        return max;
    }

    inline static void parseRequestString(std::string str) {
        size_t isFound = str.find("_id");
        while (isFound != std::string::npos) {
            str = str.substr(isFound + 5);
            size_t findBracket = str.find("}");
            try {
                int id = std::stoi(str.substr(0, findBracket));
                demonIDList.push_back(id);
            } catch (const std::exception& e) {}
            isFound = str.find("_id");
        }
    }

    inline static int getPositionOfID(int id) {
        for (unsigned int i = 0; i < demonIDList.size(); i++) {
            if (demonIDList.at(i) == id) return i;
        }
        return -1;
    }

	inline static CCSprite* getSpriteFromPosition(int pos, bool hasText) {
        auto tier = getTierForPosition(pos);
        if (tier) {
            if (tier->id == 5 && Mod::get()->getSettingValue<bool>("grandpa-demon-disable")) {
                return CCSprite::createWithSpriteFrameName(hasText ? "GrD_demon4_text.png"_spr : "GrD_demon4.png"_spr);
            }

            std::string spriteName = hasText ? tier->textSpriteName : tier->spriteName;

            std::string expandedName(Mod::get()->expandSpriteName(spriteName));

            return CCSprite::createWithSpriteFrameName(expandedName.c_str());
        }
        return nullptr;
    }

    inline static GJSearchObject* getSearchObject(int maxRank, int minRank) {
        std::stringstream download;
        bool first = true;
        for (int i = maxRank; i >= minRank; i--) {
            if (i < 0 || i >= demonIDList.size()) continue;
            if (!first) download << ",";
            download << std::to_string(demonIDList.at(i));
            first = false;
        }
        download << "&gameVersion=22";
        return GJSearchObject::create(SearchType::Type19, download.str());
    }
};
#endif