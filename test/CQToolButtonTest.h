#include <QWidget>

class QAbstractButton;
class CQToolButton;
class QMenu;

class CQToolButtonTest : public QWidget {
  Q_OBJECT

 public:
  CQToolButtonTest(QWidget *parent=0);

 private slots:
  void updateStyle();
  void updateMenu();

 private:
  QAbstractButton *iconBox_;
  QAbstractButton *textBox_;
  QAbstractButton *menu1Box_;
  QAbstractButton *menu2Box_;
  QAbstractButton *menu3Box_;
  QAbstractButton *menu4Box_;
  QAbstractButton *arrowBox_;
  CQToolButton    *button_;
  QMenu           *menu_;
};
