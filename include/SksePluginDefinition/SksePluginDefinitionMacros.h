#pragma once

#include <RE/Skyrim.h>
#include <REL/Version.h>
#include <SKSE/SKSE.h>

#include "./SksePluginDefinition.h"  // IWYU pragma: export

/**
 * Helper macro to allow passing version as arguments that will be passes to REL::Version
 * e.g. (0, 0, 1) or ("0", "0", "1")
 *
 * Note that we do not parse "0.0.1" as it would require a more complex macro (though possible)
 */

#define __SKSE_PLUGIN_DEFINITION__UNPACK(...) __VA_ARGS__

/**
 * Given the information provided about the SKSE plugin, this defined a SksePluginDefinition
 * namespace that contains the plugin name, author name, author email and version.
 *
 * This namespace is specific to this "skse-plugin-definition" package and
 * is not generic to all SKSE / CommonLibSSE plugins.
 *
 * It does however allow for all plugins which use this package to have a consistent way
 * of accessing the plugin information at runtime, if needed.
 */

#define __SKSE_PLUGIN_DEFINITION(pluginName, pluginVersion, authorName, authorEmail) \
    namespace SksePluginDefinition {                                                 \
        const char*        GetPluginName() { return pluginName; }                    \
        const char*        GetAuthorName() { return authorName; }                    \
        const char*        GetAuthorEmail() { return authorEmail; }                  \
        const REL::Version GetPluginVersion() {                                      \
            return REL::Version(__SKSE_PLUGIN_DEFINITION__UNPACK pluginVersion);     \
        }                                                                            \
    }

#if !HAS_SKYRIM_MULTI_TARGETING
    #define __SKSE_PLUGIN_DEFINITION_QUERY_INTERFACE(pluginName, pluginVersion)    \
        extern "C" __declspec(dllexport) bool SKSEPlugin_Query(                    \
            const SKSE::QueryInterface* a_skse, SKSE::PluginInfo* a_info           \
        ) {                                                                        \
            a_info->infoVersion = SKSE::PluginInfo::kVersion;                      \
            a_info->name        = SksePluginDefinition::GetPluginName();           \
            a_info->version     = SksePluginDefinition::GetPluginVersion().pack(); \
            if (a_skse->IsEditor()) return false;                                  \
            return true;                                                           \
        }
#else
    #define __SKSE_PLUGIN_DEFINITION_QUERY_INTERFACE(pluginName, pluginVersion)
#endif

#if SKYRIM_SUPPORT_AE && !HAS_SKYRIM_MULTI_TARGETING
    #define __SKSE_PLUGIN_DEFINITION_VERSION_INTERFACE(                             \
        pluginName, pluginVersion, authorName, authorEmail                          \
    )                                                                               \
        extern "C" __declspec(dllexport) constinit auto SKSEPlugin_Version = []() { \
            SKSE::PluginVersionData v{};                                            \
            v.PluginVersion(SksePluginDefinition::GetPluginVersion());              \
            v.PluginName(SksePluginDefinition::GetPluginName());                    \
            v.AuthorName(SksePluginDefinition::GetAuthorName());                    \
            v.AuthorEmail(SksePluginDefinition::GetAuthorEmail());                  \
            v.UsesAddressLibrary();                                                 \
            v.UsesUpdatedStructs();                                                 \
            return v;                                                               \
        }();
#else
    #define __SKSE_PLUGIN_DEFINITION_VERSION_INTERFACE(    \
        pluginName, pluginVersion, authorName, authorEmail \
    )
#endif

/**
 * Macros providing default parameters so that you can call DEFINE_SKSE_PLUGIN with 1 to 4
 * arguments:
 */

#define __DEFINE_SKSE_PLUGIN_1(pluginName)                              \
    __SKSE_PLUGIN_DEFINITION(pluginName, (0, 0, 1), "", "")             \
    __SKSE_PLUGIN_DEFINITION_QUERY_INTERFACE(pluginName, pluginVersion) \
    __SKSE_PLUGIN_DEFINITION_VERSION_INTERFACE(pluginName, pluginVersion, authorName, authorEmail)

#define __DEFINE_SKSE_PLUGIN_2(pluginName, pluginVersion)               \
    __SKSE_PLUGIN_DEFINITION(pluginName, pluginVersion, "", "")         \
    __SKSE_PLUGIN_DEFINITION_QUERY_INTERFACE(pluginName, pluginVersion) \
    __SKSE_PLUGIN_DEFINITION_VERSION_INTERFACE(pluginName, pluginVersion, authorName, authorEmail)

#define __DEFINE_SKSE_PLUGIN_3(pluginName, pluginVersion, authorName)   \
    __SKSE_PLUGIN_DEFINITION(pluginName, pluginVersion, authorName, "") \
    __SKSE_PLUGIN_DEFINITION_QUERY_INTERFACE(pluginName, pluginVersion) \
    __SKSE_PLUGIN_DEFINITION_VERSION_INTERFACE(pluginName, pluginVersion, authorName, authorEmail)

#define __DEFINE_SKSE_PLUGIN_4(pluginName, pluginVersion, authorName, authorEmail) \
    __SKSE_PLUGIN_DEFINITION(pluginName, pluginVersion, authorName, authorEmail)   \
    __SKSE_PLUGIN_DEFINITION_QUERY_INTERFACE(pluginName, pluginVersion)            \
    __SKSE_PLUGIN_DEFINITION_VERSION_INTERFACE(pluginName, pluginVersion, authorName, authorEmail)

/**
 * Helper macros to choose the correct macro based on the number of arguments passed to
 * DEFINE_SKSE_PLUGIN:
 */

#define __SKSE_PLUGIN_DEFINITION__GET_ARG_COUNT(_1, _2, _3, _4, COUNT, ...) COUNT
#define __SKSE_PLUGIN_DEFINITION__CONCATENATE(NAME, COUNT) NAME##COUNT
#define __SKSE_PLUGIN_DEFINITION__CHOOSE_MACRO(NAME, COUNT) \
    __SKSE_PLUGIN_DEFINITION__CONCATENATE(NAME, COUNT)

/**
 * The main macro that you should use to define your plugin:
 */

#define DEFINE_SKSE_PLUGIN(...)                                                                                                      \
    __SKSE_PLUGIN_DEFINITION__CHOOSE_MACRO(__DEFINE_SKSE_PLUGIN_, __SKSE_PLUGIN_DEFINITION__GET_ARG_COUNT(__VA_ARGS__, 4, 3, 2, 1))( \
        __VA_ARGS__                                                                                                                  \
    )

/**
 * The entry point macro that you should use to define the entry point of your plugin:
 */

#define SKSE_PLUGIN_ENTRY_POINT extern "C" __declspec(dllexport) bool SKSEPlugin_Load
