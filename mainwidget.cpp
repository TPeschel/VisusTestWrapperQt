#include "mainwidget.hpp"
#include "ui_mainwidget.h"
#include <QString>
#include <iostream>


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

    cb = QApplication::clipboard( );

    QObject::connect( cb, SIGNAL( changed( QClipboard::Mode ) ), this, SLOT( slotFrACTFinished( QClipboard::Mode ) ) );
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

void MainWidget::slotScanSIC()
{
    QString scannedSIC = ui->lineEditSICVal->text( );
    QRegularExpression re( "LI\\d{8}", QRegularExpression::CaseInsensitiveOption );
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

    QStringList arg;

    arg << "FrACT3.9.8.swf";

    proc->setWorkingDirectory ( "." );
//    proc->startDetached ( "flashplayer_29_sa.exe", arg ); //windows
    proc->startDetached ( "./flashplayer", arg ); //linux
//    fract->startDetached ( "FrACT3.10.0b.exe", arg );
//    proc->waitForFinished ( );

//    QClipboard *cb = QApplication::clipboard( );

//    ui->plainTextEdit->appendPlainText( cb->text( QClipboard::Clipboard ) );

//    delete fract;
}

void MainWidget::slotFrACTFinished( QClipboard::Mode m )
{
    //proc->blockSignals( true );

    cb->blockSignals( true );

    QString
    txt = cb->text( m );

    std::cout << txt.toStdString( ) << std::endl;

    ui->plainTextEdit->blockSignals( true );
    ui->plainTextEdit->appendPlainText( txt );
    ui->plainTextEdit->blockSignals( false );

    //cb->clear( );

    cb->blockSignals( false );
    proc->blockSignals( false );
}


