#include "pch.h"
#include "Util.h"

#include <fileapi.h>
#include <filesystem>
#include <dirent.h>

//namespace fs = std::filesystem;
//================================================================================

void Util::GetAllFiles(const std::string& pathDirectory, std::vector<std::string>& paths)
{
	char buffer[PATH_MAX + 2];
	char *p = buffer;
	char *end = &buffer[PATH_MAX];

	/* Copy directory name to buffer */
	const char *dirname = pathDirectory.c_str();
	const char *src = pathDirectory.c_str();
	while (p < end && *src != '\0') 
	{
		*p++ = *src++;
	}
	*p = '\0';

	/* Open directory stream */
	DIR *dir = opendir(dirname);
	if (!dir) 
	{
		/* Could not open directory */
		/*fprintf(stderr,
			"Cannot open %s (%s)\n", dirname, strerror(errno));*/
		return /*failure*/;
	}

	/* Print all files and directories within the directory */
	struct dirent *ent;
	while ((ent = readdir(dir)) != NULL) 
	{
		char *q = p;
		char c;

		/* Get final character of directory name */
		if (buffer < q)
			c = q[-1];
		else
			c = ':';

		/* Append directory separator if not already there */
		if (c != ':' && c != '/' && c != '\\')
			*q++ = '\\';

		/* Append file name */
		src = ent->d_name;
		while (q < end && *src != '\0') 
		{
			*q++ = *src++;
		}
		*q = '\0';

		/* Decide what to do with the directory entry */
		switch (ent->d_type) 
		{
		case DT_LNK:
		case DT_REG:
		{
					   /* Output file name with directory */
					   paths.push_back(std::string(buffer));
					   printf("%s\n", buffer);
					   break;
		}
		case DT_DIR:
		{
					   /* Scan sub-directory recursively */
					   if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0)
					   {
						   GetAllFiles(buffer, paths);
					   }
					   break;
		}

		default:
			/* Ignore device entries */
			/*NOP*/;
		}

	}

	closedir(dir);
	return /*success*/;


	/*std::string path = pathDirectory;
	for (const auto & entry : fs::directory_iterator(path))
	{
		std::string pathCheck = entry.path().string();

		if (IsValidFolder(pathCheck))
		{
			GetAllFiles(pathCheck, paths);
			continue;
		}
		paths.emplace_back(pathCheck);
	}*/

}
//================================================================================
bool Util::IsValidFolder(const std::string& path)
{
	std::wstring wsTmp(path.begin(), path.end());

	DWORD FileAttributes = GetFileAttributes(wsTmp.c_str());
	if ((FileAttributes & INVALID_FILE_ATTRIBUTES) == INVALID_FILE_ATTRIBUTES)
		return false;

	if ((FileAttributes & FILE_ATTRIBUTE_HIDDEN) == FILE_ATTRIBUTE_HIDDEN)
	{
		return false;
	}
	if ((FileAttributes & FILE_ATTRIBUTE_TEMPORARY) == FILE_ATTRIBUTE_TEMPORARY)
	{
		return false;
	}

	if ((FileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
	{
		return true;
	}
	return false;
}

//================================================================================

bool Util::IsFileWritable(const std::string&  path)
{
	std::wstring wsTmp(path.begin(), path.end());

	DWORD FileAttributes = GetFileAttributes(wsTmp.c_str());
	if ((FileAttributes & INVALID_FILE_ATTRIBUTES) == INVALID_FILE_ATTRIBUTES)
		return false;

	/*if ((FileAttributes & FILE_ATTRIBUTE_ARCHIVE) == FILE_ATTRIBUTE_ARCHIVE)
	{
		return false;
	}*/
	if ((FileAttributes & FILE_ATTRIBUTE_HIDDEN) == FILE_ATTRIBUTE_HIDDEN)
	{
		return false;
	}
	if ((FileAttributes & FILE_ATTRIBUTE_TEMPORARY) == FILE_ATTRIBUTE_TEMPORARY)
	{
		return false;
	}

	/*if ((FileAttributes & FILE_ATTRIBUTE_ENCRYPTED) == FILE_ATTRIBUTE_ENCRYPTED)
	{
		return false;
	}*/
	if ((FileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
	{
		//printf("Directory ");
		return false;
	}
	if ((FileAttributes & FILE_ATTRIBUTE_READONLY) == FILE_ATTRIBUTE_READONLY)
	{
		return false;
	}
	return true;
}
//================================================================================

void Util::GetAcceptedExtension(CString& extensions, std::vector<std::string>& splittedExtension)
{
	try
	{
		std::string allExt = CT2A(extensions);
		int breakingCount = 25;
		while (allExt.find_last_of(".") != std::string::npos)
		{
			size_t index = allExt.find(".");
			size_t index2 = allExt.find(" ");
			if (index2 == std::string::npos)
				index2 = allExt.find(",");

			bool takeFullString = false;
			if (index2 == std::string::npos)
			{
				index2 = allExt.length() - 1;
				takeFullString = true;
			}


			if (index == std::string::npos || index2 == std::string::npos)
			{
				--breakingCount;
				if (breakingCount <= 1)
					break;
			}

			std::string extension = allExt.substr(index, index2 - index);
			if (takeFullString)
				extension = allExt.substr(index);

			splittedExtension.emplace_back(extension);

			// remove extra space or comma
			allExt = allExt.substr(index2 + 1);

		}
	}
	catch (std::exception ex)
	{

	}
	return ;
}
//================================================================================

void Util::CreateFolder(const std::string&  path)
{
	std::wstring wsTmp = L"";
	Util::StringToWstring(path, wsTmp);
	_tmkdir(wsTmp.c_str());
}
//================================================================================
void Util::StringToWstring(const std::string&  path, std::wstring& ws)
{
	ws = std::wstring(path.begin(), path.end());
}
//================================================================================
