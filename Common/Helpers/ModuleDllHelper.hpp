#pragma once

#include <Windows.h>
#include <filesystem>

#include "../Common/Interfaces/ModuleInterface.h"

namespace ModuleDllHelper
{
    namespace fs = ::std::experimental::filesystem::v1;
    using namespace Yorozuya;
    
    class CModuleDll : public Module::IModule
    {
    public:
        CModuleDll(const fs::path& dll_path)
        {
            m_hDll = LoadLibraryW(dll_path.generic_wstring().c_str());
            assert(m_hDll != NULL);

            auto fnGetAddonSignature =
                (ModuleApi::GetAddonSignature_ptr)
                GetProcAddress(
                    m_hDll,
                    ModuleApi::csNameGetAddonSignature);

            if (fnGetAddonSignature == NULL ||
                fnGetAddonSignature() != ModuleApi::ADDON_SIGNATURE)
            {
                FreeLibrary(m_hDll);
                m_hDll = NULL;

                ATF::Global::MyMessageBox("GetAddonSignature", "Addon folder contains incompatible dlls");
                throw std::runtime_error("GetAddonSignature - missing addon signature");
            }

            m_fnCreateModule = (ModuleApi::CreateModule_ptr)GetProcAddress(m_hDll, ModuleApi::csNameCreateModule);
            m_fnReleaseModule = (ModuleApi::ReleaseModule_ptr)GetProcAddress(m_hDll, ModuleApi::csNameReleaseModule);

            m_impl = std::move(
                Module::Module_ptr(
                    m_fnCreateModule(),
                    [fnRelease = m_fnReleaseModule](Module::IModule* obj) {
                        fnRelease(obj);
                    }));
        };

        virtual ~CModuleDll()
        {
            m_impl.reset();

            if (m_hDll != NULL)
            {
                FreeLibrary(m_hDll);
                m_hDll = NULL;
            }
        }

        virtual void load() override
        {
            m_impl->load();
        }

        virtual void unload() override
        {
            m_impl->unload();
        }

        virtual void loop() override
        {
            m_impl->loop();
        }

        virtual void zone_start() override
        {
            return m_impl->zone_start();
        }

        virtual Module::ModuleVersion_t get_version() override
        {
            return m_impl->get_version();
        }

        virtual Module::ModuleName_t get_name() override
        {
            return m_impl->get_name();
        }

        virtual void configure(const rapidjson::Value& nodeConfig) override
        {
            m_impl->configure(nodeConfig);
        }

    private:
        HMODULE m_hDll;
        Module::Module_ptr m_impl;
        ModuleApi::CreateModule_ptr m_fnCreateModule;
        ModuleApi::ReleaseModule_ptr m_fnReleaseModule;
    };
}
