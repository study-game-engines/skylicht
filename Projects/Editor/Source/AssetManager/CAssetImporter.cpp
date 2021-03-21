/*
!@
MIT License

CopyRight (c) 2021 Skylicht Technology CO., LTD

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files
(the "Software"), to deal in the Software without restriction, including without limitation the Rights to use, copy, modify,
merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRight HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

This file is part of the "Skylicht Engine".
https://github.com/skylicht-lab/skylicht-engine
!#
*/

#include "pch.h"
#include "CAssetImporter.h"

#include <filesystem>
#include <chrono>
#include <sstream>
#include <sys/stat.h>

#include "Utils/CPath.h"
#include "Utils/CStringImp.h"

#if defined(__APPLE_CC__)
namespace fs = std::__fs::filesystem;
#else
namespace fs = std::filesystem;
#endif

namespace Skylicht
{
	namespace Editor
	{
		CAssetImporter::CAssetImporter() :
			m_fileID(0)
		{
			m_assetManager = CAssetManager::getInstance();
			m_fileIterator = m_assetManager->m_files.begin();
			m_fileID = 0;
		}

		CAssetImporter::~CAssetImporter()
		{

		}

		bool CAssetImporter::loadGUID(int count)
		{
			if (m_fileIterator == m_assetManager->m_files.end())
				return true;

			std::time_t now = std::time(0);

			for (int j = 0; j < count; j++)
			{
				SFileNode* node = (*m_fileIterator);

				std::string path = node->FullPath;
				std::string meta = path + ".meta";

				bool regenerate = true;

				if (fs::exists(meta))
				{
					// load meta
					readGUID(meta.c_str(), node);

					// remove meta
					m_assetManager->m_meta.remove(meta);

					// check collision
					if (node->Guid.empty()
						|| node->Guid.size() != 64
						|| m_assetManager->m_guidToFile.find(node->Guid) != m_assetManager->m_guidToFile.end())
					{
						regenerate = true;

						char log[1024];
						sprintf(log, "[CAssetImporter::loadGUID] GUID Collision: %s\n", node->Path.c_str());
						os::Printer::log(log);
					}
					else
					{
						regenerate = false;

						// map guid
						m_assetManager->m_guidToFile[node->Guid] = node;
					}
				}
				else
				{
					regenerate = true;
				}

				if (regenerate)
				{
					// generate guid
					node->Guid = m_assetManager->generateHash(node->Bundle.c_str(), node->Path.c_str(), node->CreateTime, now);

					// save meta
					saveGUID(meta.c_str(), node);

					// map guid
					m_assetManager->m_guidToFile[node->Guid] = node;
				}

				m_lastGUIDFile = node->Path;

				++m_fileIterator;
				++m_fileID;

				if (m_fileIterator == m_assetManager->m_files.end())
				{
					removeUnusedMeta();
					return true;
				}
			}

			return false;
		}

		void CAssetImporter::readGUID(const char* path, SFileNode* node)
		{
			io::IFileSystem* filesystem = getIrrlichtDevice()->getFileSystem();
			io::IXMLReader* xmlRead = filesystem->createXMLReader(path);
			if (xmlRead == NULL)
				return;

			while (xmlRead->read())
			{
				switch (xmlRead->getNodeType())
				{
				case io::EXN_ELEMENT:
				{
					std::wstring nodeName = xmlRead->getNodeName();
					if (nodeName == L"guid")
					{
						const wchar_t* value = xmlRead->getAttributeValue(L"id");
						if (value != NULL)
						{
							char text[70];
							CStringImp::convertUnicodeToUTF8(value, text);
							node->Guid = text;

							xmlRead->drop();
							return;
						}
					}
					break;
				}
				default:
					break;
				}
			}

			xmlRead->drop();
		}

		void CAssetImporter::saveGUID(const char* path, SFileNode* node)
		{
			io::IFileSystem* filesystem = getIrrlichtDevice()->getFileSystem();
			io::IWriteFile* file = filesystem->createAndWriteFile(path);
			if (file == NULL)
				return;

			std::string data;
			data += "<meta>\n";
			data += "\t<guid id=\"" + node->Guid += "\"/>\n";
			data += "</meta>";

			file->write(data.c_str(), (u32)data.size());
			file->drop();
		}

		void CAssetImporter::getStatus(float& percent, std::string& last)
		{
			percent = m_fileID / (float)(m_assetManager->m_files.size());
			last = m_lastGUIDFile;
		}

		bool CAssetImporter::isFinish()
		{
			if (m_fileIterator == m_assetManager->m_files.end())
				return true;

			return false;
		}

		void CAssetImporter::removeUnusedMeta()
		{
			for (const std::string& path : m_assetManager->m_meta)
			{
				fs::remove(path);
			}

			m_assetManager->m_meta.clear();
		}
	}
}