#pragma once

namespace Animator::Utils
{
	static std::string RetrieveFilenameFromFilepath(const std::string& filepath)
	{
		const size_t start = filepath.find_last_of("/\\") + 1;
		size_t end = filepath.rfind('.');

		if (end == std::string::npos || end < start)
		{
			end = filepath.length();
		}

		return filepath.substr(start, end - start);
	}

	static std::string RetrieveFilenameFromFilepathWithExtension(const std::string& filepath)
	{
		const size_t pos = filepath.find_last_of("/\\");

		if (pos != std::string::npos)
			return filepath.substr(0, pos);

		return filepath;
	}
}