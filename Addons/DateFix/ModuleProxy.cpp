#include "stdafx.h"

#include "DateFix.h"
#include "../../Common/Helpers/ModuleProxy.hpp"
#include "../../Common/Interfaces/ModuleInterface.h"

using namespace GameServer::Addon;

#define DllExport __declspec(dllexport)

extern "C" DllExport
Yorozuya::Module::IModule* CreateModule()
{
    return ModuleProxy::CModuleProxy<CDateFix>::get_instance()->CreateModule();
}

extern "C" DllExport
void ReleaseModule(Yorozuya::Module::IModule* pObj)
{
    ModuleProxy::CModuleProxy<CDateFix>::get_instance()->ReleaseModule(pObj);
}

extern "C" DllExport
uint32_t GetYorozuyaAddonSignature()
{
    return Yorozuya::ModuleApi::ADDON_SIGNATURE;
}
