#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP

#include <QWidget>
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
        currSIC;

        enum Modes {

            Acuity_LandoltCODWithoutTrialFrame,
            Acuity_LandoltCOSWithoutTrialFrame,
            Acuity_LandoltCODWithTrialFrame,
            Acuity_LandoltCOSWithTrialFrame,
            Acuity_LandoltCODWithHoleAperture,
            Acuity_LandoltCOSWithHoleAperture
        }
        currMode;

    public slots:

		void slotNewExamination( );
        void slotScanSIC( );
        void slotFinishExamination( );
        void slotFrACTFinished( int exitCode );
        void slotStartFrACTAcuity_LandoltCODWithoutTrialFrame( );
        void slotStartFrACTAcuity_LandoltCOSWithoutTrialFrame( );
        void slotStartFrACTAcuity_LandoltCODWithTrialFrame( );
        void slotStartFrACTAcuity_LandoltCOSWithTrialFrame( );
        void slotStartFrACTAcuity_LandoltCODWithHoleAperture( );
        void slotStartFrACTAcuity_LandoltCOSWithHoleAperture( );

    public:

        void closeEvent( QCloseEvent *p_closeEvent );

    public:

        CSV
        csv_output;

        void startFrACT( );
};

#endif // MAINWIDGET_HPP
