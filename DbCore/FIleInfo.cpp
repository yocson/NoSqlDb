#include "FileInfo.h"

FileInfo::FileInfo(std::shared_ptr<XmlProcessing::AbstractXmlElement> payloadTag)
{
	for (auto payloadInfo: payloadTag->children()) {
		if (payloadInfo->tag() == "filepath") {
			filePath_ = payloadInfo->children()[0]->value();
		}
		if (payloadInfo->tag() == "categories") {
			for (auto category : payloadInfo->children()) {
				category_.push_back(category->children()[0]->value());
			}
		}
	}
}

FileInfo::FileInfo(std::string)
{
}

std::string FileInfo::toString()
{
	return std::string();
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
