#ifndef CQToolButton_H
#define CQToolButton_H

#include <QToolButton>
#include <QStyle>

class CQToolButton : public QToolButton {
  Q_OBJECT

 public:
  CQToolButton(QWidget *parent=nullptr);

 private:
  void mouseMoveEvent(QMouseEvent *e) override;

  void leaveEvent(QEvent *) override;

  void paintEvent(QPaintEvent *e) override;

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
