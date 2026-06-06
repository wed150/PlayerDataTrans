# PlayerDataTrans
**全面玩家(背包 插件)数据迁移 & 定时背包备份工具（LeviLamina）**

> 📦 适用于 LeviLamina 服务端  
> 🛡️ 支持跨存档玩家数据迁移  
> 📦 底层数据库实现，做到多插件适配
> ⏰ 可配合定时任务插件实现自动玩家背包 / 数据备份

---

## ✨ 功能简介

PlayerDataTrans 是一个玩家数据库级数据迁移工具，主要能力包括：

- ✅ 完整迁移玩家数据
    - 玩家背包
    - 末影箱
    - 属性 / 状态
    - 插件存储信息(可手动删插件数据选择性储存)
- ✅ 导出为本地 .bin 文件
- ✅ 从本地文件恢复玩家数据
- ✅ 支持离线玩家
- ✅ 可配合定时任务插件实现自动备份

>[!NOTE]
> ⚠️ 本插件操作的是 DBStorage 层数据，属于底层迁移，比单纯复制文件更安全、更全面。

---

## 📌 使用场景

| 场景 | 说明 |
|----|----|
| 🌍 换存档 | 老存档 → 新存档，玩家数据不丢失 |
| 🔄 服主迁移 | BDS 重装 / 地图大改 |
| 💾 定期备份 | 防止回档导致玩家物品消失 |
| 🧩 多插件环境 | 解决插件不识别玩家的问题 |

---

## 📖 命令说明

### /transfer_data backup
**备份当前存档的所有玩家数据**

- 导出路径：
  ./plugins/PlayerDataTrans/playerdb/

✅ 在执行后关闭服务器 备份./plugins/levilamina/playerinfo 更换存档 插件 删除部分插件数据

---

### /transfer_data restore
**从备份文件恢复玩家数据**

- 自动读取 playerdb 目录下的所有 .bin 文件
- 写入当前存档的 DBStorage

✅ 可用于新存档首次启动后恢复

---

## ⏰ 定时玩家背包备份方案

本插件本身不内置定时器，但可以非常方便地配合定时任务插件使用。

### 示例（概念配置）
每天 02:00 执行：
transfer_data backup

搭配插件示例：
- ✅ GMEssential

📌 实际效果：
> 每天自动生成一个玩家数据快照  
> 即使服务器回档，也能找回玩家背包

---


## ⚠️ 注意事项（非常重要）

1. ❗ 功能尚不完善，请手动保留以下内容：  
   ./plugins/levilamina/playerinfo
> [!TIP]
> 否则部分插件可能无法识别玩家名称(不影响使用 只是名称乱码 玩家上线一下就好)
2. ❗ 不同 Minecraft 版本之间迁移可能存在兼容风险

---

## 🧠 工作原理（简要）

DBStorage (player_*)
↓  backup
.bin files
↓  restore
DBStorage (new world)

- 基于 ll::service::getDBStorage()
- 使用 KeyValueDB 级别读写
- 不依赖玩家在线状态

---


## 🙏 鸣谢

- [小小的子沐呀](https://github.com/zimuya4153)