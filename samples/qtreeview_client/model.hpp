
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

#ifndef COMO__SAMPLES__QLISTVIEW_CLIENT__MODEL_HPP__INCLUDED
#define COMO__SAMPLES__QLISTVIEW_CLIENT__MODEL_HPP__INCLUDED

// Qt include.
#include <QtCore/QAbstractTableModel>
#include <QtCore/QScopedPointer>
#include <QtGui/QColor>

// Como include.
#include <Como/ClientSocket>
#include <Como/Source>


//
// Model
//

//! Model with Como::Source sources.
class Model
	:	public QAbstractTableModel
{
	Q_OBJECT

public:
	Model( Como::ClientSocket * socket,
		QObject * parent = 0 );
	~Model();

	//! \return Color for the item with \arg index.
	QColor color( const QModelIndex & index );

	//! Clear model.
	void clear();

	//! \return Count of the rows.
	int rowCount( const QModelIndex & parent = QModelIndex() ) const;
	//! \return Count of the columns.
	int columnCount( const QModelIndex & parent = QModelIndex() ) const;
	//! \return Data by the given index and role.
	QVariant data( const QModelIndex & index,
		int role = Qt::DisplayRole ) const;
	//! Insert rows to the model.
	bool insertRows( int row, int count,
		const QModelIndex & parent = QModelIndex() );
	//! Remove rows from the model.
	bool removeRows( int row, int count,
		const QModelIndex & parent = QModelIndex() );
	//! Set data.
	bool setData( const QModelIndex & index, const QVariant & value,
		int role = Qt::EditRole );
	//! \return Flags.
	Qt::ItemFlags flags( const QModelIndex & index ) const;
	//! \return Header data.
	QVariant headerData( int section, Qt::Orientation orientation,
		int role = Qt::DisplayRole ) const;
	//! Sort model.
	void sort( int column, Qt::SortOrder order = Qt::AscendingOrder );

private slots:
	//! Source has been deinitialized.
	void slotSourceHasBeenDeinitialized( const Como::Source & source );
	//! Source has updated his value.
	void slotSourceHasUpdatedValue( const Como::Source & source );

private:
	Q_DISABLE_COPY( Model )

	//! Private implementation.
	struct ModelPrivate;
	//! Private implementation.
	QScopedPointer< ModelPrivate > d;
}; // class Model

#endif // COMO__SAMPLES__QLISTVIEW_CLIENT__MODEL_HPP__INCLUDED
