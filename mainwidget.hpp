#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP

#include <QWidget>
#include <QProcess>
#include <QClipboard>
#include <QCloseEvent>
#include "csv.hpp"

namespace Ui {
	class MainWidget;
}

class MainWidget : public QWidget
{
		Q_OBJECT

	public:
		explicit MainWidget(QWidget *parent = 0);
		~MainWidget();

    QProcess
    *proc;

	private:
		Ui::MainWidget *ui;

	public slots:

		void slotNewExamination( );
        void slotScanSIC( );
        void slotFinishExamination( );
        void slotStartFrACT( );
        void slotFrACTFinished( int exitCode );

    public:

        void closeEvent( QCloseEvent *p_closeEvent );

    public:

        CSV
        csv_output;
};

#endif // MAINWIDGET_HPP
