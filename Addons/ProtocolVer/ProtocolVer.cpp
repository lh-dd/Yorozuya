#include "stdafx.h"

#include "ProtocolVer.h"
#include "../../Common/Helpers/RapidHelper.hpp"
#include "../../Common/Helpers/Memory.hpp"

namespace GameServer
{
    namespace Addon
    {
        bool CProtocolVer::m_bActivated = false;

        void CProtocolVer::load()
        {
        }

        void CProtocolVer::unload()
        {
            cleanup_all_hook();
        }

        Yorozuya::Module::ModuleName_t CProtocolVer::get_name()
        {
            static const Yorozuya::Module::ModuleName_t name = "addon.protocol_ver";
            return name;
        }

        void CProtocolVer::configure(const rapidjson::Value& nodeConfig)
        {
            CProtocolVer::m_bActivated = RapidHelper::GetValueOrDefault(nodeConfig, "activated", false);
            if (!CProtocolVer::m_bActivated)
                return;

            std::string value =
                RapidHelper::GetValue<std::string>(
                    nodeConfig,
                    "value");

            if (!CProtocolVer::IsValidHex(value))
                return;

            Memory::WriteMemoryStr(
                0x140110780,
                value);
        }

        bool CProtocolVer::IsValidHex(
            const std::string& value)
        {
            if (value.length() != 8)
                return false;

            for (const char character : value)
            {
                const bool isDigit =
                    character >= '0' &&
                    character <= '9';

                const bool isUpper =
                    character >= 'A' &&
                    character <= 'F';

                const bool isLower =
                    character >= 'a' &&
                    character <= 'f';

                if (!isDigit && !isUpper && !isLower)
                    return false;
            }

            return true;
        }
    }
}
