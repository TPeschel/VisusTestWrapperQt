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

    csv_config.loadFromFile( "config.tsv", '\t', true );

    //    flashPlayerExe = ui->lineEditFlashplayerExe->text( );
    //    fractSWF = ui->lineEditFrACTSWF->text( );
    //    dataDir = ui->lineEditDataDir->text( );

    flashPlayerExe = csv_config[ 3 ];
    fractSWF = csv_config[ 4 ];
    dataDir = csv_config[ 5 ];

    ui->lineEditFlashplayerExe->setText( flashPlayerExe );
    ui->lineEditFrACTSWF->setText( fractSWF );
    ui->lineEditDataDir->setText( dataDir );

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
    QObject::connect( ui->toolButtonFlashPlayerExe, SIGNAL( released( ) ), this, SLOT( slotStartFileDialoagForFlashPlayer( ) ) );
    QObject::connect( ui->toolButtonFrACTSWF, SIGNAL( released( ) ), this, SLOT( slotStartFileDialoagForFractSWF( ) ) );
    QObject::connect( ui->toolButtonDataDir, SIGNAL( released( ) ), this, SLOT( slotStartFileDialoagForDataDir( ) ) );

    ui->toolBox->widget( 0 )->setEnabled( true );
    ui->toolBox->widget( 1 )->setEnabled( true );
    ui->toolBox->widget( 2 )->setEnabled( false );
    ui->toolBox->widget( 3 )->setEnabled( false );
    ui->toolBox->widget( 4 )->setEnabled( false );
    ui->toolBox->setCurrentIndex( 1 );

    csv_output.loadFromFile( "data.csv", '\t' );
}

MainWidget::~MainWidget()
{
	delete ui;
}

void MainWidget::slotNewExamination( )
{
    ui->toolBox->setCurrentIndex( 2 );
    ui->toolBox->setItemText( 1, "Running Examination" );
    ui->lineEditSICVal->setFocus( );
    ui->lineEditSICVal->setText( "" );

    ui->pushButtonODWithoutTrialFrame->setStyleSheet( "QPushButton { color : black; }" );
    ui->pushButtonODWithoutTrialFrame->setEnabled( true );

    ui->pushButtonOSWithoutTrialFrame->setStyleSheet( "QPushButton { color : black; }" );
    ui->pushButtonOSWithoutTrialFrame->setEnabled( true );

    ui->pushButtonODWithTrialFrame->setStyleSheet( "QPushButton { color : black; }" );
    ui->pushButtonODWithTrialFrame->setEnabled( true );

    ui->pushButtonOSWithTrialFrame->setStyleSheet( "QPushButton { color : black; }" );
    ui->pushButtonOSWithTrialFrame->setEnabled( true );

    ui->pushButtonODWithHoleAperture->setStyleSheet( "QPushButton { color : black; }" );
    ui->pushButtonODWithHoleAperture->setEnabled( false );

    ui->pushButtonOSWithHoleAperture->setStyleSheet( "QPushButton { color : black; }" );
    ui->pushButtonOSWithHoleAperture->setEnabled( false );
/*
    ui->labelODWithoutTrialFrame->setStyleSheet( "QLabel { color : black; }" );
    ui->labelODWithoutTrialFrame->setText( "noch kein Wert gemessen" );

    ui->labelOSWithoutTrialFrame->setStyleSheet( "QLabel { color : black; }" );
    ui->labelOSWithoutTrialFrame->setText( "noch kein Wert gemessen" );

    ui->labelODWithTrialFrame->setStyleSheet( "QLabel { color : black; }" );
    ui->labelODWithTrialFrame->setText( "noch kein Wert gemessen" );

    ui->labelOSWithTrialFrame->setStyleSheet( "QLabel { color : black; }" );
    ui->labelOSWithTrialFrame->setText( "noch kein Wert gemessen" );

    ui->labelODWithHoleAperture->setStyleSheet( "QLabel { color : black; }" );
    ui->labelODWithHoleAperture->setText( "keine Messung nötig" );

    ui->labelOSWithHoleAperture->setStyleSheet( "QLabel { color : black; }" );
    ui->labelOSWithHoleAperture->setText( "keine Messung nötig" );
*/

    ui->toolBox->widget( 1 )->setEnabled( false );
    ui->toolBox->widget( 2 )->setEnabled( true );
    ui->toolBox->widget( 3 )->setEnabled( false );
    ui->toolBox->widget( 4 )->setEnabled( false );

    ui->lineEditSICVal->setFocus( );
}

