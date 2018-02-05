#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <unordered_set>
#include "../XmlDocument/XmlDocument/XmlDocument.h"
#include "../XmlDocument/XmlParser/XmlParser.h"

namespace PAYLOAD {
	class PayLoad
	{
	public:
		using Sptr = std::shared_ptr<XmlProcessing::AbstractXmlElement>;
		virtual Sptr toXML() = 0;
		virtual bool compare(const PayLoad & p) = 0;
		virtual std::string toString() const = 0;
	};


	class FileInfo : public PayLoad
	{
	public:
		using FilePath = std::string;
		using Category = std::unordered_set<std::string>;
		using Sptr = std::shared_ptr<XmlProcessing::AbstractXmlElement>;

		FileInfo() {}
		FileInfo(std::shared_ptr<XmlProcessing::AbstractXmlElement> payloadTag);
		FileInfo(std::string str);
		std::string toString() const;
		void fromXML(Sptr payloadTag);
		Sptr toXML();
		operator std::string();

		friend std::ostream& operator<<(std::ostream& out, const FileInfo& f);

		bool compare(const PayLoad & p);

		FilePath& filePath() { return filePath_; }
		FilePath filePath() const { return filePath_; }
		void filePath(const FilePath& fp) { filePath_ = fp; }

		Category& category() { return category_; }
		Category category() const { return category_; }
		void category(const Category& cate) { category_ = cate; }

	private:
		FilePath filePath_;
		Category category_;
		bool matchFilePath(const FilePath &fp);
		bool matchCategory(const Category &cate);
	};
}
