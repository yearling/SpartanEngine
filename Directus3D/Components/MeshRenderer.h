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

#pragma once

//= INCLUDES =================
#include "IComponent.h"
#include "../Math/Matrix.h"
#include "../Core/Material.h"
//============================

class Light;

class __declspec(dllexport) MeshRenderer : public IComponent
{
public:
	MeshRenderer();
	~MeshRenderer();

	/*------------------------------------------------------------------------------
									[INTERFACE]
	------------------------------------------------------------------------------*/
	virtual void Initialize();
	virtual void Update();
	virtual void Save();
	virtual void Load();

	/*------------------------------------------------------------------------------
										[MISC]
	------------------------------------------------------------------------------*/
	void Render(unsigned int indexCount, Directus::Math::Matrix viewMatrix, Directus::Math::Matrix projectionMatrix, Light* directionalLight);

	/*------------------------------------------------------------------------------
										[PROPERTIES]
	------------------------------------------------------------------------------*/
	void SetCastShadows(bool castShadows);
	bool GetCastShadows();
	void SetReceiveShadows(bool receiveShadows);
	bool GetReceiveShadows();

	/*------------------------------------------------------------------------------
									[MATERIAL]
	------------------------------------------------------------------------------*/
	std::shared_ptr<Material> GetMaterial();
	void SetMaterial(std::shared_ptr<Material> material);
	void SetMaterialStandardDefault();
	void SetMaterialStandardSkybox();
	bool HasMaterial();

private:
	std::shared_ptr<Material> m_material;
	bool m_castShadows;
	bool m_receiveShadows;
};