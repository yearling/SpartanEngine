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

//= INCLUDES ===============
#include "../Math/Vector2.h"
#include "../Math/Vector3.h"
#include <vector>

//==========================

class Serializer
{
public:
	static void StartWriting(std::string path);
	static void StopWriting();
	static void StartReading(std::string path);
	static void StopReading();

	static void SaveBool(bool value);
	static void SaveSTR(std::string value);
	static void SaveInt(int value);
	static void SaveFloat(float value);
	static void SaveVectorSTR(std::vector<std::string> vector);
	static void SaveVector2(Directus::Math::Vector2 vector);
	static void SaveVector3(Directus::Math::Vector3 vector);
	static void SaveVector4(Directus::Math::Vector4 vector);
	static void SaveQuaternion(Directus::Math::Quaternion quaternion);

	static bool LoadBool();
	static std::string LoadSTR();
	static unsigned int LoadUINT();
	static int LoadInt();
	static float LoadFloat();
	static std::vector<std::string> LoadVectorSTR();
	static Directus::Math::Vector2 LoadVector2();
	static Directus::Math::Vector3 LoadVector3();
	static Directus::Math::Vector4 LoadVector4();
	static Directus::Math::Quaternion LoadQuaternion();
};