void MainWidget::slotScanSIC( )
{

    QString scannedSIC = ui->lineEditSICVal->text( );
    QRegularExpression re( "^LI\\d{7}[\\d{1}xX]", QRegularExpression::CaseInsensitiveOption );
    QRegularExpressionMatch m = re.match( scannedSIC );

    if( m.hasMatch( ) ) {

        currSIC = m.captured( ).toUpper( );
        ui->toolBox->setItemText( 2, currSIC );
        ui->toolBox->setCurrentIndex( 3 );

        ui->toolBox->widget( 1 )->setEnabled( false );
        ui->toolBox->widget( 2 )->setEnabled( false );
        ui->toolBox->widget( 3 )->setEnabled( true );
        ui->toolBox->widget( 4 )->setEnabled( false );

        ui->pushButtonODWithoutTrialFrame->setFocus( );
    }
}

void MainWidget::slotFinishExamination( )
{
    ui->toolBox->setItemText( 1, "Complete Examination" );
    ui->toolBox->setCurrentIndex( 4 );

    ui->toolBox->widget( 1 )->setEnabled( false );
    ui->toolBox->widget( 2 )->setEnabled( false );
    ui->toolBox->widget( 3 )->setEnabled( false );
    ui->toolBox->widget( 4 )->setEnabled( true );
}

void MainWidget::slotNextExamination( )
{
    ui->toolBox->setCurrentIndex( 1 );
    ui->toolBox->widget( 1 )->setEnabled( true );
    ui->toolBox->widget( 2 )->setEnabled( false );
    ui->toolBox->widget( 3 )->setEnabled( false );
    ui->toolBox->widget( 4 )->setEnabled( false );

    ui->toolBox->setItemText( 1, "New Examination" );
    ui->toolBox->setItemText( 2, "New Proband" );
    ui->lineEditSICVal->setText( "" );

    ui->pushButtonODWithoutTrialFrame->setStyleSheet( "QPushButton { color : black; }" );
    ui->pushButtonOSWithoutTrialFrame->setStyleSheet( "QPushButton { color : black; }" );

    ui->pushButtonODWithTrialFrame->setStyleSheet( "QPushButton { color : black; }" );
    ui->pushButtonOSWithTrialFrame->setStyleSheet( "QPushButton { color : black; }" );

    ui->pushButtonODWithHoleAperture->setStyleSheet( "QPushButton { color : black; }" );
    ui->pushButtonOSWithHoleAperture->setStyleSheet( "QPushButton { color : black; }" );

    ui->labelODWithoutTrialFrameDVAVal->setStyleSheet( "QLabel { color : black; }" );
    ui->labelODWithoutTrialFrameDVAVal->setText( "noch kein Wert gemessen" );
    ui->labelODWithoutTrialFrameLogMARVal->setStyleSheet( "QLabel { color : black; }" );
    ui->labelODWithoutTrialFrameLogMARVal->setText( "noch kein Wert gemessen" );

    ui->labelOSWithoutTrialFrameDVAVal->setStyleSheet( "QLabel { color : black; }" );
    ui->labelOSWithoutTrialFrameDVAVal->setText( "noch kein Wert gemessen" );
    ui->labelOSWithoutTrialFrameLogMARVal->setStyleSheet( "QLabel { color : black; }" );
    ui->labelOSWithoutTrialFrameLogMARVal->setText( "noch kein Wert gemessen" );

    ui->labelODWithTrialFrameDVAVal->setStyleSheet( "QLabel { color : black; }" );
    ui->labelODWithTrialFrameDVAVal->setText( "noch kein Wert gemessen" );
    ui->labelODWithTrialFrameLogMARVal->setStyleSheet( "QLabel { color : black; }" );
    ui->labelODWithTrialFrameLogMARVal->setText( "noch kein Wert gemessen" );

    ui->labelOSWithTrialFrameDVAVal->setStyleSheet( "QLabel { color : black; }" );
    ui->labelOSWithTrialFrameDVAVal->setText( "noch kein Wert gemessen" );
    ui->labelOSWithTrialFrameLogMARVal->setStyleSheet( "QLabel { color : black; }" );
    ui->labelOSWithTrialFrameLogMARVal->setText( "noch kein Wert gemessen" );

    ui->labelODWithHoleApertureDVAVal->setStyleSheet( "QLabel { color : black; }" );
    ui->labelODWithHoleApertureDVAVal->setText( "keine Messung nötig" );
    ui->labelODWithHoleApertureLogMARVal->setStyleSheet( "QLabel { color : black; }" );
    ui->labelODWithHoleApertureLogMARVal->setText( "keine Messung nötig" );

    ui->labelOSWithHoleApertureDVAVal->setStyleSheet( "QLabel { color : black; }" );
    ui->labelOSWithHoleApertureDVAVal->setText( "keine Messung nötig" );
    ui->labelOSWithHoleApertureLogMARVal->setStyleSheet( "QLabel { color : black; }" );
    ui->labelOSWithHoleApertureLogMARVal->setText( "keine Messung nötig" );

    ui->pushButtonODWithHoleAperture->setStyleSheet( "QPushButton { color : black; }" );
    ui->pushButtonOSWithHoleAperture->setStyleSheet( "QPushButton { color : black; }" );

    ui->plainTextEditSummary->clear( );
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

    //arg << "http://www.michaelbach.de/fract/versions/FrACT3.10.0d.swf";
    //arg << "C:/Users/Thomas Peschel/Documents/dev/c++/life/Franziska/github.com/TPeschel/VisusTestWrapperQt/release/flash/FrACT3.10.0d.swf";
    arg << fractSWF;
    //proc->setWorkingDirectory ( "C:/Users/Thomas Peschel/Documents/dev/c++/life/Franziska/github.com/TPeschel/VisusWarapperQt/release/" );
    proc->setWorkingDirectory ( dataDir );

    proc->start( flashPlayerExe, arg ); //windows
//    proc->start( "flash/start.bat" ); //windows
//    proc->startDetached ( "flashplayer", arg ); //linux
    }


