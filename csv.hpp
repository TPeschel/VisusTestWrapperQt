#ifndef CSV_HPP
#define CSV_HPP

#include <QString>
#include <QStringList>
#include <QFile>
#include <QTextStream>

class CSV :
public QObject {

        Q_OBJECT

    private:

        QString
        name;

        QChar
        __sep;

        QStringList
        items;

    public:

        CSV( QString const &p_name = "daten.csv", QChar const & p_sep = ',' ) :
        name( p_name ),
        __sep( p_sep ) {

        }

        virtual
        ~CSV( ) {

        }

        QChar
        sep( QChar const &p_sep = ',' ) {

            if( !p_sep.isNull( ) )

                __sep = p_sep;

            return __sep;
        }

        bool
        loadFromFile( QString const & p_filename, bool p_setFileNameAsName = false ) {

            QFile
            csv_file( p_filename );

            if( csv_file.open( QIODevice::ReadOnly ) ) {

                QString
                txt = csv_file.readAll( );

                items = txt.split( __sep );

                csv_file.close( );

                if( p_setFileNameAsName )

                    name = p_filename;

                return true;
            }

            return false;
        }

        void
        loadFromString( QString const &p_text ) {

            items.append( p_text.split( __sep ) );
        }

        void
        writeToFile( QString const & p_filename ) {

            QFile
            csv_file( p_filename );

            if( csv_file.open( QIODevice::ReadWrite | QIODevice::Text ) ) {

                QTextStream
                ts( &csv_file );

                for( auto it = items.cbegin( ); it != items.cend( ); ++ it ) {

                    ts << *it << __sep;
                }

                csv_file.close( );
            }
        }
};

#endif // CSV_HPP
