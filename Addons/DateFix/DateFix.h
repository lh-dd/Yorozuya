#pragma once

#include "../../Common/Interfaces/ModuleInterface.h"
#include "../../Common/Helpers/ModuleHook.hpp"
#include "../../Common/Helpers/RapidHelper.hpp"

namespace GameServer
{
    namespace Addon
    {
        class CDateFix
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
            static unsigned int WINAPI GetKorLocalTime_Hook(
                ATF::Global::Info::GetKorLocalTime574_ptr next
            );

            static unsigned int WINAPI GetConnectTime_AddBySec_Hook(
                int iSec,
                ATF::Global::Info::GetConnectTime_AddBySec464_ptr next
            );
        };
    }
}
