/*
!@
MIT License

Copyright (c) 2020 Skylicht Technology CO., LTD

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files
(the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify,
merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

This file is part of the "Skylicht Engine".
https://github.com/skylicht-lab/skylicht-engine
!#
*/
#pragma once

#include "CBase.h"
#include "CDialogWindow.h"
#include "CDataGridView.h"
#include "CTextBox.h"

namespace Skylicht
{
	namespace Editor
	{
		namespace GUI
		{
			class COpenSaveDialog : public CDialogWindow
			{
			public:
				enum EDialogType
				{
					Open = 0,
					Save,
					SaveAs
				};

			protected:
				CDataGridView* m_files;

				CButton* m_back;
				CButton* m_next;
				CButton* m_up;
				CButton* m_refresh;
				CButton* m_newfolder;

				CTextBox* m_path;
				CTextBox* m_search;
				CTextBox* m_fileName;

				EDialogType m_type;

				std::string m_folder;
				std::string m_currentFolder;

				std::vector<std::string> m_filter;
				std::vector<std::wstring> m_history;

			public:
				COpenSaveDialog(CBase* base, EDialogType type, const char* folder, const char* filter = "*.xml;*.*");

				virtual ~COpenSaveDialog();

				Listener OnSave;
				Listener OnOpen;
				Listener OnCancel;

			protected:

				void browseFolder(const char* folder);

				void onBtnBack(CBase* base);
				void onBtnNext(CBase* base);
				void onBtnRefresh(CBase* base);
				void onBtnNewFolder(CBase* base);

				void onSaveOpen(CBase* base);
				void onCancel(CBase* base);
			};
		}
	}
}