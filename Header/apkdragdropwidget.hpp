#ifndef APKDRAGDROPWIDGET_H
#define APKDRAGDROPWIDGET_H

#include <QWidget>

namespace Ui {
class apkDragDropWidget;
}

class QDropEvent;
class QDragEnterEvent;
class QDragLeaveEvent;

class apkDragDropWidget : public QWidget
{
    Q_OBJECT

public:
    explicit apkDragDropWidget(QWidget *parent = nullptr);
    ~apkDragDropWidget();

private:
    Ui::apkDragDropWidget *m_UI;
    void installAPKFile(const QString& filename);

protected:
void dropEvent(QDropEvent *event) override;
void dragEnterEvent(QDragEnterEvent *event) override;
void dragLeaveEvent (QDragLeaveEvent *event) override;

};

#endif // APKDRAGDROPWIDGET_H
