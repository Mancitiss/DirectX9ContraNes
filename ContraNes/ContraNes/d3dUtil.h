#pragma once
#include <d3d9.h>
#include <string>
#include <fstream>
#include "GameSprite.h"

namespace d3dColors
{
	const D3DCOLOR Red = D3DCOLOR_ARGB(255, 255, 0, 0);
	const D3DCOLOR Green = D3DCOLOR_ARGB(255, 0, 255, 0);
	const D3DCOLOR Blue = D3DCOLOR_ARGB(255, 0, 0, 255);
	const D3DCOLOR CornflowerBlue = D3DCOLOR_ARGB(255, 100, 149, 237);
	const D3DCOLOR White = D3DCOLOR_ARGB(255, 255, 255, 255);
	const D3DCOLOR Black = D3DCOLOR_ARGB(255, 0, 0, 0);
	const D3DCOLOR Yellow = D3DCOLOR_ARGB(255, 255, 255, 0);
	const D3DCOLOR Orange = D3DCOLOR_ARGB(255, 255, 165, 0);
	const D3DCOLOR Grey = D3DCOLOR_ARGB(255, 128, 128, 128);
	const D3DCOLOR LightGrey = D3DCOLOR_ARGB(255, 192, 192, 192);
	const D3DCOLOR DarkGrey = D3DCOLOR_ARGB(255, 64, 64, 64);
	const D3DCOLOR Purple = D3DCOLOR_ARGB(255, 128, 0, 128);
	const D3DCOLOR Pink = D3DCOLOR_ARGB(255, 255, 192, 203);
	const D3DCOLOR Brown = D3DCOLOR_ARGB(255, 139, 69, 19);
	const D3DCOLOR Gold = D3DCOLOR_ARGB(255, 255, 215, 0);
	const D3DCOLOR Silver = D3DCOLOR_ARGB(255, 192, 192, 192);
	
	const D3DCOLOR Transparent = D3DCOLOR_ARGB(0, 0, 0, 0);
	
}

template<typename T>
void DeleteCircularList(T*& head) {
	if (!head) return;

	T* current = head;
	T* next = nullptr;

	// Handle 1 node case
	if (head->pNext == head) {
		delete head;
		return;
	}

	T* pLast = head;
	current = head->pNext;
	pLast->pNext = nullptr;
	/*while (pLast->pNext != head) {
		pLast = pLast->pNext;
	}
	pLast->pNext = nullptr;*/

	// Delete each node in the list
	do {
		next = current->pNext;
		delete current;
		current = next;
	} while (current != head);

	// Set head to null
	head = nullptr;
}

LPCWSTR ConvertToLPCWSTR(const std::string& str);

void GetPngImageDimensions(std::string& file_path, unsigned int& width, unsigned int& height);

bool CreateSprites(LPDIRECT3DDEVICE9 device, int N, const std::string& prefix, const std::string& ext, GameSprite*& first, GameSprite*& pDefault);

bool CheckIntersection(RECT* const& rectA, RECT* const& rectB);