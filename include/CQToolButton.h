#ifndef CQToolButton_H
#define CQToolButton_H

#include <QToolButton>
#include <QStyle>

class CQToolButton : public QToolButton {
  Q_OBJECT

 public:
  CQToolButton(QWidget *parent=0);

 private:
  void mouseMoveEvent(QMouseEvent *e);

  void leaveEvent(QEvent *);

  void paintEvent(QPaintEvent *e);

  void drawPopupButton(QStyleOptionToolButton &opt);
  void drawMenuButton(QStyleOptionToolButton &opt);
  void drawNormalButton(QStyleOptionToolButton &opt);

 public slots:
  void setAutoRaise(bool raise);
  void setCheckable(bool check);
  void setDebug(bool debug);
  void setDrawCustom(bool draw);

 private:
  bool               drawCustom_;
  bool               debug_;
  QStyle::SubControl mouseControls_;
};

#endif
