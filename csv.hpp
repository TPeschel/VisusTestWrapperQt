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

        QString
        __sep;

    public:

        QStringList
        __items;

    public:

        CSV( QString const &p_name = "data.csv" ) :
        __name( p_name ),
        __sep( "," ) {

        }

        virtual
        ~CSV( ) {

        }

        QString
        & operator [ ] ( int const &p_i ) {

            return __items[ p_i ];
        }

        int
        size( ) const {

            return __items.length( );
        }

        QString
        & name( QString const &p_name ) {

            return __name = p_name;
        }

        QString
        sep( QString const &p_sep = "," ) {

            if( !p_sep.isNull( ) )

                __sep = p_sep;

            return __sep;
        }

        bool
        loadFromFile( QString const & p_filename, bool p_setFileNameAsName = false ) {

            QFile
            csv_file( p_filename.isEmpty( ) ? __name : p_filename );

            if( csv_file.open( QIODevice::ReadOnly ) ) {

                QString
                txt = csv_file.readAll( );

                __items.append( txt.split( QRegExp( __sep ), QString::KeepEmptyParts ) );

                csv_file.close( );

                if( p_setFileNameAsName )

                    __name = p_filename;

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
        loadFromString( QString const &p_text ) {

            __items.append( p_text.split( QRegExp( __sep ), QString::KeepEmptyParts ) );
//            __items.append( p_text.split( __sep ) );
        }

        void
        writeToFile( QString const & p_filename ) {

            QFile
            csv_file( p_filename.isEmpty( ) ? __name : p_filename );

            if( csv_file.open( QIODevice::WriteOnly | QIODevice::Text ) ) {

                QTextStream
                ts( &csv_file );

                for( auto it = __items.cbegin( ); it != __items.cend( ); ++ it ) {

                    ts << ( *it ).toLatin1( ) << "\t";
                }

                csv_file.close( );
            }
        }
};

#endif // CSV_HPP
