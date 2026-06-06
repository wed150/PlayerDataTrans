#pragma once
#include "mc/server/commands/CommandOutput.h"
#include "ll/api/command/CommandHandle.h"


namespace PlayerDataTrans {

/**
 * @brief transfer_data command actions
 */
enum TransferOperation : int {
    backup,
    restore
};

/**
 * @brief Structure for the transfer_data command overload parameters.
 */
struct DataTransferOverload {
    TransferOperation action;// "backup" to backup, "restore" to restore
};

/**
 * @brief Registers all commands provided by this mod.
 * 
 * Current registers the 'transfer_data' command which allows administrators
 * to manage the player data transfer process.
 */
void registerCommands();

} // namespace TrueIPGetter
