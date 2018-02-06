#include "PayLoad.h"
#include <regex>
using namespace PAYLOAD;

//----< constructor from a xmlelement pointer>-----------
FileInfo::FileInfo(std::shared_ptr<XmlProcessing::AbstractXmlElement> payloadTag)
{
	fromXML(payloadTag);
}

//----< constructor from a string>-----------
FileInfo::FileInfo(std::string str)
{
	XmlProcessing::XmlParser parser(str, XmlProcessing::XmlParser::sourceType::str);
	XmlProcessing::XmlDocument* pDoc = parser.buildDocument();
	Sptr pFileInfo = pDoc->element("fileInfo").select()[0];
	fromXML(pFileInfo);
}

//----< cast to string >-----------
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

//----< transfer to string >-----------
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

//----< initialize the data from XML >-----------
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

//----< translate to XML >-----------
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

//----< overload cout >-----------
std::ostream& PAYLOAD::operator<<(std::ostream& out, const FileInfo& f) {
	out << f.toString();
	return out;
}

//----< match the fields, call two private match functions respectively >-----------
bool FileInfo::compare(const PayLoad & p)
{
	const FileInfo *f = dynamic_cast<const FileInfo *>(&p);
	return matchFilePath(f->filePath()) && matchCategory(f->category());
}

void FileInfo::addCate(const std::string & cat)
{
	category_.insert(cat);
}

//----< match the filepath using regular expression >-----------
bool FileInfo::matchFilePath(const FilePath &fp) {
	if (fp.length() > 0) {
		std::regex e(fp);
		if (std::regex_match(filePath_, e)) return true;
		else return false;
	}
	return true;
}

//----< match the category using regular expression, assuming only one cate passing in >-----------
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

#ifdef TEST_PAYLOAD
using namespace PAYLOAD;

int main() {
	FileInfo f1;
	f1.filePath() = "testPath";
	f1.addCate("cat1");
	f1.addCate("cat2");
	std::cout << "\n  Show f1:" << std::endl;
	std::cout << f1 << std::endl;
	FileInfo f2;
	f2.filePath() = "testPath";
	f1.addCate("cat2");
	std::cout << "\n  match f1 with f2:" << std::endl;
	bool match = f1.compare(f2);

	if (match) {
		std::cout << "match!" << std::endl;
	}
	else {
		std::cout << "not match!" << std::endl;
	}
	FileInfo::Sptr sptr = f1.toXML();
	std::cout << "\n transfer f1 to xml: " << std::endl;
	std::cout << sptr->toString() << std::endl;
	f2.fromXML(sptr);
	std::cout << "\n restore f2 from xml f1 generated: " << std::endl;
	std::cout << f2 << std::endl;
	getchar();
	getchar();
	return 0;
}
#endif // TEST_PAYLOAD
