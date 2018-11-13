#include "sectormenu.h"
#include <QBitmap>
#include <QPainter>
#include <math.h>
#include <iostream>

SectorMenu::SectorMenu(const QString &title,
                       QWidget       *parent):
  QtPieMenu(title, parent)
{
  int  depth = menuDepth();

  setInnerRadius(depth * 65);
  setOuterRadius((depth + 1) * 65);
  setFixedSize(2 * outerRadius() + 1,
               2 * outerRadius() + 1);


  gradientBack = new QRadialGradient(rect().center().x(), rect().center().y() - 10, rect().width() / 2.0); // diagonal gradient from top-left to bottom-right
  gradientBack->setColorAt(0, QColor(174, 184, 191));
  gradientBack->setColorAt(0.25, QColor(230, 230, 230));
  gradientBack->setColorAt(0.90, QColor(239, 238, 239));
  gradientBack->setColorAt(1, QColor(220, 220, 220));

  gradientLight = new QRadialGradient(rect().center().x(), rect().center().y() - 10, rect().width() / 2.0); // diagonal gradient from top-left to bottom-right
  gradientLight->setColorAt(0, QColor(253, 99, 70));
  gradientLight->setColorAt(1, QColor(253, 184, 92));
}

int  SectorMenu::menuDepth() const
{
  const QObject *pie   = this;
  int            depth = 0;

  while (pie->parent()
         && pie->parent()->inherits("SectorMenu"))
  {
    pie = pie->parent();
    ++depth;
  }

  return depth;
}

void  SectorMenu::generateMask(QBitmap *mask)
{
  if (menuDepth() == 0)
  {
    startAngle = 45 * 16;
    arcLength  = 360 * 16;
  }
  else
  {
    SectorMenu *parentPie = (SectorMenu *)parent();

    for (int i = 0; i < parentPie->count(); ++i)
    {
      if (parentPie->subMenuAt(i) == this)
      {
        arcLength  = parentPie->arcLength / parentPie->count();
        startAngle = (45 * 16) + (arcLength * i);
        break;
      }
    }
  }

  QPainter  painter(mask);
  painter.setRenderHint(QPainter::Antialiasing, true);

  painter.setPen(QPen(Qt::color1, 2));
  painter.setBrush(Qt::color1);
  painter.drawPie(0, 0,
                  outerRadius() * 2, outerRadius() * 2,
                  startAngle, arcLength);

  if (innerRadius() > 0)
  {
// painter.fillRect(rect(), Qt::color0);
    QPoint  center = rect().center();
    painter.setPen(Qt::color0);
    painter.setBrush(Qt::color0);
    painter.drawPie(center.x() - innerRadius(),
                    center.y() - innerRadius(),
                    innerRadius() * 2,
                    innerRadius() * 2,
                    0, 360 * 16);
  }
}

int  SectorMenu::indexAt(const QPoint &pos)
{
  if (count() == 0)
  {
    return -1;
  }

  int  sliceSize = arcLength / count();

  if (sliceSize == 0)
  {
    return -1;
  }

  int  angle = (int)((angleAt(pos) * 360.0 * 16.0) / (2.0 * M_PI));

  if (angle - startAngle < 0)
  {
    angle += 16 * 360;
  }

  angle -= startAngle;

  int  sector = angle / sliceSize;

  if ((sector < 0) || (sector >= count()))
  {
    return -1;
  }

  return sector;
}

void  SectorMenu::reposition()
{
  if (menuDepth() > 0)
  {
    SectorMenu *parentPie = (SectorMenu *)parent();
    QPoint      center    = parentPie->geometry().center();
    move(center.x() - outerRadius(),
         center.y() - outerRadius());
  }
}

void  SectorMenu::paintEvent(QPaintEvent *)
{
  QPainter  painter(this);

  painter.setRenderHint(QPainter::Antialiasing, true);

  QFontMetrics  metrics(font());
  QPoint        center    = rect().center();
  int           sliceSize = arcLength / count();

  for (int i = 0; i < count(); ++i)
  {
    if (i == highlightedItem())
    {
      painter.setPen(palette().highlightedText().color());
      painter.setBrush(*gradientLight);
    }
    else
    {
      painter.setPen(palette().foreground().color());
      painter.setBrush(*gradientBack);
    }

    painter.drawPie(0, 0,
                    outerRadius() * 2,
                    outerRadius() * 2,
                    startAngle + sliceSize * i,
                    sliceSize);

// painter.setPen(palette().foreground().color());
// painter.setBrush(Qt::darkGray);

// painter.drawPie(0, 0,
// outerRadius() * 2,
// outerRadius() * 2,
// startAngle + sliceSize * i,
// sliceSize);

    double  rad = (innerRadius() + outerRadius())
                  / 2.0;
    double  slice = sliceSize * M_PI / (360.0 * 8.0);
    double  angle = startAngle * M_PI / (360.0 * 8.0);
    angle += slice * (i + 0.5);

    int  x = (int)(cos(angle) * rad);
    int  y = (int)(-sin(angle) * rad);

    painter.drawText(center.x() + x
                     - metrics.width(itemText(i)) / 2,
                     center.y() + y, itemText(i));
  }

  painter.setPen(palette().foreground().color());
  painter.setBrush(Qt::darkGray);
  painter.drawEllipse(center, 15, 15);

  painter.setPen(palette().foreground().color());
  painter.setBrush(*gradientBack);
  painter.drawEllipse(center, 10, 10);
}
