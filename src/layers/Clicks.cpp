#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>

#include "../utils/ee.hpp"
#include "../utils/Clicks.hpp"

using namespace geode::prelude;

static bool didPushButton; // done to solve all other occasions where the release sfx might play, despite the player not actually releasing the button

class $modify(PlayerObject)
{

public:
    // CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS
    // CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS CLICK SOUNDS

    bool pushButton(PlayerButton p0)
    {
        if (!PlayerObject::pushButton(p0))
            return false;

        if (Mod::get()->getSettingValue<bool>("only-on-jump")) {
            if (p0 != PlayerButton::Jump) {
                return false;
            }
        }

        didPushButton = true;

        if (!GameManager::sharedState()->getPlayLayer() && !GameManager::sharedState()->getEditorLayer())
            return false;
        if (PlayLayer::get())
            if (this == PlayLayer::get()->m_player2 && !PlayLayer::get()->m_level->m_twoPlayerMode)
                return false;
        if (LevelEditorLayer::get()) {
            if (this == LevelEditorLayer::get()->m_player2 && !LevelEditorLayer::get()->m_level->m_twoPlayerMode)
                return false;
        }

        auto clickSliderValue = Mod::get()->getSettingValue<int64_t>("clicksound-currentsound");
        auto customClickSound = Mod::get()->getSettingValue<std::filesystem::path>("custom-clicksound").string();
        bool usingCustomClickSound;
        std::string clickSoundInUse = Clicks::getClickSprite(clickSliderValue);

        usingCustomClickSound = false;
        if (clickSliderValue != 0)
            usingCustomClickSound = false;
        if (clickSoundInUse == "__USECUSTOM__")
            usingCustomClickSound = true;

        if (Mod::get()->getSettingValue<bool>("enable-clicksound") && !usingCustomClickSound) {
            auto fae    = FMODAudioEngine::sharedEngine();
            auto system = fae->m_system;

            FMOD::Channel *channel;
            FMOD::Sound *sound;

            system->createSound((Mod::get()->getResourcesDir().parent_path() / clickSoundInUse).string().c_str(), FMOD_DEFAULT, nullptr, &sound);

            if (Mod::get()->getSettingValue<bool>("use-sfx-volume")) {
#if defined(GEODE_IS_WINDOWS)
                if (fae->m_sfxVolume != 0)
                    system->playSound(sound, nullptr, false, &channel);
                channel->setVolume(fae->m_sfxVolume * 2.f);
#else
                if (Mod::get()->getSettingValue<int64_t>("volume-slider") != 0)
                    system->playSound(sound, nullptr, false, &channel);
                channel->setVolume(Mod::get()->getSettingValue<int64_t>("volume-slider") / 50.f);
#endif
            }
            else {
                if (Mod::get()->getSettingValue<int64_t>("volume-slider") != 0)
                    system->playSound(sound, nullptr, false, &channel);
                channel->setVolume(Mod::get()->getSettingValue<int64_t>("volume-slider") / 50.f);
            }
        }

        if (Mod::get()->getSettingValue<bool>("enable-clicksound") && usingCustomClickSound) {
            auto fae    = FMODAudioEngine::sharedEngine();
            auto system = fae->m_system;

            FMOD::Channel *channel;
            FMOD::Sound *sound;

            system->createSound(customClickSound.c_str(), FMOD_DEFAULT, nullptr, &sound);

            if (Mod::get()->getSettingValue<bool>("use-sfx-volume")) {
#if defined(GEODE_IS_WINDOWS)
                if (fae->m_sfxVolume != 0)
                    system->playSound(sound, nullptr, false, &channel);
                channel->setVolume(fae->m_sfxVolume * 2.f);
#else
                if (Mod::get()->getSettingValue<int64_t>("volume-slider") != 0)
                    system->playSound(sound, nullptr, false, &channel);
                channel->setVolume(Mod::get()->getSettingValue<int64_t>("volume-slider") / 50.f);
#endif
            }
            else {
                if (Mod::get()->getSettingValue<int64_t>("volume-slider") != 0)
                    system->playSound(sound, nullptr, false, &channel);
                channel->setVolume(Mod::get()->getSettingValue<int64_t>("volume-slider") / 50.f);
            }
        }
        if (!Mod::get()->getSettingValue<bool>("enable-clicksound") && !Mod::get()->getSettingValue<bool>("enable-releasesound")) {
        }
        else {
            Carrot::carrot = true;
        }

        return true;
    }

