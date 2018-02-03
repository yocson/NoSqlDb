#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "../XmlDocument/XmlDocument/XmlDocument.h"

class FileInfo
{
public:
	using FilePath = std::string;
	using Category = std::vector<std::string>;
	using Sptr = std::shared_ptr<XmlProcessing::AbstractXmlElement>;

	FileInfo(){}
	FileInfo(std::shared_ptr<XmlProcessing::AbstractXmlElement> payloadTag);
	FileInfo(std::string);
	std::string toString() const;
	Sptr toXML();
	operator std::string();

	friend std::ostream& operator<<(std::ostream& out, const FileInfo& f);

private:
	FilePath filePath_;
	Category category_;
};