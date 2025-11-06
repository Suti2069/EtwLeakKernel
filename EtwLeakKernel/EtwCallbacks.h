#pragma once
#include "pch.h"

constexpr auto IsValidKernelAddress = [](DWORD64 address) -> bool {
	return (address > 0x8000000000000000 && address < 0xFFFFFFFFFFFFFFFF);
};

void ProviderCallback(_In_ const EVENT_RECORD& record, _In_ const krabs::trace_context& traceContext);