
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2012 Igor Mironchik

	Permission is hereby granted, free of charge, to any person
	obtaining a copy of this software and associated documentation
	files (the "Software"), to deal in the Software without
	restriction, including without limitation the rights to use,
	copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the
	Software is furnished to do so, subject to the following
	conditions:

	The above copyright notice and this permission notice shall be
	included in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
	OTHER DEALINGS IN THE SOFTWARE.
*/

// QTreeViewClient include.
#include "mainwindow.hpp"
#include "treeview.hpp"
#include "model.hpp"

// Qt include.
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QApplication>
#include <QHostAddress>

// Como include.
#include <Como/ClientSocket>


//
// MainWindow::MainWindowPrivate
//

struct MainWindow::MainWindowPrivate {
	MainWindowPrivate( Como::ClientSocket * socket )
		:	m_socket( socket )
		,	m_model( 0 )
	{}

	Como::ClientSocket * m_socket;
	Model * m_model;
}; // struct MainWindow::MainWindowPrivate


//
// MainWindow
//

MainWindow::MainWindow( Como::ClientSocket * socket,
	QWidget * parent, Qt::WindowFlags flags )
	:	QMainWindow( parent, flags )
	,	d( new MainWindowPrivate( socket ) )
{
	init();
}

MainWindow::~MainWindow()
{
}

void
MainWindow::init()
{
	QMenu * fileMenu = menuBar()->addMenu( tr( "File" ) );
	QAction * quitAction =
		fileMenu->addAction( tr( "Quit" ), this, SLOT( slotQuit() ) );
	quitAction->setShortcut( Qt::CTRL + Qt::Key_Q );

	d->m_model = new Model( d->m_socket, this );

	TreeView * treeView = new TreeView( d->m_model, this );
	treeView->setModel( d->m_model );

	setCentralWidget( treeView );

	connect( d->m_socket, &Como::ClientSocket::connected,
		this, &MainWindow::slotConnected );

	connect( d->m_socket, &Como::ClientSocket::disconnected,
		this, &MainWindow::slotDisconnected );

	slotDisconnected();
}

void
MainWindow::slotQuit()
{
	QApplication::quit();
}

void
MainWindow::slotConnected()
{
	d->m_socket->sendGetListOfSourcesMessage();
}

void
MainWindow::slotDisconnected()
{
	d->m_model->clear();
	d->m_socket->connectToHost( QHostAddress::LocalHost, 4545 );
}
