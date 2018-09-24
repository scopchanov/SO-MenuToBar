/**
MIT License

Copyright (c) 2018 Michael Scopchanov

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "MainWindow.h"
#include <QBoxLayout>
#include <QRadioButton>
#include <QMenuBar>
#include <QMenu>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent)
{
	createCentralWidget();
	createMenuBar();
	resize(300, 200);
}

void MainWindow::createCentralWidget()
{
	auto *widget = new QWidget(this);
	auto *layoutMain = new QVBoxLayout(widget);
	auto *btnCompact = new QRadioButton(tr("Compact"), this);
	auto *btnExpanded = new QRadioButton(tr("Expanded"), this);

	btnCompact->setChecked(true);

	layoutMain->addWidget(btnCompact);
	layoutMain->addWidget(btnExpanded);
	layoutMain->addStretch();

	setCentralWidget(widget);

	connect(btnCompact, &QRadioButton::toggled, this, &MainWindow::compactMenus);
	connect(btnExpanded, &QRadioButton::toggled, this, &MainWindow::expandMenus);
}

void MainWindow::createMenuBar()
{
	auto *menubar = new QMenuBar(this);

	menubar->addMenu(createMenu(tr("Menu")));

	setMenuBar(menubar);
}

QMenu *MainWindow::createMenu(const QString &name)
{
	auto *menu = new QMenu(name, this);

	menu->addMenu(createSubmenu("Submenu 1",
								(ActionList({new QAction("Action 1", this),
											 new QAction("Action 2", this),
											 new QAction("Action 3", this)}))));
	menu->addMenu(createSubmenu("Submenu 2",
								(ActionList({new QAction("Action 4", this)}))));
	menu->addSeparator();
	menu->addMenu(createSubmenu("Submenu 3",
								(ActionList({new QAction("Action 5", this),
											 new QAction("Action 6", this)}))));

	return menu;
}

QMenu *MainWindow::createSubmenu(const QString &name, ActionList acts)
{
	auto *menu = new QMenu(name, this);

	menu->addActions(acts);

	return menu;
}

void MainWindow::compactMenus()
{
	if (!static_cast<QRadioButton *>(sender())->isChecked()
		|| menuBar()->actions().isEmpty())
		return;

	auto *menu = new QMenu(tr("Menu"), this);

	foreach (QAction *act, menuBar()->actions()) {
		if (act->menu())
			menu->addMenu(act->menu());
		else
			menu->addSeparator();

		menuBar()->removeAction(act);
	}

	menuBar()->addMenu(menu);
}

void MainWindow::expandMenus()
{
	if (!static_cast<QRadioButton *>(sender())->isChecked()
		|| menuBar()->actions().isEmpty())
		return;

	QAction *action = menuBar()->actions().first();
	QMenu *menu = action->menu();

	menuBar()->removeAction(action);

	foreach (QAction *act, menu->actions())
		if (act->menu())
			menuBar()->addMenu(act->menu());
		else
			menuBar()->addSeparator();

	menu->deleteLater();
}
