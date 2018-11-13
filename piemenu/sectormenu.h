#ifndef SECTORMENU_H
#define SECTORMENU_H

#include "qtpiemenu.h"

class SectorMenu: public QtPieMenu
{
  Q_OBJECT

public:
  SectorMenu(const QString &title, QWidget *parent);

  int   indexAt(const QPoint &pos);

protected:
  void  generateMask(QBitmap *mask);

  void  reposition();

  void  paintEvent(QPaintEvent *event);

  int   menuDepth() const;

private:
  int              startAngle;
  int              arcLength;
  QRadialGradient *gradientCenter;
  QRadialGradient *gradientBack;
  QRadialGradient *gradientLight;
};

#endif // SECTORMENU_H
