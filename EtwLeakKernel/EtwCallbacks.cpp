#include "pch.h"
#include "EtwCallbacks.h"

void ProviderCallback(_In_ const EVENT_RECORD& record, _In_ const krabs::trace_context& traceContext) {
	krabs::schema schema(record, traceContext.schema_locator);
	krabs::parser parser(schema);
	std::vector<DWORD64> stackTrace = schema.stack_trace();

	std::cout << "----------------------------------------" << std::endl;

	for (DWORD64 address : stackTrace) {
		if (IsValidKernelAddress(address))
			std::cout << "[*] Leaked kernel address: 0x" << std::hex << address << std::dec << std::endl;
	}
	std::cout << "----------------------------------------" << std::endl;
}