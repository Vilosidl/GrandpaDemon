#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/utils/web.hpp>
#include "ListManager.h"
#include <string>

using namespace geode::prelude;

class $modify(MenuLayer) {

    struct Fields {
        TaskHolder<web::WebResponse> m_listener;
    };

    bool init() {
        if (!MenuLayer::init()) return false;

        if (ListManager::firstTimeOpen) {
            return true;
        }

        auto req = web::WebRequest();

        m_fields->m_listener.spawn(
            req.get("https://api.aredl.net/v2/api/aredl/levels"),
            [](web::WebResponse res) {

                auto str = res.string().unwrapOr("Failed.");

                if (res.code() != 200 || str == "Failed." || str == "-1") {
                    ListManager::firstTimeOpen = true;
                    ListManager::filterType = -2;
                    std::string errorStr = "\n\n<cr>Could not load data from AREDL.</c>\nThe API could be down, but chances are, your internet just sucks.\n\n<cg>Restart your game to try again.</c>\n\n<cb>-Grandpa Demon</c>";
                    FLAlertLayer::create("What the??", str + errorStr, "OK")->show();
                    return;
                }

                ListManager::parseRequestString(str);
                ListManager::firstTimeOpen = true;
                ListManager::filterType = -1;
            }
        );

        return true;
    }
};