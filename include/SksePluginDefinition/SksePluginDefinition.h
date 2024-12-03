#pragma once

#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>


namespace SksePluginDefinition {
    const char*        GetPluginName();
    const char*        GetAuthorName();
    const char*        GetAuthorEmail();
    const REL::Version GetPluginVersion();
}