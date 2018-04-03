#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP

#include <QWidget>
#include <QProcess>
#include <QClipboard>

namespace Ui {
	class MainWidget;
}

class MainWidget : public QWidget
{
		Q_OBJECT

	public:
		explicit MainWidget(QWidget *parent = 0);
		~MainWidget();

    QClipboard
    *cb;

    QProcess
    *proc;

	private:
		Ui::MainWidget *ui;

	public slots:

		void slotNewExamination( );
        void slotScanSIC( );
        void slotFinishExamination( );
        void slotStartFrACT();
        void slotFrACTFinished(QClipboard::Mode m);
};

#endif // MAINWIDGET_HPP
