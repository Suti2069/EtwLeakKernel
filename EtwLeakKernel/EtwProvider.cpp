#include "pch.h"
#include "EtwProvider.h"

DWORD WINAPI ConsumerThread(_In_ LPVOID lpParam) {
	EtwProvider* etwProvider = static_cast<EtwProvider*>(lpParam);

	if (!etwProvider) {
		return 1;
	}
	return etwProvider->StartProviderTrace();
}

EtwProvider::EtwProvider(_In_ const std::wstring& traceName, const std::wstring& providerName, _In_ DWORD duration) noexcept
	: trace(traceName), provider(providerName), sessionDuration(duration), consuming(false), consumer(NULL) {
}

EtwProvider::~EtwProvider() {
	StopConsumption();
}

void EtwProvider::Register(_In_ krabs::event_filter& filter, _In_ DWORD64 flags) {
	provider.add_filter(filter);

	if (flags != 0)
		provider.trace_flags(provider.trace_flags() | flags);
	trace.enable(provider);
}

void EtwProvider::Register(_In_ krabs::c_provider_callback callback, _In_ DWORD64 flags) {
	provider.add_on_event_callback(callback);

	if (flags != 0)
		provider.trace_flags(provider.trace_flags() | flags);
	trace.enable(provider);
}


bool EtwProvider::Consume() {
	if (consuming)
		return false;
	consumer = CreateThread(NULL, 0, ConsumerThread, this, 0, NULL);

	if (!consumer || consumer == INVALID_HANDLE_VALUE) {
		std::cerr << "[-] Failed to create consumer thread. Error: " << GetLastError() << std::endl;
		return false;
	}
	consuming = true;
	Sleep(sessionDuration * MILISECONDS_IN_SECONDS);
	StopConsumption();
	return true;
}

void EtwProvider::StopConsumption() {
	if (consuming) {
		trace.stop();

		if (consumer && consumer != INVALID_HANDLE_VALUE) {
			WaitForSingleObject(consumer, INFINITE);
			CloseHandle(consumer);
			consumer = NULL;
		}
		consuming = false;
	}
}

DWORD EtwProvider::StartProviderTrace() {
	try {
		trace.start();
	}
	catch (const krabs::need_to_be_admin_failure&) {
		std::cerr << "[-] Insuffice privileges, rerun as admin" << std::endl;
		return 1;
	}
	return 0;
}