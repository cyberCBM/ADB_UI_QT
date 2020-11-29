#pragma once

#include <QDialog>

namespace Ui {
class About;
}

class AboutDialog : public QDialog
{
public:
    AboutDialog(QDialog* dialog);
    ~AboutDialog();

private:
    Ui::About *m_aboutUI;
};

