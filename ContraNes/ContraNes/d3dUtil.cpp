#include "d3dUtil.h"
#include "GameSprite.h"

LPCWSTR ConvertToLPCWSTR(const std::string& str)
{
	LPCWSTR result;
	size_t size = strlen(str.c_str()) + 1;
	size_t outSize;
	mbstowcs_s(&outSize, NULL, 0, str.c_str(), size);
	wchar_t* buf = new wchar_t[outSize];
	mbstowcs_s(&outSize, buf, outSize, str.c_str(), size);
	result = buf;
	return result;
}

void GetPngImageDimensions(std::string& file_path, unsigned int& width, unsigned int& height)
{
	unsigned char buf[8];

	std::ifstream in(file_path, std::ios::binary);
	in.seekg(16);
	in.read(reinterpret_cast<char*>(&buf), 8);

	width = (buf[0] << 24) + (buf[1] << 16) + (buf[2] << 8) + (buf[3] << 0);
	height = (buf[4] << 24) + (buf[5] << 16) + (buf[6] << 8) + (buf[7] << 0);
}

bool CreateSprites(LPDIRECT3DDEVICE9 device, int N, const std::string& prefix, const std::string& ext, GameSprite*& first, GameSprite*& pDefault)
{
    std::string filename;
    GameSprite* current = nullptr;

    // Loop through each sprite index and create a new GameSprite
    for (int i = 1; i <= N; i++)
    {
        // Construct the filename for the current sprite
        filename = prefix + "-" + std::to_string(i) + ext;

        // Get the dimensions of the image file
        unsigned int width, height;
        GetPngImageDimensions(filename, width, height);

        // Create a new GameSprite with the image dimensions
        GameSprite* sprite = new GameSprite();
        if (!sprite->Init(device, ConvertToLPCWSTR(filename), width, height, 0))
        {
            // Cleanup and return false if sprite creation fails
            delete sprite;
            delete current;
            return false;
        }

        // Set the default sprite for the current sprite
        sprite->pDefault = pDefault;

        // Set the next sprite for the current sprite
        if (current == nullptr)
        {
            first = sprite;
        }
        else
        {
            current->pNext = sprite;
        }

        // Update the current sprite
        current = sprite;
    }

    // Set the next sprite for the last sprite to complete the loop
    current->pNext = first;

    return true;
}

bool CheckIntersection(RECT* const& rectA, RECT* const& rectB)
{
    return max(rectA->left, rectB->left) < min(rectA->right, rectB->right)
        && max(rectA->top, rectB->top) < min(rectA->bottom, rectB->bottom);
}