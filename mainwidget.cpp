#include "mainwidget.hpp"
#include "ui_mainwidget.h"
#include <QString>
#include <QTime>
#include <iostream>
#include "csv.hpp"

static QString const modeStrings[ ] = {

    "OD_PLACEHOLDER_WithoutTrialFrame",
    "OS_PLACEHOLDER_WithoutTrialFrame",
    "OD_PLACEHOLDER_WithTrialFrame",
    "OS_PLACEHOLDER_WithTrialFrame",
    "OD_PLACEHOLDER_WithHoleAperture",
    "OS_PLACEHOLDER_WithHoleAperture"
};

MainWidget::MainWidget(QWidget *parent) :
	QWidget(parent),
    ui(new Ui::MainWidget)
{
	ui->setupUi(this);

    proc = 0;

    QObject::connect( ui->pushButtonNewExamination, SIGNAL(	released( ) ), this, SLOT( slotNewExamination( ) ) );
    QObject::connect( ui->pushButtonNewExaminationAfterFinish, SIGNAL(	released( ) ), this, SLOT( slotNextExamination( ) ) );
    QObject::connect( ui->lineEditSICVal, SIGNAL( textChanged( QString ) ), this, SLOT( slotScanSIC( ) ) );
    QObject::connect( ui->pushButtonFinishExamination, SIGNAL( released( ) ), this, SLOT( slotFinishExamination( ) ) );
    QObject::connect( ui->pushButtonODWithoutTrialFrame, SIGNAL( released( ) ), this, SLOT( slotStartFrACTAcuity_LandoltCODWithoutTrialFrame( ) ) );
    QObject::connect( ui->pushButtonOSWithoutTrialFrame, SIGNAL( released( ) ), this, SLOT( slotStartFrACTAcuity_LandoltCOSWithoutTrialFrame( ) ) );
    QObject::connect( ui->pushButtonODWithTrialFrame, SIGNAL( released( ) ), this, SLOT( slotStartFrACTAcuity_LandoltCODWithTrialFrame( ) ) );
    QObject::connect( ui->pushButtonOSWithTrialFrame, SIGNAL( released( ) ), this, SLOT( slotStartFrACTAcuity_LandoltCOSWithTrialFrame( ) ) );
    QObject::connect( ui->pushButtonODWithHoleAperture, SIGNAL( released( ) ), this, SLOT( slotStartFrACTAcuity_LandoltCODWithHoleAperture( ) ) );
    QObject::connect( ui->pushButtonOSWithHoleAperture, SIGNAL( released( ) ), this, SLOT( slotStartFrACTAcuity_LandoltCOSWithHoleAperture( ) ) );
    QObject::connect( ui->pushButtonClose, SIGNAL( released( ) ), this, SLOT( close( ) ) );

    ui->toolBox->widget( 0 )->setEnabled( true );
    ui->toolBox->widget( 1 )->setEnabled( false );
    ui->toolBox->widget( 2 )->setEnabled( false );
    ui->toolBox->widget( 3 )->setEnabled( false );

    csv_output.name( "data.csv" );
    csv_output.sep( "[\t\n\r]+" );
    csv_output.loadFromFile( "data.csv" );
}

MainWidget::~MainWidget()
{
	delete ui;
}



