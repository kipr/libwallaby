#ifndef _AREYOUSUREDIALOG_H_
#define _AREYOUSUREDIALOG_H_

#include <QDialog>

namespace Ui
{
	class AreYouSureDialog;
}

class AreYouSureDialog : public QDialog
{
Q_OBJECT
public:
	AreYouSureDialog(QWidget *parent = 0);
	~AreYouSureDialog();
	
	Q_PROPERTY(QString description READ description WRITE setDescription)
	void setDescription(const QString &description);
	QString description() const;
	
private:
	Ui::AreYouSureDialog *ui;
};

#endif
