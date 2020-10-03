#include "aboutdialog.hpp"
#include "ui_about.h"

AboutDialog::AboutDialog(QDialog* dialog) :
    QDialog(dialog),
    m_About(new Ui::About)
{
    m_About->setupUi(this);
}

AboutDialog::~AboutDialog()
{
    delete m_About;
}
