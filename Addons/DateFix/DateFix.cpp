#include "stdafx.h"

#include "DateFix.h"
#include <time.h>

namespace GameServer
{
    namespace Addon
    {
        bool CDateFix::m_bActivated = false;

        unsigned int WINAPI CDateFix::GetKorLocalTime_Hook(
            ATF::Global::Info::GetKorLocalTime574_ptr next)
        {
            time_t now = time(nullptr);
            struct tm tm {};

            if (localtime_s(&tm, &now) != 0)
                return next();

            return
                (tm.tm_year % 100) * 100000000 +
                (tm.tm_mon + 1) * 1000000 +
                tm.tm_mday * 10000 +
                tm.tm_hour * 100 +
                tm.tm_min;
        }

        unsigned int WINAPI CDateFix::GetConnectTime_AddBySec_Hook(
            int iSec,
            ATF::Global::Info::GetConnectTime_AddBySec464_ptr next)
        {
            time_t now = time(nullptr);
            struct tm tm {};

            if (localtime_s(&tm, &now) != 0)
                return next(iSec);

            tm.tm_sec += iSec;
            time_t adjusted = mktime(&tm);
            if (adjusted == -1)
                return next(iSec);

            localtime_s(&tm, &adjusted);

            return
                (tm.tm_year % 100) * 100000000 +
                (tm.tm_mon + 1) * 1000000 +
                tm.tm_mday * 10000 +
                tm.tm_hour * 100 +
                tm.tm_min;
        }

        void CDateFix::load()
        {
            if (!m_bActivated)
                return;

            enable_hook(&ATF::Global::GetKorLocalTime, &GetKorLocalTime_Hook);
            enable_hook(&ATF::Global::GetConnectTime_AddBySec, &GetConnectTime_AddBySec_Hook);
        }

        void CDateFix::unload()
        {
            cleanup_all_hook();
        }

        Yorozuya::Module::ModuleName_t CDateFix::get_name()
        {
            static const Yorozuya::Module::ModuleName_t name = "addon.datefix";
            return name;
        }

        void CDateFix::configure(const rapidjson::Value& nodeConfig)
        {
            m_bActivated = RapidHelper::GetValueOrDefault(nodeConfig, "activated", false);
        }
    }
}
