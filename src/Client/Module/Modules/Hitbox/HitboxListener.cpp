#include "HitboxListener.hpp"
#include "../../../../Utils/Render/DrawUtils.hpp"

void HitboxListener::onSetupAndRender(SetupAndRenderEvent &event) {
    aabbsToRender.clear();
    if (!SDK::clientInstance || !SDK::clientInstance->getLocalPlayer() || !SDK::clientInstance->mcgame->mouseGrabbed ||
        !SDK::clientInstance->getLocalPlayer()->level)
        return;
    auto player = SDK::clientInstance->getLocalPlayer();
    for (const auto &ent: player->level->getRuntimeActorList()) {
        if (ent != nullptr && ent != player /*&& ent->isPlayer() && ent->hasCategory(ActorCategory::Player)*/) {
            float dist = player->getPosition()->dist(*ent->getPosition());
            // This may let through some entites
            if (!ent->isAlive() || !player->isValidTarget(ent) || dist > 30 || !player->canSee(*ent) ||
                ent->getActorFlag(ActorFlags::FLAG_INVISIBLE)) // + ent == player ||
                continue;

            Vec3<float> end = ent->getRenderPositionComponent()->renderPos;
            AABB render;

            if (ent->hasCategory(ActorCategory::Player)) {
                render = AABB(end, ent->aabb->size.x, ent->aabb->size.y, 1.8);
                render.upper.y += 0.2f;
                render.lower.y += 0.2f;
            } else
                render = AABB(end, ent->aabb->size.x, ent->aabb->size.y, 0);

            render.upper.y += 0.1f;

            aabbsToRender.emplace_back(render);
        }
    }
}

void HitboxListener::onRender(RenderEvent &event) {

    if (!SDK::clientInstance || !SDK::clientInstance->getLocalPlayer() || !SDK::clientInstance->mcgame->mouseGrabbed ||
        !SDK::clientInstance->getLocalPlayer()->level)
        return;

    auto player = SDK::clientInstance->getLocalPlayer();

    if (player != nullptr) {
        D2D1_COLOR_F color2;
        // TODO: optimize getting colors
        if (module->settings.getSettingByName<bool>("color_rgb")->value) color2 = FlarialGUI::rgbColor;
        else color2 = FlarialGUI::HexToColorF(module->settings.getSettingByName<std::string>("color")->value);

        for (const auto &aabb: aabbsToRender) {
            auto lineWidth = (float)fmax(0.5f, 1 / (float)fmax(1,
                                                               player->getRenderPositionComponent()->renderPos.dist(aabb.lower)));
            DrawUtils::addBox(aabb.lower, aabb.upper, lineWidth, 1, color2);
        }
    }
}
