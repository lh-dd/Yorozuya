#pragma once

#include "../../Common/Interfaces/ModuleInterface.h"
#include "../../Common/Helpers/ModuleHook.hpp"

namespace GameServer
{
    namespace Addon
    {
        class CProtocolVer
            : public Yorozuya::Module::IModule
            , CModuleHook
        {
        public:
            virtual void load() override;
            virtual void unload() override;
            virtual Yorozuya::Module::ModuleName_t get_name() override;
            virtual void configure(const rapidjson::Value& nodeConfig) override;

        private:
            static bool m_bActivated;

        private:
            static bool IsValidHex(const std::string& value);
        };
    }
}
