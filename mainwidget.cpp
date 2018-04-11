#include "mainwidget.hpp"
#include "ui_mainwidget.h"
#include <QString>
#include <QTime>
#include <iostream>
#include "csv.hpp"

static QString const modeStrings[ ] = {

    "Acuity_LandoltCODWithoutTrialFrame",
    "Acuity_LandoltCOSWithoutTrialFrame",
    "Acuity_LandoltCODWithTrialFrame",
    "Acuity_LandoltCOSWithTrialFrame",
    "Acuity_LandoltCODWithHoleAperture",
    "Acuity_LandoltCOSWithHoleAperture"
};

MainWidget::MainWidget(QWidget *parent) :
	QWidget(parent),
    ui(new Ui::MainWidget)
{
	ui->setupUi(this);

    proc = 0;

	QObject::connect( ui->pushButtonNewExamination, SIGNAL(	released( ) ), this, SLOT(slotNewExamination( ) ) );
    QObject::connect( ui->lineEditSICVal, SIGNAL( textChanged( QString ) ), this, SLOT( slotScanSIC( ) ) );
    QObject::connect( ui->pushButtonFinishExamination, SIGNAL( released( ) ), this, SLOT( slotFinishExamination( ) ) );
    QObject::connect( ui->pushButtonODWithoutTrialFrame, SIGNAL( released( ) ), this, SLOT( slotStartFrACTAcuity_LandoltCODWithoutTrialFrame( ) ) );
    QObject::connect( ui->pushButtonOSWithoutTrialFrame, SIGNAL( released( ) ), this, SLOT( slotStartFrACTAcuity_LandoltCOSWithoutTrialFrame( ) ) );
    QObject::connect( ui->pushButtonODWithTrialFrame, SIGNAL( released( ) ), this, SLOT( slotStartFrACTAcuity_LandoltCODWithTrialFrame( ) ) );
    QObject::connect( ui->pushButtonOSWithTrialFrame, SIGNAL( released( ) ), this, SLOT( slotStartFrACTAcuity_LandoltCOSWithTrialFrame( ) ) );
    QObject::connect( ui->pushButtonODWithHoleAperture, SIGNAL( released( ) ), this, SLOT( slotStartFrACTAcuity_LandoltCODWithHoleAperture( ) ) );
    QObject::connect( ui->pushButtonOSWithHoleAperture, SIGNAL( released( ) ), this, SLOT( slotStartFrACTAcuity_LandoltCOSWithHoleAperture( ) ) );
    QObject::connect( ui->pushButtonClose, SIGNAL( released( ) ), this, SLOT( close( ) ) );

    csv_output.name( "data.csv" );
    csv_output.sep( "[\n\t\r]+" );
    csv_output.loadFromFile( "data.csv" );
}

MainWidget::~MainWidget()
{
	delete ui;
}

void MainWidget::slotNewExamination( )
{
	ui->lineEditSICVal->setText( "----------" );
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
    }
}

