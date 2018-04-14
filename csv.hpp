#ifndef CSV_HPP
#define CSV_HPP

#include <QString>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QRegExp>

class CSV :
public QObject {

        Q_OBJECT

    private:

        QString
        __name;

        QChar
        __sep;

        int
        __columns;

        bool
        __header;

        QStringList
        __items;

    public:

        CSV( QString const &p_name = "data.csv" ) :
        __name( p_name ),
        __sep( ',' ),
        __columns( 0 ),
        __items( ) {

        }

        virtual
        ~CSV( ) {

        }

        QString
        operator [ ] ( int const &p_i ) const {

            return __items[ p_i ];
        }

        QString
        & operator [ ] ( int const &p_i ) {

            return __items[ p_i ];
        }

        int
        columns( ) const {

            return __columns;
        }

        int
        size( ) const {

            return __items.length( );
        }

        QString
        & name( QString const &p_name ) {

            return __name = p_name;
        }

        QChar
        sep( QChar const &p_sep = ',' ) {

            if( !p_sep.isNull( ) )

                __sep = p_sep;

            return __sep;
        }

        QStringList
        & items( ) {

            return __items;
        }

        bool
        loadFromFile( QString const & p_filename, QChar const & p_sep, bool p_header = true, QIODevice::OpenMode const & p_openMode = ( QIODevice::ReadOnly | QIODevice::Text ) ) {

            QFile
            csv_file( p_filename );

            if( csv_file.open( p_openMode ) ) {

                QString
                hdr = csv_file.readLine( );

                __header  = p_header;
                __sep     = p_sep;
                __name    = p_filename;

                QStringList
                hdrTmp = hdr.split( __sep );

                __columns = hdrTmp.length( );

                QString
                txt = csv_file.readAll( ),
                re = QString( "[%1\n\r]+" ).arg( __sep );

                __items.append( hdrTmp );
                __items.append( txt.split( QRegExp( re ), QString::SkipEmptyParts ) );

                csv_file.close( );

                return true;
            }

            return false;
        }

        void
        append( CSV & p_csv ) {

            for( int i = 0; i < p_csv.size( ); ++ i ) {

                __items.append( p_csv[ i ] );
            }
        }

        void
        loadFromString( QString const & p_text, QChar const & p_sep, bool p_header, int const & p_columns ) {

            __columns = p_columns;
            __header  = p_header;
            __sep     = p_sep;
            __items.append( p_text.split( QRegExp( QString( "[%1\n\r]+" ).arg( __sep ) ), QString::SkipEmptyParts ) );
        }

        void
        writeToFile( QString const & p_filename = "", QChar const & p_sep = ',', QIODevice::OpenMode const & p_openMode = QIODevice::WriteOnly | QIODevice::Text ) {

            QFile
            csv_file( p_filename.isEmpty( ) ? __name : p_filename );

            if( csv_file.open( p_openMode ) ) {

                QTextStream
                ts( &csv_file );

                if( !p_sep.isNull( ) ) {

                    __sep = p_sep;
                }

                int
                c = -1;

                for( auto it = __items.cbegin( ); it != __items.cend( ); ++ it ) {

                    ts << ( *it ).toLatin1( ) << ( ( ++ c % __columns ) < __columns ? __sep : '\n' );
                }

                csv_file.close( );
            }
        }

        QStringList
        & operator << ( QString const & p_text ) {

            __items << p_text;

            return __items;
        }
};

#endif // CSV_HPP
