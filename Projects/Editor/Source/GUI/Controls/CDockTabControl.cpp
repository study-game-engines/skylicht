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

#include "pch.h"
#include "CWindow.h"
#include "CDockTabControl.h"
#include "CDockPanel.h"

namespace Skylicht
{
	namespace Editor
	{
		namespace GUI
		{
			CDockTabControl::CDockTabControl(CBase *parent, CDockPanel *panel) :
				CTabControl(parent),
				m_dockPanel(panel)
			{
				showTabCloseButton(true);
			}

			CDockableWindow* CDockTabControl::dockWindow(CDockableWindow *window)
			{
				CBase *ret = CTabControl::addPage(window->getCaption(), window);
				window->setStyleChild(true);
				window->setCurrentDockTab(this);
				return window;
			}

			void CDockTabControl::setCurrentWindow(CDockableWindow *window)
			{
				ListTabButton::iterator i = m_tabButtons.begin(), end = m_tabButtons.end();
				while (i != end)
				{
					if ((*i)->getPage() == window)
					{
						setTab((*i));
						return;
					}
					++i;
				}
			}

			void CDockTabControl::onDragOutTabStrip(CTabButton *button)
			{
				const SPoint& holdPosition = button->getDragHoldPosition();
				button->cancelDragCommand();

				CDockableWindow *win = dynamic_cast<CDockableWindow*>(button->getPage());
				if (win != NULL)
				{
					removeButtonOnly(button);

					win->setParent(win->getDockPanel());
					win->setCurrentDockTab(NULL);
					win->setHidden(false);
					win->setStyleChild(false);
					win->dock(EPosition::None);
					win->bringToFront();
					win->dragMoveCommand(holdPosition);
				}

				if (m_tabButtons.size() == 0)
					m_dockPanel->unDockChildWindow(this);
			}

			void CDockTabControl::doTabClose(CTabButton *button)
			{
				CTabControl::doTabClose(button);

				if (m_tabButtons.size() == 0)
					m_dockPanel->unDockChildWindow(this);
			}
		}
	}
}