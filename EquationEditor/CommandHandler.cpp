#include <UIRibbon.h>

#include "CommandHandler.h"
#include "RibbonFramework.h"
#include "RibbonIds.h"
#include "resource.h"
// Статический метод создания экземпляра объекта.
__checkReturn HRESULT CCommandHandler::CreateInstance(__deref_out IUICommandHandler **ppCommandHandler)
{
	if (!ppCommandHandler)
	{
		return E_POINTER;
	}

	*ppCommandHandler = NULL;

	HRESULT hr = S_OK;

	CCommandHandler* pCommandHandler = new CCommandHandler();

	if (pCommandHandler != NULL)
	{
		*ppCommandHandler = static_cast<IUICommandHandler *>(pCommandHandler);
	}
	else
	{
		hr = E_OUTOFMEMORY;
	}

	return hr;
}


STDMETHODIMP_(ULONG) CCommandHandler::AddRef()
{
	return InterlockedIncrement(&m_cRef);
}

STDMETHODIMP_(ULONG) CCommandHandler::Release()
{
	LONG cRef = InterlockedDecrement(&m_cRef);
	if (cRef == 0)
	{
		delete this;
	}

	return cRef;
}

STDMETHODIMP CCommandHandler::QueryInterface(REFIID iid, void** ppv)
{
	if (iid == __uuidof(IUnknown))
	{
		*ppv = static_cast<IUnknown*>(this);
	}
	else if (iid == __uuidof(IUICommandHandler))
	{
		*ppv = static_cast<IUICommandHandler*>(this);
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	AddRef();
	return S_OK;
}

//
//  FUNCTION: UpdateProperty()
//
//  PURPOSE: Вызывается при обновлении.
// 
//
STDMETHODIMP CCommandHandler::UpdateProperty(
	UINT nCmdID,
	__in REFPROPERTYKEY key,
	__in_opt const PROPVARIANT* ppropvarCurrentValue,
	__out PROPVARIANT* ppropvarNewValue)
{
	UNREFERENCED_PARAMETER(nCmdID);
	UNREFERENCED_PARAMETER(key);
	UNREFERENCED_PARAMETER(ppropvarCurrentValue);
	UNREFERENCED_PARAMETER(ppropvarNewValue);

	return E_NOTIMPL;
}

//
//  FUNCTION: Execute()
//
//  PURPOSE: Обработчик комманд Ribbon
//
STDMETHODIMP CCommandHandler::Execute(
	UINT nCmdID,
	UI_EXECUTIONVERB verb,
	__in_opt const PROPERTYKEY* key,
	__in_opt const PROPVARIANT* ppropvarValue,
	__in_opt IUISimplePropertySet* pCommandExecutionProperties)
{
	UNREFERENCED_PARAMETER(pCommandExecutionProperties);
	UNREFERENCED_PARAMETER(ppropvarValue);
	UNREFERENCED_PARAMETER(key);
	UNREFERENCED_PARAMETER(verb);
	UNREFERENCED_PARAMETER(nCmdID);

	HWND hwnd = GetForegroundWindow();

	switch (nCmdID)
	{
	case ID_CMD_FRAC:
		::SendMessage(g_pHwnd, WM_COMMAND, WPARAM(ID_ADD_FRAC), LPARAM(0));
		break;

	case ID_CMD_DEGR:
		::SendMessage(g_pHwnd, WM_COMMAND, WPARAM(ID_ADD_DEGR), LPARAM(0));
		break;

	case ID_CMD_SUBSCRIPT:
		::SendMessage(g_pHwnd, WM_COMMAND, WPARAM(ID_ADD_SUBSCRIPT), LPARAM(0));
		break;

	case ID_CMD_RADICAL:
		::SendMessage(g_pHwnd, WM_COMMAND, WPARAM(ID_ADD_RADICAL), LPARAM(0));
		break;

	case ID_CMD_Bracket:
		::SendMessage(g_pHwnd, WM_COMMAND, WPARAM(ID_ADD_PARANTHESIS), LPARAM(0));
		break;

	/* case ID_CMD_SUM:
		::SendMessage( g_pHwnd, WM_COMMAND, WPARAM( ID_ADD_SUM ), LPARAM( 0 ) );
		break;

	case ID_CMD_Product:
		::SendMessage( g_pHwnd, WM_COMMAND, WPARAM( ID_ADD_PRODUCT ), LPARAM( 0 ) );
		break;*/

	case ID_CMD_EXIT:
		PostMessage(hwnd, WM_CLOSE, NULL, NULL);
		break;
	}

	return S_OK;
}
