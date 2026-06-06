#pragma once

#include "ll/api/mod/NativeMod.h"

namespace PlayerDataTrans {

/**
 * @brief Main entry point for the PlayerDataTrans mod.
 *
 * This class manages the mod lifecycle including loading, enabling, and disabling.
 * It coordinates all submodules to provide player data transfer function
 */
class Entry {

public:

    /**
     * @brief Gets the singleton instance of Entry.
     *
     * @return Entry& Reference to the singleton instance.
     */
    static Entry& getInstance();

    Entry() : mSelf(*ll::mod::NativeMod::current()) {}

    /**
     * @brief Gets the native mod instance.
     *
     * @return ll::mod::NativeMod& Reference to the native mod.
     */
    [[nodiscard]] ll::mod::NativeMod& getSelf() const { return mSelf; }

    /**
     * @brief Loads the mod
     *
     * This method is called when the mod is loaded.
     *
     * @return true if the mod loaded successfully.
     * @return false if the mod failed to load.
     */
    bool load();

    /**
     * @brief Enables the mod and starts all services.
     *
     * This method is called when the mod is enabled.
     * It registers commands
     *
     * @return true if the mod was enabled successfully.
     * @return false if the mod failed to enable.
     */
    bool enable();

    /**
     * @brief Disables the mod and stops all services.
     *
     * This method is called when the mod is disabled.
     *
     * @return true if the mod was disabled successfully.
     * @return false if the mod failed to disable.
     */
    bool disable();

    /**
     * @brief Unloads the mod
     *
     * This method is called when the mod is unloaded.
     *
     * @return true if the mod unloaded successfully.
     * @return false if the mod failed to unload.
     */
    bool unload();

private:
    ll::mod::NativeMod& mSelf;
};

} // namespace PlayerDataTrans
