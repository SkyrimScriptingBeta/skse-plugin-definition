#include <SksePluginDefinition.h>

DEFINE_SKSE_PLUGIN("MyExampleSksePluginFromDefinition")

SKSE_PLUGIN_ENTRY_POINT(const SKSE::LoadInterface* skse) {
    SKSE::Init(skse);

    SKSE::GetMessagingInterface()->RegisterListener([](SKSE::MessagingInterface::Message* a_msg) {
        if (a_msg->type == SKSE::MessagingInterface::kDataLoaded) {
            const auto thisPluginName = SksePluginDefinition::GetPluginName();
            RE::ConsoleLog::GetSingleton()->Print(
                ("Hello from MyExampleSksePluginFromDefinition "s + thisPluginName).c_str()
            );
        }
    });

    return true;
}
