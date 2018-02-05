#include "FileInfo.h"
#include <regex>

FileInfo::FileInfo(std::shared_ptr<XmlProcessing::AbstractXmlElement> payloadTag)
{
	fromXML(payloadTag);
}

FileInfo::FileInfo(std::string str)
{
	XmlProcessing::XmlParser parser(str, XmlProcessing::XmlParser::sourceType::str);
	XmlProcessing::XmlDocument* pDoc = parser.buildDocument();
	Sptr pFileInfo = pDoc->element("fileInfo").select()[0];
	fromXML(pFileInfo);
}

FileInfo::operator std::string() {
	std::string str;
	std::string inde = "          ";
	str = "<fileInfo>\n";
	str += inde + "  <filePath>" + filePath_ + "</filePath>\n";
	str += inde + "    <categories>\n";
	for (auto cate : category_) {
		str += inde + "      <category>" + cate + "</category>\n";
	}
	str += inde + "    </categories>\n";
	str += inde + "</fileInfo>";
	return str;
}

std::string FileInfo::toString() const
{
	std::string str;
	str += "filePath: " + filePath_ + " ";
	str += "Categories: ";
	for (std::string cate : category_) {
		str += cate + ", ";
	}
	return str;
}

void FileInfo::fromXML(Sptr payloadTag)
{
	for (auto payloadInfo : payloadTag->children()) {
		if (payloadInfo->tag() == "filePath") 
			filePath_ = payloadInfo->children()[0]->value();
		if (payloadInfo->tag() == "categories") {
			for (auto category : payloadInfo->children())
				category_.insert(category->children()[0]->value());
		}
	}
}

FileInfo::Sptr FileInfo::toXML()
{
	Sptr pFileInfo = XmlProcessing::makeTaggedElement("fileInfo");
	Sptr pFilePath = XmlProcessing::makeTaggedElement("filePath", filePath_);
	pFileInfo->addChild(pFilePath);
	Sptr pCategories = XmlProcessing::makeTaggedElement("categories");
	for (auto cate : category_) {
		Sptr pCategory = XmlProcessing::makeTaggedElement("category", cate);
		pCategories->addChild(pCategory);
	}
	pFileInfo->addChild(pCategories);
	return pFileInfo;
}

std::ostream& operator<<(std::ostream& out, const FileInfo& f) {
	out << f.toString();
	return out;
}

bool FileInfo::compare(const PayLoad & p)
{
	const FileInfo *f = dynamic_cast<const FileInfo *>(&p);
	return matchFilePath(f->filePath()) && matchCategory(f->category());
}

bool FileInfo::matchFilePath(const FilePath &fp) {
	if (fp.length() > 0) {
		std::regex e(fp);
		if (std::regex_match(filePath_, e)) return true;
		else return false;
	}
	return true;
}
bool FileInfo::matchCategory(const Category &cate) {
	if (cate.size() > 0) {
		std::string cat = *(cate.begin());
		std::regex e(cat);
		for (auto cat_ : category_) {
			if (std::regex_match(cat_, e)) return true;
		}
		return false;
	}
	return true;
}