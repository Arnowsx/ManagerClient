#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <QWidget>

namespace Ui {
class ProjectManager;
}

class ProjectManager : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectManager(QWidget *parent = 0);
    ~ProjectManager();

private:
    Ui::ProjectManager *ui;
};

#endif // PROJECTMANAGER_H
