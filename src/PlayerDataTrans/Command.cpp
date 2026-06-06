#include "Command.h"
#include "Entry.h"

#include "mc/world/level/Level.h"
#include "mc/world/actor/player/Player.h"
#include "mc/world/level/storage/DBStorage.h"

#include "ll/api/service/Bedrock.h"
#include "ll/api/service/Bedrock.h"
#include "ll/api/data/KeyValueDB.h"
#include "ll/api/io/FileUtils.h"

using namespace std::literals;

namespace PlayerDataTrans {

/**
 * @brief Registers the player data transfer command.
 *
 * This command allows server administrators to:
 *  - Backup player data from DBStorage to local files
 *  - Restore player data from local files into DBStorage
 */
void registerCommands() {
    // Register the /transfer_data command
    auto& command = ll::command::CommandRegistrar::getInstance(false)
                        .getOrCreateCommand(
                            "transfer_data",
                            "玩家数据迁移备份还原",
                            CommandPermissionLevel::GameDirectors
                        );

    /**
     * @brief Command execution logic for player data backup and restore.
     */
    command.overload<DataTransferOverload>()
        .required("action")
        .execute([](CommandOrigin const& origin,
                    CommandOutput& output,
                    DataTransferOverload const& overload) {

            int successes = 0; ///< Number of successful operations
            int failures  = 0; ///< Number of failed operations

            /* ================= Backup Mode ================= */
            if (overload.action == backup) {
                output.success("开始备份");

                /**
                 * @brief Iterate over all player records stored in DBStorage.
                 *
                 * Each player's data is exported as a binary (.bin) file
                 * under ./plugins/PlayerDataTrans/playerdb/
                 */
                ll::service::getDBStorage()->forEachKeyWithPrefix(
                    "player_",
                    DBHelpers::Category::Player,
                    [&successes, &failures]
                    (std::string_view key_left, std::string_view data) {
                        std::string path = "./plugins/PlayerDataTrans/playerdb/player_";
                        path.append(key_left);
                        path.append(".bin");

                        bool isSaved = ll::file_utils::writeFile(
                            path,
                            data,
                            true
                        );

                        isSaved ? ++successes : ++failures;
                    }
                );

                /**
                 * @brief Handle the special offline/local player entry.
                 */
                if (ll::service::getDBStorage()->hasKey("~local_player", DBHelpers::Category::Player)) {
                    ll::service::getDBStorage()->_read(
                        "~local_player",
                        DBHelpers::Category::Player,
                        [&successes, &failures]
                        (std::string_view, std::string_view data) {
                            bool isSaved = ll::file_utils::writeFile(
                                "./plugins/PlayerDataTrans/playerdb/~local_player.bin",
                                data,
                                true
                            );
                            isSaved ? ++successes : ++failures;
                        }
                    );
                } else {
                    ++failures;
                }
            }

            /* ================= Restore Mode ================= */
            else if (overload.action == restore) {
                output.success("开始还原");

                /**
                 * @brief Scan the playerdb directory and restore all .bin files.
                 *
                 * Each file is imported back into DBStorage using its filename
                 * (without extension) as the player identifier.
                 */
                std::for_each(
                    std::filesystem::begin(
                        std::filesystem::directory_iterator(
                            "./plugins/PlayerDataTrans/playerdb"
                        )
                    ),
                    std::filesystem::end(
                        std::filesystem::directory_iterator(
                            "./plugins/PlayerDataTrans/playerdb"
                        )
                    ),
                    [&successes, &failures]
                    (const std::filesystem::directory_entry& e) {
                        if (e.is_regular_file() &&
                            e.path().extension() == ".bin") {
                            try {
                                if (auto data = ll::file_utils::readFile(e, true)) {
                                    ll::service::getDBStorage()->saveData(
                                        e.path().stem().string(),
                                        std::move(data.value()),
                                        DBHelpers::Category::Player
                                    );
                                    ++successes;
                                } else {
                                    ++failures;
                                }
                            } catch (...) {
                                ++failures;
                            }
                        }
                    }
                );
            }

            /* ================= Output Summary ================= */
            output.success("成功迁移玩家数据"+std::to_string(successes)+"个");
            output.success("在"+std::to_string(failures)+"个玩家数据传输中出现错误");
            /**
             * @brief Post-operation notice for backup mode.
             */
            overload.action==backup?output.success("如果您正在迁移存档中玩家数据，请关闭服务器，由于功能不完善 您最好手动保留./plugins/levilamina/playerinfo文件夹的内容，可以增加您的体验(不复制可能有些插件不知道玩家名了)，随后更换至新存档(和插件)，启动使用/transfer_data restore恢复数据"):output.success("感谢您的使用");

        });
}

} // namespace PlayerDataTrans
