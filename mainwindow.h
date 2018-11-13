#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class SectorMenu;
namespace Ui
{
class MainWindow;
}

class MainWindow: public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);

  ~MainWindow();

private slots:
  void  ignoreAction();

private:
  Ui::MainWindow *ui;
  SectorMenu     *root;

  // QWidget interface

protected:
  void  contextMenuEvent(QContextMenuEvent *event);
};

#endif // MAINWINDOW_H
