#include "chat_login_dialog.h"

#include <QBoxLayout>

chat_login_dialog::chat_login_dialog(QString* nickname, QWidget* parent) : QDialog(parent)
{
	setWindowTitle("Nickname");
	m_nick_edit = new QLineEdit(this);
	m_ok_button = new QPushButton("Use", this);

	QVBoxLayout* vbox = new QVBoxLayout();
	vbox->addWidget(m_nick_edit);
	vbox->addWidget(m_ok_button);
	setLayout(vbox);

	auto ok_lambda = [=]()
	{
		*nickname = m_nick_edit->text();
		close();
	};

	connect(m_ok_button, &QAbstractButton::clicked, ok_lambda);
	connect(m_nick_edit, &QLineEdit::returnPressed, ok_lambda);
}

chat_login_dialog::~chat_login_dialog()
{
}
