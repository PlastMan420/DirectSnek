#pragma once
#include "Helpers/ErrorLogger.h"

class WindowContainer;

class RenderWindow {
public:
	bool Initialize(
		WindowContainer *pWindowContainer,
		HINSTANCE hInstance,
		std::string window_title,
		std::string window_class,
		int width,
		int height
	);

	bool ProcessMessages();
	HWND GetHWND() const;

	~RenderWindow();

private:
	void RegisterWindowClass();
	HWND handle = NULL;
	HINSTANCE hInstance = NULL;
	std::string window_title = "";
	std::wstring window_wtitle = L"";
	std::string window_class = "";
	std::wstring window_wclass = L"";
	int width = 0;
	int height = 0;
};