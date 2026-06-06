#include "PlayerDataTrans/Entry.h"
#include "Command.h"
#include "ll/api/mod/RegisterHelper.h"

namespace PlayerDataTrans {


Entry& Entry::getInstance() {
    static Entry instance;
    return instance;
}

bool Entry::load() {
    return true;
}

bool Entry::enable() {
    registerCommands();
    return true;
}

bool Entry::disable() {
    return true;
}

bool Entry::unload() {
    return true;
}

} // namespace PlayerDataTrans
LL_REGISTER_MOD(PlayerDataTrans::Entry, PlayerDataTrans::Entry::getInstance());