    // RELEASE SOUNDS RELEASE SOUNDS RELEASE SOUNDS RELEASE SOUNDS RELEASE SOUNDS RELEASE SOUNDS RELEASE SOUNDS RELEASE SOUNDS RELEASE SOUNDS RELEASE
    // SOUNDS RELEASE SOUNDS RELEASE SOUNDS RELEASE SOUNDS

    bool releaseButton(PlayerButton p0)
    {
        if (!PlayerObject::releaseButton(p0))
            return false;

        if (Mod::get()->getSettingValue<bool>("only-on-jump")) {
            if (p0 != PlayerButton::Jump) {
                return false;
            }
        }

        if (!didPushButton)
            return false;
        else
            didPushButton = false;

        if (!GameManager::sharedState()->getPlayLayer() && !GameManager::sharedState()->getEditorLayer())
            return false;

        if (PlayLayer::get())
            if (this == PlayLayer::get()->m_player2 && !PlayLayer::get()->m_level->m_twoPlayerMode)
                return false;
        if (LevelEditorLayer::get())
            if (this == LevelEditorLayer::get()->m_player2 && !LevelEditorLayer::get()->m_level->m_twoPlayerMode)
                return false;

        auto releaseSliderValue = Mod::get()->getSettingValue<int64_t>("releasesound-currentsound");
        auto customReleaseSound = Mod::get()->getSettingValue<std::filesystem::path>("custom-releasesound").string();
        bool usingCustomReleaseSound;
        std::string releaseSoundInUse = Clicks::getReleaseSprite(releaseSliderValue);

        if (releaseSliderValue != 0)
            usingCustomReleaseSound = false;
        if (releaseSoundInUse == "__USECUSTOM__")
            usingCustomReleaseSound = true;

        if (Mod::get()->getSettingValue<bool>("enable-releasesound") && !usingCustomReleaseSound) {
            auto fae    = FMODAudioEngine::sharedEngine();
            auto system = fae->m_system;

            FMOD::Channel *channel;
            FMOD::Sound *sound;

            system->createSound((Mod::get()->getResourcesDir().parent_path() / releaseSoundInUse).string().c_str(), FMOD_DEFAULT, nullptr, &sound);

            if (Mod::get()->getSettingValue<bool>("use-sfx-volume")) {
#if defined(GEODE_IS_WINDOWS)
                if (fae->m_sfxVolume != 0)
                    system->playSound(sound, nullptr, false, &channel);
                channel->setVolume(fae->m_sfxVolume * 2.f);
#else
                if (Mod::get()->getSettingValue<int64_t>("volume-slider") != 0)
                    system->playSound(sound, nullptr, false, &channel);
                channel->setVolume(Mod::get()->getSettingValue<int64_t>("volume-slider") / 50.f);
#endif
            }
            else {
                if (Mod::get()->getSettingValue<int64_t>("volume-slider") != 0)
                    system->playSound(sound, nullptr, false, &channel);
                channel->setVolume(Mod::get()->getSettingValue<int64_t>("volume-slider") / 50.f);
            }
        }

        if (Mod::get()->getSettingValue<bool>("enable-releasesound") && usingCustomReleaseSound) {
            auto fae    = FMODAudioEngine::sharedEngine();
            auto system = fae->m_system;

            FMOD::Channel *channel;
            FMOD::Sound *sound;

            system->createSound(customReleaseSound.c_str(), FMOD_DEFAULT, nullptr, &sound);

            if (Mod::get()->getSettingValue<bool>("use-sfx-volume")) {
#if defined(GEODE_IS_WINDOWS)
                if (fae->m_sfxVolume != 0)
                    system->playSound(sound, nullptr, false, &channel);
                channel->setVolume(fae->m_sfxVolume * 2.f);
#else
                if (Mod::get()->getSettingValue<int64_t>("volume-slider") != 0)
                    system->playSound(sound, nullptr, false, &channel);
                channel->setVolume(Mod::get()->getSettingValue<int64_t>("volume-slider") / 50.f);
#endif
            }
            else {
                if (Mod::get()->getSettingValue<int64_t>("volume-slider") != 0)
                    system->playSound(sound, nullptr, false, &channel);
                channel->setVolume(Mod::get()->getSettingValue<int64_t>("volume-slider") / 50.f);
            }
        }
        if (!Mod::get()->getSettingValue<bool>("enable-clicksound") && !Mod::get()->getSettingValue<bool>("enable-releasesound")) {
        }
        else {
            Carrot::carrot = true;
        }

        return true;
    }
};
