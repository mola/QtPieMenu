#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMouseEvent>
#include "piemenu/QtPieMenu"
#include "piemenu/sectormenu.h"

MainWindow::MainWindow(QWidget *parent):
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  root = new SectorMenu("root", this);

  SectorMenu *child1 = new SectorMenu("Hammer", root);
  root->insertItem(child1);
// root->setItemIcon(tr("Hammer"), 0);

  SectorMenu *child11 = new SectorMenu("Merge", child1);
  child1->insertItem(child11);
  child11->insertItem(tr("C"), this, SLOT(ignoreAction()));
  child11->insertItem(tr("B"), this, SLOT(ignoreAction()));
  child11->insertItem(tr("A"), this, SLOT(ignoreAction()));

  SectorMenu *child12 = new SectorMenu("Split", child1);
  child1->insertItem(child12);
  child12->insertItem(tr("A"), this, SLOT(ignoreAction()));
  child12->insertItem(tr("B"), this, SLOT(ignoreAction()));
  child12->insertItem(tr("C"), this, SLOT(ignoreAction()));

  root->insertItem("Left", this, SLOT(ignoreAction()));


  SectorMenu *child13 = new SectorMenu("Down", root);
  child13->insertItem(tr("Q1"), this, SLOT(ignoreAction()));
  child13->insertItem(tr("Q2"), this, SLOT(ignoreAction()));
  child13->insertItem(tr("Q3"), this, SLOT(ignoreAction()));
  root->insertItem(child13);

  root->insertItem("Right", this, SLOT(ignoreAction()));
}

MainWindow::~MainWindow()
{
  delete ui;
}

void  MainWindow::ignoreAction()
{
}

void  MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
  root->popup(event->globalPos());
}
