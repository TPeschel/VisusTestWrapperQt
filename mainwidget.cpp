#include "mainwidget.hpp"
#include "ui_mainwidget.h"
#include <QString>
#include <QTime>
#include <iostream>
#include "csv.hpp"

MainWidget::MainWidget(QWidget *parent) :
	QWidget(parent),
    ui(new Ui::MainWidget)
{
	ui->setupUi(this);

    proc = 0;

	QObject::connect( ui->pushButtonNewExamination, SIGNAL(	released( ) ), this, SLOT(slotNewExamination( ) ) );
    QObject::connect( ui->lineEditSICVal, SIGNAL( textChanged( QString ) ), this, SLOT( slotScanSIC( ) ) );
    QObject::connect( ui->pushButtonFinishExamination, SIGNAL( released( ) ), this, SLOT( slotFinishExamination( ) ) );
    QObject::connect( ui->pushButtonODWithoutTrialFrame, SIGNAL( released( ) ), this, SLOT( slotStartFrACT( ) ) );
    QObject::connect( ui->pushButtonOSWithoutTrialFrame, SIGNAL( released( ) ), this, SLOT( slotStartFrACT( ) ) );
    QObject::connect( ui->pushButtonODWithTrialFrame, SIGNAL( released( ) ), this, SLOT( slotStartFrACT( ) ) );
    QObject::connect( ui->pushButtonOSWithTrialFrame, SIGNAL( released( ) ), this, SLOT( slotStartFrACT( ) ) );
    QObject::connect( ui->pushButtonODWithHoleAperture, SIGNAL( released( ) ), this, SLOT( slotStartFrACT( ) ) );
    QObject::connect( ui->pushButtonOSWithHoleAperture, SIGNAL( released( ) ), this, SLOT( slotStartFrACT( ) ) );
    QObject::connect( ui->pushButtonClose, SIGNAL( released( ) ), this, SLOT( close( ) ) );

    csv_output.name( "daten.csv" );
    csv_output.sep( "[\n\t\r ]+" );
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
    ui->lineEditSICVal->setFocus();
    ui->lineEditSICVal->setText( "" );
}

void MainWidget::slotScanSIC( )
{
    QString scannedSIC = ui->lineEditSICVal->text( );
    QRegularExpression re( "^LI\\d{8}", QRegularExpression::CaseInsensitiveOption );
    QRegularExpressionMatch m = re.match( scannedSIC );

    if( m.hasMatch( ) ) {

        ui->toolBox->setCurrentIndex( 2 );
        ui->toolBox->setItemText( 1, m.captured( 0 ) );
    }
}

void MainWidget::slotFinishExamination( )
{
    ui->toolBox->setCurrentIndex( 3 );
}

void MainWidget::slotStartFrACT(  )
{
    if( proc )

        delete proc;

    proc = new QProcess( );

    QObject::connect( proc, SIGNAL(finished(int)),this,SLOT( slotFrACTFinished(int)));

    QStringList arg;

    arg << "FrACT3.9.8.swf";

    proc->setWorkingDirectory ( "C:/Users/Thomas Peschel/Documents/dev/c++/life/Franziska/github.com/TPeschel/build-VisusTestWrapper-Desktop_Qt_5_10_1_MinGW_32bit-Debug" );

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

    csv.sep( "[\t\n\r ]+" );
    csv.loadFromString( txt );

    if( 0 < csv.size( ) && csv[ 0 ][ 0 ] == '2' ) {

        ui->plainTextEdit->appendPlainText( txt );

        csv_output.append( csv );
    }
/*
    cb->blockSignals( true );
    cb->clear( );
    cb->blockSignals( false );
*/
    //proc->blockSignals( false );
}

void MainWidget::closeEvent(QCloseEvent *p_closeEvent)
{

    p_closeEvent->accept( );
http://doc.qt.io/qt-5/qtxml-streambookmarks-example.html
     csv_output.writeToFile( "data.csv" );
    //MainWidget::closeEvent(p_closeEvent );
}


