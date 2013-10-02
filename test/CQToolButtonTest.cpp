#include <CQToolButtonTest.h>
#include <CQToolButton.h>
#include <CQCheckGrid.h>
#include <QApplication>

#include <QVBoxLayout>
#include <QCheckBox>
#include <QMenu>

#include <icon.xpm>
#include <icon_on.xpm>
#include <icon_off.xpm>

int
main(int argc, char **argv)
{
  QApplication app(argc, argv);

  CQToolButtonTest *test = new CQToolButtonTest;

  test->show();

  return app.exec();
}

CQToolButtonTest::
CQToolButtonTest(QWidget *parent) :
 QWidget(parent)
{
  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setMargin(0); layout->setSpacing(0);

  //---

  QIcon icon;

  icon.addPixmap(QPixmap(icon_on_data ), QIcon::Normal, QIcon::On);
  icon.addPixmap(QPixmap(icon_off_data), QIcon::Normal, QIcon::Off);

  button_ = new CQToolButton;

  button_->setText("Button");
  button_->setIcon(icon);
  button_->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  //button_->setPopupMode(QToolButton::MenuButtonPopup);
  //button_->setPopupMode(QToolButton::InstantPopup);
  button_->setPopupMode(QToolButton::DelayedPopup);
  button_->setAutoRaise(true);

  //---

  menu_ = new QMenu;

  menu_->addAction("Action");

  button_->setMenu(menu_);

  //---

  CQCheckGrid *grid = new CQCheckGrid(0, 4);

  QAbstractButton *customBox = grid->addCheck ("Custom");
  QAbstractButton *debugBox  = grid->addCheck ("Debug");

  customBox->setChecked(true);

  grid->addSpacer();
  grid->addSpacer();

  connect(customBox, SIGNAL(clicked(bool)), button_, SLOT(setDrawCustom(bool)));
  connect(debugBox , SIGNAL(clicked(bool)), button_, SLOT(setDebug(bool)));

  //---

  QAbstractButton *enableBox  = grid->addCheck("Enabled");
  QAbstractButton *autoBox    = grid->addCheck("Auto Raise");
  QAbstractButton *checkBox   = grid->addCheck("Checkable");
  QAbstractButton *checkedBox = grid->addCheck("Checked");

  enableBox->setChecked(true);
  autoBox  ->setChecked(true);

  connect(enableBox , SIGNAL(clicked(bool)), button_, SLOT(setEnabled(bool)));
  connect(autoBox   , SIGNAL(clicked(bool)), button_, SLOT(setAutoRaise(bool)));
  connect(checkBox  , SIGNAL(clicked(bool)), button_, SLOT(setCheckable(bool)));
  connect(checkedBox, SIGNAL(clicked(bool)), button_, SLOT(setChecked(bool)));

  //---

  CQCheckGrid *graphicGrid = grid->createGroup("Graphic");

  graphicGrid->setExclusive(true);

  iconBox_  = graphicGrid->addCheck("Icon");
  arrowBox_ = graphicGrid->addCheck("Arrow");

  iconBox_->setChecked(true);

  grid->addGroup(graphicGrid);

  connect(iconBox_ , SIGNAL(clicked(bool)), this, SLOT(updateStyle()));
  connect(arrowBox_, SIGNAL(clicked(bool)), this, SLOT(updateStyle()));

  //---

  textBox_ = grid->addCheck("Text");

  grid->addSpacer();

  textBox_->setChecked(true);

  connect(textBox_ , SIGNAL(clicked(bool)), this, SLOT(updateStyle()));

  //---

  CQCheckGrid *menuGrid = grid->createGroup("Menu");

  menuGrid->setExclusive();

  menu1Box_ = menuGrid->addCheck("Popup");
  menu2Box_ = menuGrid->addCheck("Instant");
  menu3Box_ = menuGrid->addCheck("Delayed");
  menu4Box_ = menuGrid->addCheck("None");

  menu3Box_->setChecked(true);

  grid->addGroup(menuGrid);

  connect(menu1Box_ , SIGNAL(clicked(bool)), this, SLOT(updateMenu()));
  connect(menu2Box_ , SIGNAL(clicked(bool)), this, SLOT(updateMenu()));
  connect(menu3Box_ , SIGNAL(clicked(bool)), this, SLOT(updateMenu()));
  connect(menu4Box_ , SIGNAL(clicked(bool)), this, SLOT(updateMenu()));

  layout->addWidget(grid);

  //---

  QHBoxLayout *buttonLayout = new QHBoxLayout;

  buttonLayout->addWidget(button_);

  layout->addLayout(buttonLayout);
  layout->addStretch(1);
}

void
CQToolButtonTest::
updateStyle()
{
  button_->setArrowType(arrowBox_->isChecked() ? Qt::UpArrow : Qt::NoArrow);

  Qt::ToolButtonStyle style = Qt::ToolButtonFollowStyle;

  if      ((iconBox_->isChecked() || arrowBox_->isChecked()) && textBox_->isChecked())
    style = Qt::ToolButtonTextBesideIcon;
  else if (iconBox_->isChecked() || arrowBox_->isChecked())
    style = Qt::ToolButtonIconOnly;
  else if (textBox_->isChecked())
    style = Qt::ToolButtonTextOnly;

  button_->setToolButtonStyle(style);
}

void
CQToolButtonTest::
updateMenu()
{
  bool menu = true;

  if      (menu1Box_->isChecked())
    button_->setPopupMode(QToolButton::MenuButtonPopup);
  else if (menu2Box_->isChecked())
    button_->setPopupMode(QToolButton::InstantPopup);
  else if (menu3Box_->isChecked())
    button_->setPopupMode(QToolButton::DelayedPopup);
  else
    menu = false;

  if (! menu) {
    button_->setPopupMode(QToolButton::InstantPopup);

    button_->setMenu(0);
  }
  else
    button_->setMenu(menu_);
}
