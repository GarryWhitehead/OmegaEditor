#ifndef _SERIALISATION_H
#define _SERIALISATION_H
#include <fstream>
#include <memory>
#include <string>
#include <memory>
#include <regex>
#include <unordered_map>
#include <chrono>

enum class FileMode
{
	FILE_IN,
	FILE_OUT
};

enum class SaveMode
{
	SAVE_TEXT,
	SAVE_BINARY
};

struct var_info
{
	var_info(const std::string& n) : name(n) {}

	std::string name;
};

struct Archive
{
	Archive(FileMode m) :  mode(m) {}

	// Abstract function prototypes
	virtual void Serialise(char& value, const var_info& info) = 0;
	virtual void Serialise(float& value, const var_info& info) = 0;
	//virtual void Serialise(uint16_t& value, const var_info& info) = 0;
	virtual void Serialise(uint32_t& value, const var_info& info) = 0;
	virtual void Serialise(int32_t& value, const var_info& info) = 0;
	//virtual void Serialise(uint8_t& value, const var_info& info) = 0;
	//virtual void Serialise(double& value, const var_info& info) = 0;
	virtual void Serialise(long& value, const var_info& info) = 0;
	virtual void Serialise(bool& value, const var_info& info) = 0;

	// file mode
	FileMode mode;
};

// global function prototypes
void Serialise(Archive *arch, char& value, const var_info& info);
void Serialise(Archive *arch, float& value, const var_info& info);
//void Serialise(Archive *arch, uint16_t& value, const var_info& info);
void Serialise(Archive *arch, uint32_t& value, const var_info& info);
void Serialise(Archive *arch, int32_t& value, const var_info& info);
//void Serialise(Archive *arch, uint8_t& value, const var_info& info);
//void Serialise(Archive *arch, double& value, const var_info& info);
void Serialise(Archive *arch, long& value, const var_info& info);
void Serialise(Archive *arch, bool& value, const var_info& info);

struct TextArchive : public Archive
{
	
	TextArchive(std::fstream& f, FileMode mode) : Archive(mode), file(f) {}

	// function prototypes
	void Serialise(char& value, const var_info& info) override;
	void Serialise(float& value, const var_info& info) override;
	//void Serialise(uint16_t& value, const var_info& info) override;
	void Serialise(uint32_t& value, const var_info& info) override;
	void Serialise(int32_t& value, const var_info& info) override;
	//void Serialise(uint8_t& value, const var_info& info) override;
	//void Serialise(double& value, const var_info& info) override;
	void Serialise(long& value, const var_info& info) override;
	void Serialise(bool& value, const var_info& info) override;

	// filestream handle
	std::fstream& file;
};

// serialise data to/from text
template <typename T>
void SerialiseText(TextArchive *arch, T value, const var_info& info, std::string& line);


#endif // _SERIALISATION_H