void MainWidget::slotFinishExamination( )
{
    ui->toolBox->setCurrentIndex( 3 );
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

//    cb->blockSignals( true );

    //proc->blockSignals( true );

    QString
    txt = QApplication::clipboard( )->text( QClipboard::Clipboard );

    QTime
    tm = QTime::currentTime( );

    std::cout << tm.hour( ) << ":" << tm.minute( ) << ":" << tm.second( ) << "." << tm.msec( ) << " : >> " << txt.toStdString( ) << std::endl;

    CSV
    csv( "clipboard.csv" );

    csv.sep( "[\t\n\r]+" );

    csv.loadFromString( txt );

    if( 0 < csv.size( ) && csv[ 0 ][ 0 ] == '2' ) {

        QFile
        f( currSIC + "[" + modeStrings[ currMode ] + "].csv" );

        if( f.open( QIODevice::WriteOnly | QIODevice::Text ) ) {

            QTextStream
            ts( &f );

            ts << currSIC << "\t" << modeStrings[ currMode ] << "\t" << txt;

            f.close( );
        }

        csv_output.__items <<
            currSIC << "\t" <<
            csv[ 0 ] << "\t" <<
            csv[ 1 ] << "\t" <<
            csv[ 2 ] << "\t" <<
            csv[ 3 ] << "\t" <<
            csv[ 4 ] << "\t" <<
            csv[ 5 ] << "\t" <<
            csv[ 6 ] << "\t" <<
            csv[ 7 ] << "\t" <<
            csv[ 8 ] << "\t" <<
            csv[ 9 ] << "\t" <<
            csv[ 10 ] << "\t" <<
            csv[ 11 ] << "\t" <<
            csv[ 12 ] << "\t" <<
            csv[ 13 ] << "\n";

        switch ( currMode ) {

            case Acuity_LandoltCODWithoutTrialFrame : {

                ui->labelODWithoutTrialFrame->setText( csv[ 2 ] );

                if( ui->labelODWithTrialFrame->text( )[ 0 ] != 's' ) {

                    double
                    oDWithTF    = ui->labelODWithTrialFrame->text( ).toDouble( ),
                    oDWithoutTF = ui->labelODWithoutTrialFrame->text( ).toDouble( );

                    if( oDWithoutTF > oDWithTF ) {

                        ui->pushButtonODWithHoleAperture->setEnabled( true );
                        ui->labelODWithHoleAperture->setText( "Measurement without trial frame is better than the one without. Please measure now with hole aperture!");
                    }
                }

                break;
            }

            case Acuity_LandoltCOSWithoutTrialFrame : {

                ui->labelOSWithoutTrialFrame->setText( csv[ 2 ] );

                break;
            }

            case Acuity_LandoltCODWithTrialFrame : {

                ui->labelODWithTrialFrame->setText( csv[ 2 ] );

                if( ui->labelODWithoutTrialFrame->text( )[ 0 ] != 's' ) {

                    double
                    oDWithTF    = ui->labelODWithTrialFrame->text( ).toDouble( ),
                    oDWithoutTF = ui->labelODWithoutTrialFrame->text( ).toDouble( );

                    if( oDWithoutTF > oDWithTF ) {

                        ui->pushButtonODWithHoleAperture->setEnabled( true );
                        ui->labelODWithHoleAperture->setText( "Measurement without trial frame is better than the one without. Please measure now with hole aperture!");
                    }
                }
                break;
            }

            case Acuity_LandoltCOSWithTrialFrame : {

                ui->labelOSWithTrialFrame->setText( csv[ 2 ] );

                break;
            }

            case Acuity_LandoltCODWithHoleAperture : {

                ui->labelODWithHoleAperture->setText( csv[ 2 ] );

                break;
            }

            case Acuity_LandoltCOSWithHoleAperture : {

                ui->labelOSWithHoleAperture->setText( csv[ 2 ] );

                break;
            }

            default:
                break;
        }

    }

/*
    cb->blockSignals( true );
    cb->clear( );
    cb->blockSignals( false );
*/
    //proc->blockSignals( false );
}

void MainWidget::slotStartFrACTAcuity_LandoltCODWithoutTrialFrame()
{
    currMode = Acuity_LandoltCODWithoutTrialFrame;
    startFrACT( );
}

void MainWidget::slotStartFrACTAcuity_LandoltCOSWithoutTrialFrame()
{
    currMode = Acuity_LandoltCOSWithoutTrialFrame;
    startFrACT( );
}

void MainWidget::slotStartFrACTAcuity_LandoltCODWithTrialFrame()
{
    currMode = Acuity_LandoltCODWithTrialFrame;
    startFrACT( );
}

void MainWidget::slotStartFrACTAcuity_LandoltCOSWithTrialFrame()
{
    currMode = Acuity_LandoltCOSWithTrialFrame;
    startFrACT( );
}

void MainWidget::slotStartFrACTAcuity_LandoltCODWithHoleAperture()
{
    currMode = Acuity_LandoltCODWithHoleAperture;
    startFrACT( );
}

void MainWidget::slotStartFrACTAcuity_LandoltCOSWithHoleAperture()
{
    currMode = Acuity_LandoltCOSWithHoleAperture;
    startFrACT( );
}

void MainWidget::closeEvent( QCloseEvent *p_closeEvent )
{

    p_closeEvent->accept( );
//http://doc.qt.io/qt-5/qtxml-streambookmarks-example.html
     csv_output.writeToFile( "data.csv" );
    //MainWidget::closeEvent(p_closeEvent );
}
