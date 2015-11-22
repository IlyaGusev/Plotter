#include <UIRibbon.h>
#include <UIRibbonPropertyHelpers.h>

#include "Application.h"
#include "CommandHandler.h"

__checkReturn HRESULT CApplication::CreateInstance(__deref_out IUIApplication **ppApplication)
{
	if (!ppApplication)
	{
		return E_POINTER;
	}

	*ppApplication = NULL;

	HRESULT hr = S_OK;

	CApplication* pApplication = new CApplication();

	if (pApplication != NULL)
	{
		*ppApplication = static_cast<IUIApplication *>(pApplication);
	}
	else
	{
		hr = E_OUTOFMEMORY;
	}

	return hr;
}

STDMETHODIMP_(ULONG) CApplication::AddRef()
{
	return InterlockedIncrement(&m_cRef);
}

STDMETHODIMP_(ULONG) CApplication::Release()
{
	LONG cRef = InterlockedDecrement(&m_cRef);
	if (cRef == 0)
	{
		delete this;
	}

	return cRef;
}

STDMETHODIMP CApplication::QueryInterface(REFIID iid, void** ppv)
{
	if (iid == __uuidof(IUnknown))
	{
		*ppv = static_cast<IUnknown*>(this);
	}
	else if (iid == __uuidof(IUIApplication))
	{
		*ppv = static_cast<IUIApplication*>(this);
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
//  FUNCTION: OnCreateUICommand(UINT, UI_COMMANDTYPE, IUICommandHandler)
//
//  PURPOSE: вызывается для каждой команды указанной в Markup
//
//
STDMETHODIMP CApplication::OnCreateUICommand(
	UINT nCmdID,
	__in UI_COMMANDTYPE typeID,
	__deref_out IUICommandHandler** ppCommandHandler)
{
	UNREFERENCED_PARAMETER(typeID);
	UNREFERENCED_PARAMETER(nCmdID);

	if (NULL == m_pCommandHandler)
	{
		HRESULT hr = CCommandHandler::CreateInstance(&m_pCommandHandler);
		if (FAILED(hr))
		{
			return hr;
		}
	}

	return m_pCommandHandler->QueryInterface(IID_PPV_ARGS(ppCommandHandler));
}

//
//  FUNCTION: OnViewChanged(UINT, UI_VIEWTYPE, IUnknown*, UI_VIEWVERB, INT)
//
//  PURPOSE: Вызывается при перерисовке
//
//
STDMETHODIMP CApplication::OnViewChanged(
	UINT viewId,
	__in UI_VIEWTYPE typeId,
	__in IUnknown* pView,
	UI_VIEWVERB verb,
	INT uReasonCode)
{
	UNREFERENCED_PARAMETER(uReasonCode);
	UNREFERENCED_PARAMETER(viewId);

	HRESULT hr = E_NOTIMPL;

	// Проверяем что его действительно видно
	if (UI_VIEWTYPE_RIBBON == typeId)
	{
		switch (verb)
		{
		case UI_VIEWVERB_CREATE:
			hr = S_OK;
			break;

		case UI_VIEWVERB_SIZE:
		{
			IUIRibbon* pRibbon = NULL;
			UINT uRibbonHeight;

			hr = pView->QueryInterface(IID_PPV_ARGS(&pRibbon));
			if (SUCCEEDED(hr))
			{
				// Спрашиваем у Ribbon o размерах
				hr = pRibbon->GetHeight(&uRibbonHeight);
				pRibbon->Release();
			}
		}
		break;
		// Удаление вида
		case UI_VIEWVERB_DESTROY:
			hr = S_OK;
			break;
		}
	}

	return hr;
}


//
//  FUNCTION: OnDestroyUICommand(UINT, UI_COMMANDTYPE, IUICommandHandler*)
//
//
STDMETHODIMP CApplication::OnDestroyUICommand(
	UINT32 nCmdID,
	__in UI_COMMANDTYPE typeID,
	__in_opt IUICommandHandler* commandHandler)
{
	UNREFERENCED_PARAMETER(commandHandler);
	UNREFERENCED_PARAMETER(typeID);
	UNREFERENCED_PARAMETER(nCmdID);

	return E_NOTIMPL;
}
