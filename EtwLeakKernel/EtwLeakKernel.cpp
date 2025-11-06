#include "pch.h"
#include "EtwProvider.h"
#include "EtwCallbacks.h"

int main() {
	EtwProvider etwProvider(L"MyProvider", L"Microsoft-Windows-Kernel-Network", 60);
	etwProvider.Register(ProviderCallback, EVENT_ENABLE_PROPERTY_STACK_TRACE);

	if (!etwProvider.Consume()) {
		std::cerr << "[-] Failed to start ETW consumption." << std::endl;
		return 1;
	}
	std::cout << "[+] ETW consumption finished." << std::endl;
    return 0;
}