void MainWidget::slotFrACTFinished( int exitCode )
{

    if( exitCode != 0 )

        return;

    QString
    txt = QApplication::clipboard( )->text( QClipboard::Clipboard );

    CSV
    csv;

    csv.loadFromString( txt, '\t', false, 6 );

    if( 0 < csv.size( ) && csv[ 0 ][ 0 ] == '2' ) {

        QStringList
        date,
        time;

        date << csv[ 0 ].split( '-' );
        time << csv[ 1 ].split( ':' );

        QString
        dt = date.join( "" ) + "-" + time.join( "" );

        QStringList
        msl = modeStrings[ currMode ].split( "_PLACEHOLDER_" );

        QFile
        fileResult( dataDir + "/" + currSIC + "-" + dt + "-Acuity_LandoltC-" + msl[ 0 ] + "-" + msl[ 1 ] + "result.csv" );

        if( fileResult.open( QIODevice::WriteOnly | QIODevice::Text ) ) {

            QTextStream
            ts( &fileResult );

            ts <<
                "SIC" << "\t" << "DATE" << "\t" << "TIME" << "\t" << "RESULT" << "\t" <<
                "RESULT_UNIT" << "\t" << "OCULUS" << "\t" << "DETAILS" << "\t" << "TEST_NAME" <<
                "\t" << "DISTANCE" << "\t" << "DISTANCE_UNIT" << "\t" << "nTrials" << "\n" <<
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
                csv[ 7 ] << "\n";

            fileResult.close( );
        }

        if( 12 < csv.size( ) ) {

            QFile
            fileData( dataDir + "/" + currSIC + "-" + dt + "-Acuity_LandoltC-" + msl[ 0 ] + "-" + msl[ 1 ] + "data.csv" );

            if( fileData.open( QIODevice::WriteOnly | QIODevice::Text ) ) {

                QTextStream
                ts( &fileData );

                if( ts.atEnd( ) ) {
                    ts <<
                          "TRIAL" << "\t" << "VALUE" << "\t" <<
                          "ORIENTATION_ANGLE_PRESENTED[DEG]" << "\t" << "ORIENTATION_ANGLE_RESPONDED[DEG]" <<
                          "\t" << "CORRECTNESS[1YES0NO]" << "\t" << "REACTION_TIME[MS]" << "\n";
                }

                for( int i = 13; i < csv.size( ); ++ i ) {

                    ts << csv[ i ];

                    ts << ( ( i - 13 ) % 6 < 5 ? "\t" : "\n" );
                }

                fileData.close( );
            }
        }

        if( 0 == csv_output.size( ) ) {

            csv_output <<
                "SIC" << "\t" << "DATE" << "\t" << "TIME" << "\t" << "RESULT" << "\t" <<
                "RESULT_UNIT" << "\t" << "OCULUS" << "\t" << "DETAILS" << "\t" << "TEST_NAME" <<
                "\t" << "DISTANCE" << "\t" << "DISTANCE_UNIT" << "\t" << "nTrials" << "\n";
        }

        csv_output <<
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
            csv[ 7 ] << "\n";

        ui->plainTextEditSummary->appendPlainText( csv.items( ).join( "  " ) );

        switch ( currMode ) {

            case ODWithoutTrialFrame : {

                ui->pushButtonODWithoutTrialFrame->setStyleSheet( "QPushButton { color : green; }" );
                ui->labelODWithoutTrialFrameDVAVal->setStyleSheet( "QLabel { color : green; }" );
                ui->labelODWithoutTrialFrameDVAVal->setText( csv[ 2 ] );
                ui->labelODWithoutTrialFrameLogMARVal->setStyleSheet( "QLabel { color : green; }" );
                ui->labelODWithoutTrialFrameLogMARVal->setText( QString( "%1" ).arg( -log10( csv[ 2 ].toDouble( ) ) ) );


                if( ui->labelODWithTrialFrameDVAVal->text( )[ 0 ] != 'n' ) {

                    double
                    oDWithTF    = ui->labelODWithTrialFrameDVAVal->text( ).toDouble( ),
                    oDWithoutTF = ui->labelODWithoutTrialFrameDVAVal->text( ).toDouble( );

                    if( oDWithoutTF > oDWithTF ) {

                        ui->pushButtonODWithHoleAperture->setEnabled( true );
                        ui->labelODWithHoleApertureDVAVal->setStyleSheet( "QLabel { color : red; }" );
                        ui->labelODWithHoleApertureDVAVal->setText( "Die Messung ohne Messbrille ist besser als die ohne." );
                        ui->labelODWithHoleApertureLogMARVal->setStyleSheet( "QLabel { color : red; }" );
                        ui->labelODWithHoleApertureLogMARVal->setText( "Bitte messen Sie nun mit Messblende!" );
                    }
                }

                break;
            }

            case OSWithoutTrialFrame : {

                ui->pushButtonOSWithoutTrialFrame->setStyleSheet( "QPushButton { color : green; }" );
                ui->labelOSWithoutTrialFrameDVAVal->setStyleSheet( "QLabel { color : green; }" );
                ui->labelOSWithoutTrialFrameDVAVal->setText( csv[ 2 ] );
                ui->labelOSWithoutTrialFrameLogMARVal->setStyleSheet( "QLabel { color : green; }" );
                ui->labelOSWithoutTrialFrameLogMARVal->setText( QString( "%1" ).arg( -log10( csv[ 2 ].toDouble( ) ) ) );

                if( ui->labelOSWithTrialFrameDVAVal->text( )[ 0 ] != 'n' ) {

                    double
                    oSWithTF    = ui->labelOSWithTrialFrameDVAVal->text( ).toDouble( ),
                    oSWithoutTF = ui->labelOSWithoutTrialFrameDVAVal->text( ).toDouble( );

                    if( oSWithoutTF > oSWithTF ) {

                        ui->pushButtonOSWithHoleAperture->setEnabled( true );
                        ui->labelOSWithHoleApertureDVAVal->setStyleSheet( "QLabel { color : red; }" );
                        ui->labelOSWithHoleApertureDVAVal->setText( "Die Messung ohne Messbrille ist besser als die ohne." );
                        ui->labelOSWithHoleApertureLogMARVal->setStyleSheet( "QLabel { color : red; }" );
                        ui->labelOSWithHoleApertureLogMARVal->setText( "Bitte messen Sie nun mit Messblende!" );
                    }
                }

                break;
            }

            case ODWithTrialFrame : {

                ui->pushButtonODWithTrialFrame->setStyleSheet( "QPushButton { color : green; }" );
                ui->labelODWithTrialFrameDVAVal->setStyleSheet( "QLabel { color : green; }" );
                ui->labelODWithTrialFrameDVAVal->setText( csv[ 2 ] );
                ui->labelODWithTrialFrameLogMARVal->setStyleSheet( "QLabel { color : green; }" );
                ui->labelODWithTrialFrameLogMARVal->setText( QString( "%1" ).arg( -log10( csv[ 2 ].toDouble( ) ) ) );

                if( ui->labelODWithoutTrialFrameDVAVal->text( )[ 0 ] != 's' ) {

                    double
                    oDWithTF    = ui->labelODWithTrialFrameDVAVal->text( ).toDouble( ),
                    oDWithoutTF = ui->labelODWithoutTrialFrameDVAVal->text( ).toDouble( );

                    if( oDWithoutTF > oDWithTF ) {

                        ui->pushButtonODWithHoleAperture->setEnabled( true );
                        ui->labelODWithHoleApertureDVAVal->setStyleSheet( "QLabel { color : red; }" );
                        ui->labelODWithHoleApertureDVAVal->setText( "Die Messung ohne Messbrille ist besser als die ohne." );
                        ui->labelODWithHoleApertureLogMARVal->setStyleSheet( "QLabel { color : red; }" );
                        ui->labelODWithHoleApertureLogMARVal->setText( "Bitte messen Sie nun mit Messblende!" );
                    }
                }

                break;
            }

            case OSWithTrialFrame : {

                ui->pushButtonOSWithTrialFrame->setStyleSheet( "QPushButton { color : green; }" );
                ui->labelOSWithTrialFrameDVAVal->setStyleSheet( "QLabel { color : green; }" );
                ui->labelOSWithTrialFrameDVAVal->setText( csv[ 2 ] );
                ui->labelOSWithTrialFrameLogMARVal->setStyleSheet( "QLabel { color : green; }" );
                ui->labelOSWithTrialFrameLogMARVal->setText( QString( "%1" ).arg( -log10( csv[ 2 ].toDouble( ) ) ) );

                if( ui->labelOSWithoutTrialFrameDVAVal->text( )[ 0 ] != 's' ) {

                    double
                    oSWithTF    = ui->labelOSWithTrialFrameDVAVal->text( ).toDouble( ),
                    oSWithoutTF = ui->labelOSWithoutTrialFrameDVAVal->text( ).toDouble( );

                    if( oSWithoutTF > oSWithTF ) {

                        ui->pushButtonOSWithHoleAperture->setEnabled( true );
                        ui->labelOSWithHoleApertureDVAVal->setStyleSheet( "QLabel { color : red; }" );
                        ui->labelOSWithHoleApertureDVAVal->setText( "Die Messung ohne Messbrille ist besser als die ohne." );
                        ui->labelOSWithHoleApertureLogMARVal->setStyleSheet( "QLabel { color : red; }" );
                        ui->labelOSWithHoleApertureLogMARVal->setText( "Bitte messen Sie nun mit Messblende!" );
                    }
                }

                break;
            }

            case ODWithHoleAperture : {

                ui->pushButtonODWithHoleAperture->setStyleSheet( "QPushButton { color : green; }" );
                ui->labelODWithHoleApertureDVAVal->setStyleSheet( "QLabel { color : green; }" );
                ui->labelODWithHoleApertureDVAVal->setText( csv[ 2 ] );
                ui->labelODWithHoleApertureLogMARVal->setStyleSheet( "QLabel { color : green; }" );
                ui->labelODWithHoleApertureLogMARVal->setText( QString( "%1" ).arg( -log10( csv[ 2 ].toDouble( ) ) ) );

                break;
            }

            case OSWithHoleAperture : {

                ui->pushButtonOSWithHoleAperture->setStyleSheet( "QPushButton { color : green; }" );
                ui->labelOSWithHoleApertureDVAVal->setStyleSheet( "QLabel { color : green; }" );
                ui->labelOSWithHoleApertureDVAVal->setText( csv[ 2 ] );
                ui->labelOSWithHoleApertureLogMARVal->setStyleSheet( "QLabel { color : green; }" );
                ui->labelOSWithHoleApertureLogMARVal->setText( QString( "%1" ).arg( -log10( csv[ 2 ].toDouble( ) ) ) );

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

void MainWidget::slotStartFrACTAcuity_LandoltCODWithTrialFrame( )
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

void MainWidget::slotStartFileDialoagForFlashPlayer()
{
    flashPlayerExe = QFileDialog::getOpenFileName( this, tr( "Flash Player Projector" ), ".", tr("Exe Files (*.exe)" ) );
    ui->lineEditFlashplayerExe->setText( flashPlayerExe );
    csv_config[ 3 ] = flashPlayerExe;
}

void MainWidget::slotStartFileDialoagForFractSWF()
{
    fractSWF = QFileDialog::getOpenFileName( this, tr( "FrACT*.swf" ), ".", tr("SWF Files (*.swf)" ) );
    ui->lineEditFrACTSWF->setText( fractSWF );
    csv_config[ 4 ] = fractSWF;
}

void MainWidget::slotStartFileDialoagForDataDir( )
{
    dataDir = QFileDialog::getExistingDirectory( this, tr( "Daten Verzeichnis" ), tr( "." ), QFileDialog::ShowDirsOnly );
    ui->lineEditDataDir->setText( dataDir );
    csv_config[ 5 ] = dataDir;
}

void MainWidget::closeEvent( QCloseEvent *p_closeEvent )
{

    p_closeEvent->accept( );
//http://doc.qt.io/qt-5/qtxml-streambookmarks-example.html
     csv_output.writeToFile( "data.csv", '\t' );
     csv_config.writeToFile( "consfig.tsv", '\t' );
    //MainWidget::closeEvent(p_closeEvent );
}
