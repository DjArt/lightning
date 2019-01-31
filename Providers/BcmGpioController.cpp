#include "pch.h"

#include "ErrorCodes.h"
#include "GpioController.h"
#include "DmapSupport.h"

using namespace Windows::Devices::Custom;
using namespace Windows::Storage::Streams;

#if defined(_M_ARM)
// 
// Global extern exports
//
BcmGpioControllerClass g_bcmGpio;

/**
\return HRESULT success or error code.
*/
HRESULT BcmGpioControllerClass::_mapController()
{
	HRESULT hr = S_OK;
	PVOID baseAddress = nullptr;

	hr = GetControllerBaseAddress(
		pi2GpioDeviceName,
		m_hController,
		baseAddress,
		FILE_SHARE_READ | FILE_SHARE_WRITE);

	if (SUCCEEDED(hr))
	{
		m_registers = (PBCM_GPIO)baseAddress;
	}

	return hr;
}

HRESULT BcmGpioControllerClass::mapIfNeeded()
{
	HRESULT hr = S_OK;

	if (m_hController == INVALID_HANDLE_VALUE)
	{
		hr = _mapController();
	}

	return hr;
}
/// Method to attach to an interrupt on a GPIO port bit.
HRESULT BcmGpioControllerClass::attachInterrupt(ULONG intNo, std::function<void(void)> func, ULONG mode)
{
	HRESULT hr = S_OK;

	hr = mapIfNeeded();

	// Tell the driver to attach the interrupt.
	if (SUCCEEDED(hr))
	{
		hr = m_gpioInterrupts.attachInterrupt(intNo, func, mode, m_hController);
	}

	return hr;
}
/// Method to attach to an interrupt on a GPIO port bit with informaton return.
HRESULT BcmGpioControllerClass::attachInterruptEx(ULONG intNo, std::function<void(PDMAP_WAIT_INTERRUPT_NOTIFY_BUFFER)> func, ULONG mode)
{
	HRESULT hr = S_OK;

	hr = mapIfNeeded();

	// Tell the driver to attach the interrupt.
	if (SUCCEEDED(hr))
	{
		hr = m_gpioInterrupts.attachInterruptEx(intNo, func, mode, m_hController);
	}

	return hr;
}

/// Method to attach to an interrupt on a GPIO port bit with informaton return and context.
HRESULT BcmGpioControllerClass::attachInterruptContext(ULONG intNo, std::function<void(PDMAP_WAIT_INTERRUPT_NOTIFY_BUFFER, PVOID)> func, PVOID context, ULONG mode)
{
	HRESULT hr = S_OK;

	hr = mapIfNeeded();

	// Tell the driver to attach the interrupt.
	if (SUCCEEDED(hr))
	{
		hr = m_gpioInterrupts.attachInterruptContext(intNo, func, context, mode, m_hController);
	}

	return hr;
}

/// Method to detach an interrupt for a GPIO port bit.
HRESULT BcmGpioControllerClass::detachInterrupt(ULONG intNo)
{
	HRESULT hr = S_OK;

	hr = mapIfNeeded();

	// Tell the driver to detach the interrupt.
	if (SUCCEEDED(hr))
	{
		hr = m_gpioInterrupts.detachInterrupt(intNo, m_hController);
	}

	return hr;
}
#endif // defined(_M_ARM)