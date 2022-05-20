#pragma once
#include "../../VEWindow.h"
#include <windows.h>

class Window : public VEWindow
{
public:
	Window();
	~Window();

	void Create() override;
	void Destroy() override;

	void Show() override;
	void Hide() override;

	void* Handle() const override { return handle; }

	size_t Width() const override { return width; }
	size_t Height() const override { return height; }
	float AspectRatio() const override;

protected:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	HWND handle;

	size_t width;
	size_t height;
};