/*
Copyright(c) 2016 Panos Karabelas

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is furnished
to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

//= INCLUDES ======================
#include "Texture.h"
#include "../Core/GUIDGenerator.h"
#include "../Core/Globals.h"
#include "../IO/Serializer.h"
#include "../IO/Log.h"
#include "../Loading/ImageLoader.h"
#include "../IO/FileHelper.h"
//=================================

//= NAMESPACES =====
using namespace std;
//==================

Texture::Texture()
{
	m_ID = GENERATE_GUID;
	m_width = 0;
	m_height = 0;
	m_shaderResourceView = nullptr;
	m_type = Albedo;
	m_grayscale = false;
	m_transparency = false;
	m_alphaIsTransparency = false;
}

Texture::~Texture()
{
	DirectusSafeRelease(m_shaderResourceView);
}

void Texture::Serialize() const
{
	Serializer::SaveSTR(m_ID);
	Serializer::SaveSTR(m_name);
	Serializer::SaveSTR(m_path);
	Serializer::SaveInt(m_width);
	Serializer::SaveInt(m_height);
	Serializer::SaveInt(int(m_type));
	Serializer::SaveBool(m_grayscale);
	Serializer::SaveBool(m_transparency);
}

void Texture::Deserialize()
{
	m_ID = Serializer::LoadSTR();
	m_name = Serializer::LoadSTR();
	m_path = Serializer::LoadSTR();
	m_width = Serializer::LoadInt();
	m_height = Serializer::LoadInt();
	m_type = TextureType(Serializer::LoadInt());
	m_grayscale = Serializer::LoadBool();
	m_transparency = Serializer::LoadBool();

	LoadFromFile(m_path, m_type);
}

ID3D11ShaderResourceView* Texture::GetID3D11ShaderResourceView() const
{
	return m_shaderResourceView;
}

void Texture::SetID3D11ShaderResourceView(ID3D11ShaderResourceView* srv)
{
	m_shaderResourceView = srv;
}

bool Texture::LoadFromFile(string path, TextureType type)
{
	// load it
	bool result = ImageLoader::GetInstance().Load(path);

	if (!result)
	{
		LOG("Failed to load texture \"" + path + "\".", Log::Error);
		ImageLoader::GetInstance().Clear();
		return false;
	}

	// Fill the texture with data
	SetPath(ImageLoader::GetInstance().GetPath());
	SetName(FileHelper::GetFileNameFromPath(GetPath()));
	SetWidth(ImageLoader::GetInstance().GetWidth());
	SetHeight(ImageLoader::GetInstance().GetHeight());
	SetGrayscale(ImageLoader::GetInstance().IsGrayscale());
	SetTransparency(ImageLoader::GetInstance().IsTransparent());
	m_shaderResourceView = ImageLoader::GetInstance().GetAsD3D11ShaderResourceView();

	// Determine texture type
	// FIX: some models pass a normal map as a height map
	// and others pass a height map as a normal map...
	SetType(type);
	if (GetType() == Height && !GetGrayscale()) SetType(Normal);
	if (GetType() == Normal && GetGrayscale()) SetType(Height);

	// Clear any memory allocated by the image loader
	ImageLoader::GetInstance().Clear();

	return true;
}

string Texture::GetID() const
{
	return m_ID;
}

void Texture::SetName(string name)
{
	m_name = name;
}

string Texture::GetName()
{
	return m_name;
}

void Texture::SetWidth(int width)
{
	m_width = width;
}

int Texture::GetWidth() const
{
	return m_width;
}

void Texture::SetHeight(int height)
{
	m_height = height;
}

int Texture::GetHeight() const
{
	return m_height;
}

string Texture::GetPath() const
{
	return m_path;
}

void Texture::SetPath(string path)
{
	m_path = path;
}

TextureType Texture::GetType() const
{
	return m_type;
}

void Texture::SetType(TextureType type)
{
	m_type = type;
}

void Texture::SetGrayscale(bool isGrayscale)
{
	m_grayscale = isGrayscale;
}

bool Texture::GetGrayscale() const
{
	return m_grayscale;
}

void Texture::SetTransparency(bool transparency)
{
	m_transparency = transparency;
}

bool Texture::GetTransparency()
{
	return m_transparency;
}