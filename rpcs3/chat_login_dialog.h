#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>

class chat_login_dialog : public QDialog
{
	Q_OBJECT

public:
	explicit chat_login_dialog(QString* nickname, QWidget* parent = nullptr);
	~chat_login_dialog();

private:
	QLineEdit* m_nick_edit;
	QPushButton* m_ok_button;
};