void MainWidget::slotNewExamination( )
{
	ui->toolBox->setCurrentIndex( 1 );
	ui->toolBox->setItemText( 0, "Running Examination" );
    ui->lineEditSICVal->setFocus( );
    ui->lineEditSICVal->setText( "" );
    ui->pushButtonODWithoutTrialFrame->setEnabled( true );
    ui->pushButtonOSWithoutTrialFrame->setEnabled( true );
    ui->pushButtonODWithTrialFrame->setEnabled( true );
    ui->pushButtonOSWithTrialFrame->setEnabled( true );
    ui->pushButtonODWithHoleAperture->setEnabled( false );
    ui->pushButtonOSWithHoleAperture->setEnabled( false );
    ui->labelODWithoutTrialFrame->setText( "still no value measured" );
    ui->labelOSWithoutTrialFrame->setText( "still no value measured" );
    ui->labelODWithTrialFrame->setText( "still no value measured" );
    ui->labelOSWithTrialFrame->setText( "still no value measured" );
    ui->labelODWithHoleAperture->setText( "still no value measured" );
    ui->labelOSWithHoleAperture->setText( "still no value measured" );

    ui->pushButtonODWithHoleAperture->setEnabled( false );
    ui->labelODWithHoleApertureMsg->setStyleSheet( "QLabel { color : black; }" );
    ui->labelODWithHoleApertureMsg->setText( "no measurement needed" );

    ui->pushButtonOSWithHoleAperture->setEnabled( false );
    ui->labelOSWithHoleApertureMsg->setStyleSheet( "QLabel { color : black; }" );
    ui->labelOSWithHoleApertureMsg->setText( "no measurement needed" );

    ui->toolBox->widget( 0 )->setEnabled( false );
    ui->toolBox->widget( 1 )->setEnabled( true );
    ui->toolBox->widget( 2 )->setEnabled( false );
    ui->toolBox->widget( 3 )->setEnabled( false );

    ui->lineEditSICVal->setFocus( );
}

void MainWidget::slotScanSIC( )
{

    QString scannedSIC = ui->lineEditSICVal->text( );
    QRegularExpression re( "^LI\\d{7}[\\d{1}xX]", QRegularExpression::CaseInsensitiveOption );
    QRegularExpressionMatch m = re.match( scannedSIC );

    if( m.hasMatch( ) ) {

        currSIC = m.captured( ).toUpper( );
        ui->toolBox->setItemText( 1, currSIC );
        ui->toolBox->setCurrentIndex( 2 );

        ui->toolBox->widget( 0 )->setEnabled( false );
        ui->toolBox->widget( 1 )->setEnabled( false );
        ui->toolBox->widget( 2 )->setEnabled( true );
        ui->toolBox->widget( 3 )->setEnabled( false );

        ui->pushButtonODWithoutTrialFrame->setFocus( );
    }
}

void MainWidget::slotFinishExamination( )
{
    ui->toolBox->setItemText( 0, "Complete Examination" );
    ui->toolBox->setCurrentIndex( 3 );

    ui->toolBox->widget( 0 )->setEnabled( false );
    ui->toolBox->widget( 1 )->setEnabled( false );
    ui->toolBox->widget( 2 )->setEnabled( false );
    ui->toolBox->widget( 3 )->setEnabled( true );
}

void MainWidget::slotNextExamination( )
{
    ui->toolBox->setCurrentIndex( 0 );
    ui->toolBox->widget( 0 )->setEnabled( true );
    ui->toolBox->widget( 1 )->setEnabled( false );
    ui->toolBox->widget( 2 )->setEnabled( false );
    ui->toolBox->widget( 3 )->setEnabled( false );

    ui->toolBox->setItemText( 0, "New Examination" );
    ui->toolBox->setItemText( 1, "New Proband" );
    ui->lineEditSICVal->setText( "" );
}

void MainWidget::startFrACT( )
{
    QApplication::clipboard( )->clear( QClipboard::Clipboard );

    if( proc )

        delete proc;

    proc = new QProcess( );

    QObject::connect( proc, SIGNAL( finished( int ) ), this, SLOT( slotFrACTFinished( int ) ) );

    QStringList
    arg;

    arg << "FrACT3.9.8.swf";

//    proc->setWorkingDirectory ( "C:/Users/Thomas Peschel/Documents/dev/c++/life/Franziska/github.com/TPeschel/build-VisusTestWrapper-Desktop_Qt_5_10_1_MinGW_32bit-Debug" );
    proc->setWorkingDirectory ( "." );

    //proc->start( "FrACT3.9.9a.exe" );

    proc->start( "flashplayer_29_sa.exe", arg ); //windows
//    proc->startDetached ( "./flashplayer", arg ); //linux
}

