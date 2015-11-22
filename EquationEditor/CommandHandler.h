#pragma once

//
//  CLASS: CCommandHandler : IUICommandHandler
//
//  PURPOSE: ��������� ����������� ������� Ribbon
//
//  COMMENTS:
//
//    ���������� ���������� ����� ��������� �������
//
class CCommandHandler
	: public IUICommandHandler // ������� IUICommandHandler.
{
public:

	// ����������� ����� �������� ���������� �������.
	__checkReturn static HRESULT CreateInstance(__deref_out IUICommandHandler **ppCommandHandler);

	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();
	STDMETHODIMP QueryInterface(REFIID iid, void** ppv);

	STDMETHOD(UpdateProperty)(UINT nCmdID,
		__in REFPROPERTYKEY key,
		__in_opt const PROPVARIANT* ppropvarCurrentValue,
		__out PROPVARIANT* ppropvarNewValue);

	STDMETHOD(Execute)(UINT nCmdID,
		UI_EXECUTIONVERB verb,
		__in_opt const PROPERTYKEY* key,
		__in_opt const PROPVARIANT* ppropvarValue,
		__in_opt IUISimplePropertySet* pCommandExecutionProperties);

private:
	CCommandHandler()
		: m_cRef(1)
	{
	}

	LONG m_cRef;                        // ������� ������.
};
