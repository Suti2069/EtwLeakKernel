#pragma once
#include "pch.h"

constexpr DWORD MILISECONDS_IN_SECONDS = 1000;

class EtwProvider {
private:
	krabs::user_trace trace;
	krabs::provider<> provider;
	DWORD sessionDuration;
	HANDLE consumer;
	bool consuming;

public:
	EtwProvider(_In_ const std::wstring& traceName, const std::wstring& providerName, _In_ DWORD duration) noexcept;
	~EtwProvider();

	void Register(_In_ krabs::event_filter& filter, _In_ DWORD64 flags);
	void Register(_In_ krabs::c_provider_callback callback, _In_ DWORD64 flags);
	DWORD StartProviderTrace();
	bool Consume();
	void StopConsumption();

	bool IsConsuming() const {
		return consuming;
	}
};