void MainWidget::slotFrACTFinished( int exitCode )
{

    if( exitCode != 0 )

        return;

    QString
    txt = QApplication::clipboard( )->text( QClipboard::Clipboard );

    CSV
    csv( "clipboard.csv" );

    csv.sep( "[\t\n\r]+" );

    csv.loadFromString( txt );

    if( 0 < csv.size( ) && csv[ 0 ][ 0 ] == '2' ) {

        QStringList
        datetime;

        datetime << csv[ 0 ].split( '-' ) << csv[ 1 ].split( ':' );

        QString
        dt = datetime.join( "-" );

        QStringList
        msl = modeStrings[ currMode ].split( "_PLACEHOLDER_" );

        QFile
        f( currSIC + "-" + dt + "-" + msl[ 0 ] + "-" + msl[ 1 ] + ".csv" );

        if( f.open( QIODevice::WriteOnly | QIODevice::Text ) ) {

            QTextStream
            ts( &f );

            ts <<
                currSIC << "\t" <<
                csv[ 0 ] << "\t" <<
                csv[ 1 ] << "\t" <<
                csv[ 2 ] << "\t" <<
                csv[ 3 ] << "\t" <<
                msl[ 0 ] << "\t" <<
                msl[ 1 ] << "\t" <<
                csv[ 4 ] << "\t" <<
                csv[ 5 ] << "\t" <<
                csv[ 6 ] << "\t" <<
                csv[ 7 ] << "\t" <<
                csv[ 8 ] << "\t" <<
                csv[ 9 ] << "\t" <<
                csv[ 10 ] << "\t" <<
                csv[ 11 ] << "\t" <<
                csv[ 12 ] << "\n";

            int i = 12;

            while( ++i < csv.__items.length( ) ) {

                ts << csv[ i ];

                ts << ( ( i - 13 ) % 6 < 5 ? "\t" : "\n" );
            }

            f.close( );
        }

        csv_output.__items <<
            currSIC << "\t" <<
            csv[ 0 ] << "\t" <<
            csv[ 1 ] << "\t" <<
            csv[ 2 ] << "\t" <<
            csv[ 3 ] << "\t" <<
            msl[ 0 ] << "\t" <<
            msl[ 1 ] << "\t" <<
            csv[ 4 ] << "\t" <<
            csv[ 5 ] << "\t" <<
            csv[ 6 ] << "\t" <<
            csv[ 7 ] << "\t" <<
            csv[ 8 ] << "\t" <<
            csv[ 9 ] << "\t" <<
            csv[ 10 ] << "\t" <<
            csv[ 11 ] << "\t" <<
            csv[ 12 ] << "\n";

        ui->plainTextEditSummary->appendPlainText( csv.__items.join( "  " ) );

        switch ( currMode ) {

            case ODWithoutTrialFrame : {

                ui->labelODWithoutTrialFrame->setText( csv[ 2 ] );

                if( ui->labelODWithTrialFrame->text( )[ 0 ] != 's' ) {

                    double
                    oDWithTF    = ui->labelODWithTrialFrame->text( ).toDouble( ),
                    oDWithoutTF = ui->labelODWithoutTrialFrame->text( ).toDouble( );

                    if( oDWithoutTF > oDWithTF ) {

                        ui->pushButtonODWithHoleAperture->setEnabled( true );
                        ui->labelODWithHoleApertureMsg->setStyleSheet( "QLabel { color : red; }" );
                        ui->labelODWithHoleApertureMsg->setText( "Measurement without trial frame is better than the one without.\nPlease measure now with hole aperture!" );
                    }
                }

                break;
            }

            case OSWithoutTrialFrame : {

                ui->labelOSWithoutTrialFrame->setText( csv[ 2 ] );

                if( ui->labelOSWithTrialFrame->text( )[ 0 ] != 's' ) {

                    double
                    oSWithTF    = ui->labelOSWithTrialFrame->text( ).toDouble( ),
                    oSWithoutTF = ui->labelOSWithoutTrialFrame->text( ).toDouble( );

                    if( oSWithoutTF > oSWithTF ) {

                        ui->pushButtonOSWithHoleAperture->setEnabled( true );
                        ui->labelOSWithHoleApertureMsg->setStyleSheet( "QLabel { color : red; }" );
                        ui->labelOSWithHoleApertureMsg->setText( "Measurement without trial frame is better than the one without.\nPlease measure now with hole aperture!" );
                    }
                }

                break;
            }

            case ODWithTrialFrame : {

                ui->labelODWithTrialFrame->setText( csv[ 2 ] );

                if( ui->labelODWithoutTrialFrame->text( )[ 0 ] != 's' ) {

                    double
                    oDWithTF    = ui->labelODWithTrialFrame->text( ).toDouble( ),
                    oDWithoutTF = ui->labelODWithoutTrialFrame->text( ).toDouble( );

                    if( oDWithoutTF > oDWithTF ) {

                        ui->pushButtonODWithHoleAperture->setEnabled( true );
                        ui->labelODWithHoleApertureMsg->setStyleSheet( "QLabel { color : red; }" );
                        ui->labelODWithHoleApertureMsg->setText( "Measurement without trial frame is better than the one without.\nPlease measure now with hole aperture!" );
                    }
                }

                break;
            }

            case OSWithTrialFrame : {

                ui->labelOSWithTrialFrame->setText( csv[ 2 ] );

                if( ui->labelOSWithoutTrialFrame->text( )[ 0 ] != 's' ) {

                    double
                    oSWithTF    = ui->labelOSWithTrialFrame->text( ).toDouble( ),
                    oSWithoutTF = ui->labelOSWithoutTrialFrame->text( ).toDouble( );

                    if( oSWithoutTF > oSWithTF ) {

                        ui->pushButtonOSWithHoleAperture->setEnabled( true );
                        ui->labelOSWithHoleApertureMsg->setStyleSheet( "QLabel { color : red; }" );
                        ui->labelOSWithHoleApertureMsg->setText( "Measurement without trial frame is better than the one without.\nPlease measure now with hole aperture!" );
                    }
                }

                break;
            }

            case ODWithHoleAperture : {

                ui->labelODWithHoleAperture->setText( csv[ 2 ] );

                break;
            }

            case OSWithHoleAperture : {

                ui->labelOSWithHoleAperture->setText( csv[ 2 ] );

                break;
            }

            default:
                break;
        }
    }
}

