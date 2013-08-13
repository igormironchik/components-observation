
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
#include "model.hpp"

// Qt include.
#include <QString>
#include <QList>


//
// Model::ModelPrivate
//

struct Model::ModelPrivate {
	ModelPrivate()
		:	m_sortColumn( 0 )
		,	m_sortOrder( Qt::AscendingOrder )
	{
	}

	QList< Como::Source > m_data;
	int m_sortColumn;
	Qt::SortOrder m_sortOrder;
}; // struct Model::ModelPrivate


static const int NameColumn = 0;
static const int TypeNameColumn = 1;
static const int DateTimeColumn = 2;
static const int ValueColumn = 3;


//
// Model
//

Model::Model( Como::ClientSocket * socket, QObject * parent )
	:	QAbstractTableModel( parent )
	,	d( new ModelPrivate )
{
	connect( socket, SIGNAL( sourceDeinitialized( const Como::Source & ) ),
		this, SLOT( slotSourceHasBeenDeinitialized( const Como::Source & ) ) );

	connect( socket, SIGNAL( sourceHasUpdatedValue( const Como::Source & ) ),
		this, SLOT( slotSourceHasUpdatedValue( const Como::Source & ) ) );
}

Model::~Model()
{
}

QColor
Model::color( const QModelIndex & index )
{
	if( d->m_data.at( index.row() ).name() == QString( "ThirdObject" ) &&
		d->m_data.at( index.row() ).value() == QString( "I will die soon" ) )
			return Qt::red;
	else
		return Qt::white;
}

void
Model::slotSourceHasBeenDeinitialized( const Como::Source & source )
{
	const int index = d->m_data.indexOf( source );

	if( index >= 0 )
		removeRow( index );
}

void
Model::slotSourceHasUpdatedValue( const Como::Source & source )
{
	const int row = d->m_data.indexOf( source );

	if( row >= 0 )
	{
		setData( index( row, DateTimeColumn ), source.dateTime(), Qt::DisplayRole );
		if( d->m_sortColumn == DateTimeColumn )
			sort( d->m_sortColumn, d->m_sortOrder );

		setData( index( row, ValueColumn ), source.value(), Qt::DisplayRole );
		if( d->m_sortColumn == ValueColumn )
			sort( d->m_sortColumn, d->m_sortOrder );
	}
	else
	{
		const int row = d->m_data.size();

		insertRow( row );
		d->m_data[ row ] = source;

		emit dataChanged( QAbstractTableModel::index( row, NameColumn ),
			QAbstractTableModel::index( row, ValueColumn ) );

		sort( d->m_sortColumn, d->m_sortOrder );
	}
}

void
Model::clear()
{
	beginResetModel();
	d->m_data.clear();
	endResetModel();
}

int
Model::rowCount( const QModelIndex & parent ) const
{
	Q_UNUSED( parent )

	return d->m_data.size();
}

int
Model::columnCount( const QModelIndex & parent ) const
{
	Q_UNUSED( parent )

	return 4;
}

QVariant
Model::data( const QModelIndex & index, int role ) const
{
	const int column = index.column();

	if( role == Qt::DisplayRole )
	{
		switch( column )
		{
			case NameColumn : return d->m_data[ index.row() ].name();
			case TypeNameColumn : return d->m_data[ index.row() ].typeName();
			case DateTimeColumn : return d->m_data[ index.row() ].dateTime();
			case ValueColumn : return d->m_data[ index.row() ].value();
			default : return QVariant();
		}
	}
	else if( role == Qt::ToolTipRole )
		return d->m_data[ index.row() ].description();
	else if( role == Qt::UserRole )
	{
		QVariant value;
		value.setValue( d->m_data[ index.row() ] );
		return value;
	}
	else
		return QVariant();
}

bool
Model::insertRows( int row, int count, const QModelIndex & parent )
{
	Q_UNUSED( parent );

	beginInsertRows( QModelIndex(), row, row + count - 1 );

	for( int i = 0; i < count; ++i )
		d->m_data.insert( row + i, Como::Source() );

	endInsertRows();

	return true;
}

bool
Model::removeRows( int row, int count, const QModelIndex & parent )
{
	Q_UNUSED( parent );

	beginRemoveRows( QModelIndex(), row, row + count - 1 );

	for( int i = 0; i < count; ++i )
		d->m_data.removeAt( row );

	endRemoveRows();

	return true;
}

bool
Model::setData( const QModelIndex & index, const QVariant & value, int role )
{
	const int column = index.column();
	const int row = index.row();

	if( role == Qt::DisplayRole )
	{
		switch( column )
		{
			case NameColumn : d->m_data[ row ].setName( value.toString() ); break;
			case TypeNameColumn : d->m_data[ row ].setTypeName( value.toString() ); break;
			case DateTimeColumn : d->m_data[ row ].setDateTime( value.toDateTime() ); break;
			case ValueColumn : d->m_data[ row ].setValue( value ); break;
		}
	}

	emit dataChanged( QAbstractTableModel::index( index.row(), NameColumn ),
		QAbstractTableModel::index( index.row(), ValueColumn ) );

	return true;
}

Qt::ItemFlags
Model::flags( const QModelIndex & index ) const
{
	Q_UNUSED( index )

	return ( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
}

QVariant
Model::headerData( int section, Qt::Orientation orientation, int role ) const
{
	if( role != Qt::DisplayRole )
		return QVariant();

	if( orientation == Qt::Horizontal )
	{
		switch ( section )
		{
			case NameColumn : return tr( "Name" );
			case TypeNameColumn : return tr( "Type Name" );
			case DateTimeColumn : return tr( "Date and Time" );
			case ValueColumn : return tr( "Value" );
			default : return QVariant();
		}
	}

	return QVariant();
}

class SourceLessThenFunction {
public:
	explicit SourceLessThenFunction( int column )
		:	m_column( column )
	{}

	bool operator() ( const Como::Source & s1, const Como::Source & s2 )
	{
		switch( m_column )
		{
			case NameColumn :
				return s1.name() < s2.name();

			case TypeNameColumn :
				return s1.typeName() < s2.typeName();

			case DateTimeColumn :
				return s1.dateTime() < s2.dateTime();

			case ValueColumn :
				return s1.value().toString() < s2.value().toString();

			default :
				return false;
		}
	}

private:
	int m_column;
}; // class SourceLessThenFunction

class SourceGreaterThenFunction {
public:
	explicit SourceGreaterThenFunction( int column )
		:	m_column( column )
	{}

	bool operator() ( const Como::Source & s1, const Como::Source & s2 )
	{
		switch( m_column )
		{
			case NameColumn :
				return s1.name() > s2.name();

			case TypeNameColumn :
				return s1.typeName() > s2.typeName();

			case DateTimeColumn :
				return s1.dateTime() > s2.dateTime();

			case ValueColumn :
				return s1.value().toString() > s2.value().toString();

			default :
				return false;
		}
	}

private:
	int m_column;
}; // class SourceGreaterThenFunction

void
Model::sort( int column, Qt::SortOrder order )
{
	d->m_sortColumn = column;
	d->m_sortOrder = order;

	emit layoutAboutToBeChanged();

	if( order == Qt::AscendingOrder )
	{
		SourceLessThenFunction func( column );
		qSort( d->m_data.begin(), d->m_data.end(), func );
	}
	else
	{
		SourceGreaterThenFunction func( column );
		qSort( d->m_data.begin(), d->m_data.end(), func );
	}

	emit layoutChanged();
}
