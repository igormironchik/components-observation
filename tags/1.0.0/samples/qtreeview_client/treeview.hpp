
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

#ifndef COMO__SAMPLES__QLISTVIEW_CLIENT__TREEVIEW_HPP__INCLUDED
#define COMO__SAMPLES__QLISTVIEW_CLIENT__TREEVIEW_HPP__INCLUDED

// Qt include.
#include <QtGui/QTreeView>
#include <QtCore/QScopedPointer>


class Model;


//
// TreeView
//

//! ListView with Como::Source sources.
class TreeView
	:	public QTreeView
{
	Q_OBJECT

public:
	explicit TreeView( Model * model, QWidget * parent = 0 );
	~TreeView();

protected:
	void drawRow( QPainter * painter,
		const QStyleOptionViewItem & option,
		const QModelIndex & index ) const;

protected slots:
	void dataChanged( const QModelIndex & topLeft,
		const QModelIndex & bottomRight );

private:
	void init();

private:
	Q_DISABLE_COPY( TreeView )

	struct TreeViewPrivate;
	QScopedPointer< TreeViewPrivate > d;
}; // class TreeView

#endif // COMO__SAMPLES__QLISTVIEW_CLIENT__TREEVIEW_HPP__INCLUDED
