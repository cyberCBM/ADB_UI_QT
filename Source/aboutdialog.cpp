#include "aboutdialog.hpp"
#include "ui_about.h"

AboutDialog::AboutDialog(QDialog* dialog) :
    QDialog(dialog),
    m_aboutUI(new Ui::About)
{
    m_aboutUI->setupUi(this);
}

AboutDialog::~AboutDialog()
{
    delete m_aboutUI;
}
