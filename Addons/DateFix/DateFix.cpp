#include "stdafx.h"

#include "DateFix.h"
#include "../../Common/Helpers/Memory.hpp"

namespace GameServer
{
    namespace Addon
    {
        bool CDateFix::m_bActivated = false;

        unsigned int WINAPIV CDateFix::GetKorLocalTime_Hook()
        {
            char szDate[32], szTime[32];
            _strdate(szDate);
            _strtime(szTime);

            char szLocalTime[32]{ 0 };

            szLocalTime[0] = szDate[6];
            szLocalTime[1] = szDate[7];

            szLocalTime[2] = szDate[0];
            szLocalTime[3] = szDate[1];

            szLocalTime[4] = szDate[3];
            szLocalTime[5] = szDate[4];

            szLocalTime[6] = szTime[0];
            szLocalTime[7] = szTime[1];

            szLocalTime[8] = szTime[3];
            szLocalTime[9] = szTime[4];

            szLocalTime[10] = NULL;

            return (unsigned int)atoll(szLocalTime);
        }

        unsigned int WINAPIV CDateFix::GetConnectTime_AddBySec_Hook(int iSec)
        {
            struct tm* Tm;
            time_t origTime;

            time(&origTime);
            Tm = localtime(&origTime);

            Tm->tm_sec += iSec;
            mktime(Tm);

            char Dest[32]{ 0 };

            sprintf(
                Dest,
                "%01d%02d%02d%02d%02d",
                (unsigned int)(Tm->tm_year - 100),
                (unsigned int)(Tm->tm_mon + 1),
                (unsigned int)(Tm->tm_mday),
                (unsigned int)(Tm->tm_hour),
                (unsigned int)(Tm->tm_min));

            return (unsigned int)atoll(Dest);
        }

        void CDateFix::load()
        {
            if (!m_bActivated)
                return;

            enable_hook(&ATF::Global::GetKorLocalTime, &GetKorLocalTime_Hook);
            enable_hook(&ATF::Global::GetConnectTime_AddBySec, &GetConnectTime_AddBySec_Hook);

            Memory::WriteMemoryStr(0x14074EF86, "75"); //LastConnTime
            Memory::WriteMemoryStr(0x1407502F1, "75"); //[LastConnTime]
            Memory::WriteMemoryStr(0x140750588, "75"); //LastLogoutTime
            Memory::WriteMemoryStr(0x14048EA6B, "E5FF"); //BIGINT
            Memory::WriteMemoryStr(0x1404C4AB2, "E5FF"); //BIGINT
            Memory::WriteMemoryStr(0x1404C7E14, "75"); //Jmp Query

            //Time -> %d.log
            const unsigned __int64 Address_dLog[69] =
            {
                0x140709C7F, 0x140726DE1, 0x140737890, 0x1407378C1, 0x14076B8AF,
                0x14076B90C, 0x14076B93C, 0x14076B970, 0x14076B99B, 0x14076B9CA,
                0x14076B9F3, 0x14076BA26, 0x14076BA52, 0x14076BA79, 0x14076BAA1,
                0x14076BACF, 0x14076BAF9, 0x14076BB20, 0x14076BB4C, 0x14076BB82,
                0x1407708A4, 0x1407727AC, 0x140794955, 0x1407A603C, 0x1407A6075,
                0x1407AE4BA, 0x1407B93EB, 0x1407B942A, 0x1407BA8D0, 0x1407BB0DA,
                0x1407BBACD, 0x1407BCD00, 0x1407BD4BD, 0x1407BDD22, 0x1407BE444,
                0x1407C01B6, 0x1407C5A28, 0x1407C5A6B, 0x1407C7CBE, 0x1407C7CFD,
                0x1407C930E, 0x1407CAD05, 0x1407CAD3E, 0x1407D1E21, 0x1407D1E3D,
                0x1407D1FED, 0x1407D8A69, 0x1407D8BC5, 0x1407E4451, 0x1407E72FB,
                0x1407EF87B, 0x1407F1689, 0x14081574A, 0x140818FBD, 0x140818FFB,
                0x14081906D, 0x1408190AB, 0x140825D15, 0x1408299AF, 0x14082A76F,
                0x14082C7FE, 0x14082C836, 0x14083EC4E, 0x14084E1FE, 0x14084E216,
                0x14084E230, 0x14084E246, 0x1408501C2, 0x1408501D7
            };

            for (int i = 0; i < 69; i++)
                Memory::WriteMemoryStr(Address_dLog[i], "75");

            //Time -> %d_A.log
            Memory::WriteMemoryStr(0x140853C56, "75");
            Memory::WriteMemoryStr(0x140853C94, "75");
            Memory::WriteMemoryStr(0x140853E66, "75");
            Memory::WriteMemoryStr(0x140853EA4, "75");

            //Time -> %d_U.log
            Memory::WriteMemoryStr(0x140853C36, "75");
            Memory::WriteMemoryStr(0x140853C74, "75");
            Memory::WriteMemoryStr(0x140853E47, "75");
            Memory::WriteMemoryStr(0x140853E84, "75");

            //Time -> Shop_%d.sys, Shop_%d.prc, %s\Guild   %s\%d
            Memory::WriteMemoryStr(0x1407D1DF9, "75");
            Memory::WriteMemoryStr(0x1407D1E09, "75");
            Memory::WriteMemoryStr(0x140793790, "75");
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