void MainWidget::slotStartFrACTAcuity_LandoltCODWithoutTrialFrame()
{
    currMode = ODWithoutTrialFrame;
    startFrACT( );
}

void MainWidget::slotStartFrACTAcuity_LandoltCOSWithoutTrialFrame()
{
    currMode = OSWithoutTrialFrame;
    startFrACT( );
}

void MainWidget::slotStartFrACTAcuity_LandoltCODWithTrialFrame()
{
    currMode = ODWithTrialFrame;
    startFrACT( );
}

void MainWidget::slotStartFrACTAcuity_LandoltCOSWithTrialFrame()
{
    currMode = OSWithTrialFrame;
    startFrACT( );
}

void MainWidget::slotStartFrACTAcuity_LandoltCODWithHoleAperture()
{
    currMode = ODWithHoleAperture;
    startFrACT( );
}

void MainWidget::slotStartFrACTAcuity_LandoltCOSWithHoleAperture()
{
    currMode = OSWithHoleAperture;
    startFrACT( );
}

void MainWidget::closeEvent( QCloseEvent *p_closeEvent )
{

    p_closeEvent->accept( );
//http://doc.qt.io/qt-5/qtxml-streambookmarks-example.html
     csv_output.writeToFile( "data.csv" );
    //MainWidget::closeEvent(p_closeEvent );
}
