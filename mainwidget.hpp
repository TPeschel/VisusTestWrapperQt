#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP

#include <QWidget>
#include <QFileDialog>
#include <QProcess>
#include <QClipboard>
#include <QCloseEvent>
#include <QString>
#include <QFile>
#include <QTextStream>
#include "csv.hpp"

namespace Ui {

    class MainWidget;
}

class MainWidget : public QWidget {

        Q_OBJECT

	public:

        explicit
        MainWidget(QWidget *parent = 0);

        ~MainWidget();

    public:

        QProcess
        *proc;

	private:

        Ui::MainWidget
        *ui;

    public:

        QString
        flashPlayerExe,
        fractSWF,
        dataDir,
        currSIC;

        enum Acuity_LandoltCModes {

            ODWithoutTrialFrame,
            OSWithoutTrialFrame,
            ODWithTrialFrame,
            OSWithTrialFrame,
            ODWithHoleAperture,
            OSWithHoleAperture
        }
        currMode;

    public slots:

		void slotNewExamination( );
        void slotScanSIC( );
        void slotFinishExamination( );
        void slotNextExamination( );
        void slotFrACTFinished( int exitCode );
        void slotStartFrACTAcuity_LandoltCODWithoutTrialFrame( );
        void slotStartFrACTAcuity_LandoltCOSWithoutTrialFrame( );
        void slotStartFrACTAcuity_LandoltCODWithTrialFrame( );
        void slotStartFrACTAcuity_LandoltCOSWithTrialFrame( );
        void slotStartFrACTAcuity_LandoltCODWithHoleAperture( );
        void slotStartFrACTAcuity_LandoltCOSWithHoleAperture( );
        void slotStartFileDialoagForFlashPlayer( );
        void slotStartFileDialoagForFractSWF( );
        void slotStartFileDialoagForDataDir( );


    public:

        void closeEvent( QCloseEvent *p_closeEvent );

    public:

        CSV
        csv_config,
        csv_output;

        void startFrACT( );
};

#endif // MAINWIDGET_HPP
