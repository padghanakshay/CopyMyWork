#pragma once
#include<vector>
#include<string>
#include "stdafx.h"

class Util
{
public:
	static void GetAllFiles(const std::string& pathDirectory, std::vector<std::string>& paths);

	static bool IsValidFolder(const std::string& path);

	static bool IsFileWritable(const std::string&  path);

	static void GetAcceptedExtension(CString& extensions, std::vector<std::string>& splittedExtension);

	static void CreateFolder(const std::string&  path);
	static void StringToWstring(const std::string&  path, std::wstring& ws);

};

