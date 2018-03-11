#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <QWidget>
#include <QDebug>

namespace Ui {
class ProjectManagerWindow;
}

class ProjectManagerWindow: public QWidget
{
    Q_OBJECT

public:
    explicit ProjectManagerWindow(QWidget *parent = 0);
    ~ProjectManagerWindow();

private:
    Ui::ProjectManagerWindow *ui;

    void connects();
};

#endif // PROJECTMANAGER_H
