#pragma once

//
//  CLASS: CApplication : IUIApplication
//
//  PURPOSE: реализует интерфейс, который позволяет управлять событиями Framework
//
//  COMMENTS:
//
//    Реализует интерфейс необходимый для взаимодействия
//    Содержит создание, удаление
//
class CApplication
	: public IUIApplication // базовый IUIApplication.
{
public:

	// Статический метод создания экземпляра объекта.
	static HRESULT CreateInstance(__deref_out IUIApplication **ppApplication);

	STDMETHOD_(ULONG, AddRef());
	STDMETHOD_(ULONG, Release());
	STDMETHOD(QueryInterface(REFIID iid, void** ppv));

	STDMETHOD(OnCreateUICommand)(UINT nCmdID,
		__in UI_COMMANDTYPE typeID,
		__deref_out IUICommandHandler** ppCommandHandler);

	STDMETHOD(OnViewChanged)(UINT viewId,
		__in UI_VIEWTYPE typeId,
		__in IUnknown* pView,
		UI_VIEWVERB verb,
		INT uReasonCode);

	STDMETHOD(OnDestroyUICommand)(UINT32 commandId,
		__in UI_COMMANDTYPE typeID,
		__in_opt IUICommandHandler* commandHandler);

private:
	CApplication()
		: m_cRef(1)
		, m_pCommandHandler(NULL)
	{
	}

	~CApplication()
	{
		if (m_pCommandHandler)
		{
			m_pCommandHandler->Release();
			m_pCommandHandler = NULL;
		}
	}

	LONG m_cRef;                            // Счетчик ссылок.
	IUICommandHandler * m_pCommandHandler;  // Ссылка на обработчик команд
};
