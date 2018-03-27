#include "serialisation.h"
#include "file_log.h"
#include <sstream>

template <typename T>
void SerialiseText(TextArchive *arch, T value, const var_info& info, std::string& line)
{
	// data stored in text file in the format e.g. <.x>val
	std::string begin_str = "<" + info.name + "> ";
	std::string end_str = "";

	if (arch->mode == FileMode::FILE_OUT)
	{
		arch->file << begin_str << value << end_str << "\n";
	}
	else
	{
		// note : this method requires a space between the descriptor and the value
		getline(arch->file, line);
		std::stringstream ss(line);
		std::string descriptor;
		ss >> descriptor;
		ss >> line;
		if (ss.fail())
		{
			g_filelog->WriteLog("Critical error! could not read data from level file.");
			exit(EXIT_FAILURE);
		}
	}
}

void TextArchive::Serialise(char& value, const var_info& info)
{
	std::string line;
	SerialiseText<int32_t>(this, value, info, line);
	if (!line.empty())
		value = line[0];
}

void TextArchive::Serialise(float& value, const var_info& info)
{
	std::string line;
	SerialiseText<float>(this, value, info, line);
	if (!line.empty())
		value = std::stof(line);
}

void TextArchive::Serialise(int32_t& value, const var_info& info)
{
	std::string line;
	SerialiseText<int32_t>(this, value, info, line);
	if (!line.empty())
		value = std::stoi(line);
}

void TextArchive::Serialise(uint32_t& value, const var_info& info)
{
	std::string line;
	SerialiseText<uint32_t>(this, value, info, line);
	if (!line.empty())
		value = std::stoi(line);
}

void TextArchive::Serialise(long& value, const var_info& info)
{
	std::string line;
	SerialiseText<long>(this, value, info, line);
	if (!line.empty())
		value = std::stol(line);
}

void TextArchive::Serialise(bool& value, const var_info& info)
{
	std::string line;
	SerialiseText<bool>(this, value, info, line);
	if (!line.empty())
		value = (std::stoi(line) == 0) ? false : true;
}

// global serialisation functions
void Serialise(Archive *arch, char& value, const var_info& info)
{
	arch->Serialise(value, info);
}

void Serialise(Archive *arch, float& value, const var_info& info)
{
	arch->Serialise(value, info);
}

void Serialise(Archive *arch, int32_t& value, const var_info& info)
{
	arch->Serialise(value, info);
}

void Serialise(Archive *arch, uint32_t& value, const var_info& info)
{
	arch->Serialise(value, info);
}

void Serialise(Archive *arch, long& value, const var_info& info)
{
	arch->Serialise(value, info);
}

void Serialise(Archive *arch, bool& value, const var_info& info)
{
	arch->Serialise(value, info);
}

