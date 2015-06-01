#include <CQToolButton.h>

#define FOCUS_TRACKING 1

#include <QStyleOption>
#include <QProxyStyle>
#include <QPainter>
#include <QMouseEvent>
#include <QStylePainter>

#include <iostream>

class CQToolButtonStyle : public QProxyStyle {
 public:
  CQToolButtonStyle() : drawCustom_(true) { }

  void setDrawCustom(bool draw) { drawCustom_ = draw; }

  static void setDebug(bool debug) { debug_ = debug; }

 private:
  void drawComplexControl(ComplexControl cc, const QStyleOptionComplex *opt,
                          QPainter *p, const QWidget *widget) const {
    if (drawCustom_ && cc == CC_ToolButton) {
      SubControls mouseOverControls = QStyle::SC_None;

      if (opt->state & QStyle::State_MouseOver)
        mouseOverControls = (QStyle::SC_ToolButton | QStyle::SC_ToolButtonMenu);

      drawToolButton(proxy(), (QStyleOptionToolButton *) opt, p, widget, mouseOverControls);
    }
    else
      QProxyStyle::drawComplexControl(cc, opt, p, widget);
  }

 public:
  static void drawToolButton(const QStyle *style, const QStyleOptionToolButton *opt,
                             QPainter *p, const QWidget *w, SubControls mouseOverControls);

 private:
  static bool debug_;

  bool drawCustom_;
};

namespace {
  CQToolButtonStyle *toolbuttonStyle = 0;
}

bool CQToolButtonStyle::debug_ = false;

//------

CQToolButton::
CQToolButton(QWidget *parent) :
 QToolButton(parent), drawCustom_(true), debug_(false), mouseControls_(QStyle::SC_None)
{
  if (! toolbuttonStyle)
    toolbuttonStyle = new CQToolButtonStyle;

  setStyle(toolbuttonStyle);

#ifdef FOCUS_TRACKING
  setMouseTracking(true);
#endif
}

void
CQToolButton::
setAutoRaise(bool raise)
{
  QToolButton::setAutoRaise(raise);
}

void
CQToolButton::
setCheckable(bool check)
{
  QToolButton::setCheckable(check);
}

void
CQToolButton::
setDrawCustom(bool draw)
{
  drawCustom_ = draw;

  toolbuttonStyle->setDrawCustom(draw);

  update();
}

void
CQToolButton::
setDebug(bool debug)
{
  debug_ = debug;

  toolbuttonStyle->setDebug(debug);
}

void
CQToolButton::
leaveEvent(QEvent *e)
{
#ifdef FOCUS_TRACKING
  Q_UNUSED(e)

  QStyle::SubControl mouseControls = QStyle::SC_None;

  if (mouseControls != mouseControls_) {
    mouseControls_ = mouseControls;

    update();
  }
#else
  QToolButton::leaveEvent(e);
#endif
}

void
CQToolButton::
mouseMoveEvent(QMouseEvent *e)
{
#ifdef FOCUS_TRACKING
  QStyleOptionToolButton opt;

  QToolButton::initStyleOption(&opt);

  QRect button   =
    style()->subControlRect(QStyle::CC_ToolButton, &opt, QStyle::SC_ToolButton, this);
  QRect menuarea =
    style()->subControlRect(QStyle::CC_ToolButton, &opt, QStyle::SC_ToolButtonMenu, this);

  QStyle::SubControl mouseControls;

  if      (button.contains(e->pos()))
    mouseControls = QStyle::SC_ToolButton;
  else if (menuarea.contains(e->pos()))
    mouseControls = QStyle::SC_ToolButtonMenu;
  else
    mouseControls = QStyle::SC_None;

  if (mouseControls != mouseControls_) {
    mouseControls_ = mouseControls;

    update();
  }
#else
  QToolButton::mouseMoveEvent(e);
#endif
}

void
CQToolButton::
paintEvent(QPaintEvent *e)
{
  if (! drawCustom_)
    return QToolButton::paintEvent(e);

  QStyleOptionToolButton opt;

  QToolButton::initStyleOption(&opt);

  QPainter p(this);

  CQToolButtonStyle::drawToolButton(style(), &opt, &p, this, mouseControls_);

#if 0
  if (debug_) {
    std::cerr << "features: ";
    if (opt.features & QStyleOptionToolButton::Arrow     ) std::cerr << " arrow";
    if (opt.features & QStyleOptionToolButton::Menu      ) std::cerr << " menu";
    if (opt.features & QStyleOptionToolButton::PopupDelay) std::cerr << " delay";
    if (opt.features & QStyleOptionToolButton::HasMenu   ) std::cerr << " has_menu";
    std::cerr << std::endl;

    std::cerr << "controls: ";
    if (opt.subControls & QStyle::SC_ToolButton    ) std::cerr << " button";
    if (opt.subControls & QStyle::SC_ToolButtonMenu) std::cerr << " menu";
    std::cerr << std::endl;

    std::cerr << "active controls: ";
    if (opt.activeSubControls & QStyle::SC_ToolButton    ) std::cerr << " button";
    if (opt.activeSubControls & QStyle::SC_ToolButtonMenu) std::cerr << " menu";
    std::cerr << std::endl;

    std::cerr << "state: ";
    if (opt.state & QStyle::State_AutoRaise) std::cerr << " auto-raise";
    if (opt.state & QStyle::State_MouseOver) std::cerr << " mouse-over";
    if (opt.state & QStyle::State_Sunken   ) std::cerr << " sunken";
    if (opt.state & QStyle::State_On       ) std::cerr << " checked";
    if (opt.state & QStyle::State_Raised   ) std::cerr << " raised";
    if (opt.state & QStyle::State_HasFocus ) std::cerr << " focus";
    std::cerr << std::endl;
  }

  if      (opt.features & QStyleOptionToolButton::MenuButtonPopup)
    drawPopupButton(opt);
  else if (opt.features & QStyleOptionToolButton::HasMenu)
    drawMenuButton(opt);
  else
    drawNormalButton(opt);
#endif
}

void
CQToolButton::
drawPopupButton(QStyleOptionToolButton &opt)
{
  QStylePainter p(this);

  bool downWhileChecked = ((opt.state & QStyle::State_On) && (opt.state & QStyle::State_Sunken));

  bool buttonPressed     = (opt.activeSubControls & QStyle::SC_ToolButton    );
  bool menuButtonPressed = (opt.activeSubControls & QStyle::SC_ToolButtonMenu);

#ifdef FOCUS_TRACKING
  bool mouseOverButton     = (mouseControls_ == QStyle::SC_ToolButton);
  bool mouseOverMenuButton = (mouseControls_ == QStyle::SC_ToolButtonMenu);
#else
  bool mouseOverButton     = (opt.state & QStyle::State_MouseOver);
  bool mouseOverMenuButton = (opt.state & QStyle::State_MouseOver);
#endif

  bool autoRaise = (opt.state & QStyle::State_AutoRaise);
//bool pressed   = (opt.state & QStyle::State_Sunken);
  bool checked   = (opt.state & QStyle::State_On);
  bool hasFocus  = (opt.state & QStyle::State_HasFocus);
  bool enabled   = (opt.state & QStyle::QStyle::State_Enabled);

  QStyle::State buttonState = (enabled ? QStyle::State_Enabled : QStyle::State_None);

  // button is sunken if pressed or checked
  if      (buttonPressed || checked)
    buttonState |= QStyle::State_Sunken;
  // button is raised if auto raise and mouse over button or menu button or menu button pressed
  else if (autoRaise && (mouseOverButton || mouseOverMenuButton || menuButtonPressed))
    buttonState |= QStyle::State_Raised;
  // button is raised if not auto raise
  else if (! autoRaise)
    buttonState |= QStyle::State_Raised;

  QStyle::State menuButtonState = (enabled ? QStyle::State_Enabled : QStyle::State_None);

  // menu button is sunken if pressed
  if      (menuButtonPressed)
    menuButtonState |= QStyle::State_Sunken;
  // menu button is raised if auto raise and mouse over button or menu button or button pressed
  else if (autoRaise && (mouseOverButton || mouseOverMenuButton || buttonPressed))
    menuButtonState |= QStyle::State_Raised;
  // menu button is raised if not auto raise
  else if (! autoRaise)
    menuButtonState |= QStyle::State_Raised;

  //------

  // get button and menu rects
  QRect buttonRect =
    style()->subControlRect(QStyle::CC_ToolButton, &opt, QStyle::SC_ToolButton, this);
  QRect menuButtonRect =
    style()->subControlRect(QStyle::CC_ToolButton, &opt, QStyle::SC_ToolButtonMenu, this);

  //------

  QPalette pal = opt.palette;

  QColor bg = pal.background().color();

  // draw button
  bool drawButton = (buttonState & (QStyle::State_Sunken | QStyle::State_Raised));

  if (drawButton) {
    opt.rect    = buttonRect;
    opt.state   = buttonState;
    opt.palette = pal;

    if      (downWhileChecked)
      opt.palette.setBrush(QPalette::Active, QPalette::Button, bg.darker (100));
    else if (buttonPressed)
      opt.palette.setBrush(QPalette::Active, QPalette::Button, bg.darker (105));
    else if (mouseOverButton)
      opt.palette.setBrush(QPalette::Active, QPalette::Button, bg.lighter(105));
    else if (checked)
      opt.palette.setBrush(QPalette::Active, QPalette::Button, bg.darker (105));

    p.drawPrimitive(QStyle::PE_PanelButtonTool, opt);
  }

  // draw button focus
  if (hasFocus) {
    QStyleOptionFocusRect focus;

    if (drawButton)
      focus.rect = buttonRect.adjusted(3, 3, -3, -3);
    else
      focus.rect = buttonRect.adjusted(1, 1, -1, -1);

    p.drawPrimitive(QStyle::PE_FrameFocusRect, focus);
  }

  // draw label
  int fw = style()->pixelMetric(QStyle::PM_DefaultFrameWidth, &opt, this);

  opt.rect    = buttonRect.adjusted(fw, fw, -fw, -fw);
  opt.state   = buttonState;
  opt.palette = pal;

  opt.state &= ~QStyle::State_Sunken;

  if (checked || buttonPressed) {
    if (! downWhileChecked)
      opt.state |= QStyle::State_On;
  }

  p.drawControl(QStyle::CE_ToolButtonLabel, opt);

  // draw menu button
  bool drawMenuButton = (menuButtonState & (QStyle::State_Sunken | QStyle::State_Raised));

  if (drawMenuButton) {
    opt.rect    = menuButtonRect;
    opt.state   = menuButtonState;
    opt.palette = pal;

    if      (menuButtonPressed)
      opt.palette.setBrush(QPalette::Active, QPalette::Button, bg.darker (105));
    else if (mouseOverMenuButton)
      opt.palette.setBrush(QPalette::Active, QPalette::Button, bg.lighter(105));

    p.drawPrimitive(QStyle::PE_IndicatorButtonDropDown, opt);
  }

  // draw menu button focus
  if (hasFocus) {
    QStyleOptionFocusRect focus;

    if (drawMenuButton)
      focus.rect = menuButtonRect.adjusted(1, 2, -1, -2);
    else
      focus.rect = menuButtonRect.adjusted(1, 1, -1, -1);

    p.drawPrimitive(QStyle::PE_FrameFocusRect, focus);
  }

  // draw menu button arrow
  opt.rect    = menuButtonRect;
  opt.state   = menuButtonState;
  opt.palette = pal;

  p.drawPrimitive(QStyle::PE_IndicatorArrowDown, opt);
}

void
CQToolButton::
drawMenuButton(QStyleOptionToolButton &opt)
{
  QStylePainter p(this);

  bool downWhileChecked = ((opt.state & QStyle::State_On) && (opt.state & QStyle::State_Sunken));

  bool buttonPressed = (opt.activeSubControls & QStyle::SC_ToolButton);

#ifdef FOCUS_TRACKING
  bool mouseOverButton = (mouseControls_ == QStyle::SC_ToolButton);
#else
  bool mouseOverButton = (opt.state & QStyle::State_MouseOver);
#endif

  bool autoRaise = (opt.state & QStyle::State_AutoRaise);
//bool pressed   = (opt.state & QStyle::State_Sunken);
  bool checked   = (opt.state & QStyle::State_On);
  bool hasFocus  = (opt.state & QStyle::State_HasFocus);
  bool enabled   = (opt.state & QStyle::QStyle::State_Enabled);

  QStyle::State buttonState = (enabled ? QStyle::State_Enabled : QStyle::State_None);

  // button is sunken if pressed or checked
  if      (buttonPressed || checked)
    buttonState |= QStyle::State_Sunken;
  // button is raised if auto raise and mouse over button
  else if (autoRaise && mouseOverButton)
    buttonState |= QStyle::State_Raised;
  // button is raised if not auto raise
  else if (! autoRaise)
    buttonState |= QStyle::State_Raised;

  //------

  // get button rect
  QRect buttonRect =
    style()->subControlRect(QStyle::CC_ToolButton, &opt, QStyle::SC_ToolButton, this);

  int bw = 8;

  QRect subMenuRect = QRect(buttonRect.right() - bw, buttonRect.y(), bw - 2, buttonRect.height());
  QRect subTextRect = buttonRect.adjusted(-bw/2,0,-bw,0);

  //------

  QPalette pal = opt.palette;

  QColor bg = pal.background().color();

  // draw button
  bool drawButton = (buttonState & (QStyle::State_Sunken | QStyle::State_Raised));

  if (drawButton) {
    opt.rect    = buttonRect;
    opt.state   = buttonState;
    opt.palette = pal;

    if      (downWhileChecked || buttonPressed)
      opt.palette.setBrush(QPalette::Active, QPalette::Button, bg.darker (105));
    else if (mouseOverButton)
      opt.palette.setBrush(QPalette::Active, QPalette::Button, bg.lighter(105));

    p.drawPrimitive(QStyle::PE_PanelButtonTool, opt);
  }

  // draw button focus
  if (hasFocus) {
    QStyleOptionFocusRect focus;

    if (drawButton)
      focus.rect = subTextRect.adjusted(6, 3, -3, -3);
    else
      focus.rect = subTextRect.adjusted(6, 1, -3, -1);

    p.drawPrimitive(QStyle::PE_FrameFocusRect, focus);
  }

  // draw label
  int fw = style()->pixelMetric(QStyle::PM_DefaultFrameWidth, &opt, this);

  opt.rect    = subTextRect.adjusted(fw, fw, -fw, -fw);
  opt.state   = buttonState;
  opt.palette = pal;

  opt.state &= ~QStyle::State_Sunken;

  if (checked || buttonPressed) {
    if (! downWhileChecked)
      opt.state |= QStyle::State_On;
  }

  p.drawControl(QStyle::CE_ToolButtonLabel, opt);

  // draw menu button arrow
  opt.rect    = subMenuRect;
  opt.state   = buttonState;
  opt.palette = pal;

  p.drawPrimitive(QStyle::PE_IndicatorArrowDown, opt);
}

void
CQToolButton::
drawNormalButton(QStyleOptionToolButton &opt)
{
  QStylePainter p(this);

  bool downWhileChecked = ((opt.state & QStyle::State_On) && (opt.state & QStyle::State_Sunken));

  bool buttonPressed = (opt.activeSubControls & QStyle::SC_ToolButton);

#ifdef FOCUS_TRACKING
  bool mouseOverButton = (mouseControls_ == QStyle::SC_ToolButton);
#else
  bool mouseOverButton = (opt.state & QStyle::State_MouseOver);
#endif

  bool autoRaise = (opt.state & QStyle::State_AutoRaise);
//bool pressed   = (opt.state & QStyle::State_Sunken);
  bool checked   = (opt.state & QStyle::State_On);
  bool hasFocus  = (opt.state & QStyle::State_HasFocus);
  bool enabled   = (opt.state & QStyle::QStyle::State_Enabled);

  QStyle::State buttonState = (enabled ? QStyle::State_Enabled : QStyle::State_None);

  // button is sunken if pressed or checked
  if      (buttonPressed || checked)
    buttonState |= QStyle::State_Sunken;
  // button is raised if auto raise and mouse over button
  else if (autoRaise && mouseOverButton)
    buttonState |= QStyle::State_Raised;
  // button is raised if not auto raise
  else if (! autoRaise)
    buttonState |= QStyle::State_Raised;

  //------

  // get button rect
  QRect buttonRect =
    style()->subControlRect(QStyle::CC_ToolButton, &opt, QStyle::SC_ToolButton, this);

  //------

  QPalette pal = opt.palette;

  QColor bg = pal.background().color();

  // draw button
  bool drawButton = (buttonState & (QStyle::State_Sunken | QStyle::State_Raised));

  if (drawButton) {
    opt.rect    = buttonRect;
    opt.state   = buttonState;
    opt.palette = pal;

    if      (downWhileChecked || buttonPressed)
      opt.palette.setBrush(QPalette::Active, QPalette::Button, bg.darker (105));
    else if (mouseOverButton)
      opt.palette.setBrush(QPalette::Active, QPalette::Button, bg.lighter(105));

    p.drawPrimitive(QStyle::PE_PanelButtonTool, opt);
  }

  // draw button focus
  if (hasFocus) {
    QStyleOptionFocusRect focus;

    if (drawButton)
      focus.rect = buttonRect.adjusted(3, 3, -3, -3);
    else
      focus.rect = buttonRect.adjusted(1, 1, -1, -1);

    p.drawPrimitive(QStyle::PE_FrameFocusRect, focus);
  }

  // draw label
  int fw = style()->pixelMetric(QStyle::PM_DefaultFrameWidth, &opt, this);

  opt.rect    = buttonRect.adjusted(fw, fw, -fw, -fw);
  opt.state   = buttonState;
  opt.palette = pal;

  opt.state &= ~QStyle::State_Sunken;

  if (checked || buttonPressed) {
    if (! downWhileChecked)
      opt.state |= QStyle::State_On;
  }

  p.drawControl(QStyle::CE_ToolButtonLabel, opt);
}

//------

void
CQToolButtonStyle::
drawToolButton(const QStyle *style, const QStyleOptionToolButton *toolbutton,
               QPainter *p, const QWidget *widget, QStyle::SubControls mouseOverControls)
{
  if (debug_) {
    std::cerr << "features: ";
    if (toolbutton->features & QStyleOptionToolButton::Arrow     ) std::cerr << " arrow";
    if (toolbutton->features & QStyleOptionToolButton::Menu      ) std::cerr << " menu";
    if (toolbutton->features & QStyleOptionToolButton::PopupDelay) std::cerr << " delay";
    if (toolbutton->features & QStyleOptionToolButton::HasMenu   ) std::cerr << " has_menu";
    std::cerr << std::endl;

    std::cerr << "controls: ";
    if (toolbutton->subControls & QStyle::SC_ToolButton    ) std::cerr << " button";
    if (toolbutton->subControls & QStyle::SC_ToolButtonMenu) std::cerr << " menu";
    std::cerr << std::endl;

    std::cerr << "active controls: ";
    if (toolbutton->activeSubControls & QStyle::SC_ToolButton    ) std::cerr << " button";
    if (toolbutton->activeSubControls & QStyle::SC_ToolButtonMenu) std::cerr << " menu";
    std::cerr << std::endl;

    std::cerr << "state: ";
    if (toolbutton->state & QStyle::State_AutoRaise) std::cerr << " auto-raise";
    if (toolbutton->state & QStyle::State_MouseOver) std::cerr << " mouse-over";
    if (toolbutton->state & QStyle::State_Sunken   ) std::cerr << " sunken";
    if (toolbutton->state & QStyle::State_On       ) std::cerr << " checked";
    if (toolbutton->state & QStyle::State_Raised   ) std::cerr << " raised";
    if (toolbutton->state & QStyle::State_HasFocus ) std::cerr << " focus";
    std::cerr << std::endl;
  }

  //---

  // is this a menu button with separate menu popup button
  bool menu_popup = (toolbutton->features & QStyleOptionToolButton::MenuButtonPopup);

  // is this a menu button without separate menu popup button
  bool menu_no_popup = (! menu_popup && (toolbutton->features & QStyleOptionToolButton::HasMenu));

  // does menu button without separate menu popup button have text
  bool menu_no_popup_text (menu_no_popup && toolbutton->toolButtonStyle != Qt::ToolButtonIconOnly);

  if (debug_) {
    if (menu_popup        ) std::cerr << "menu_popup"         << std::endl;
    if (menu_no_popup     ) std::cerr << "menu_no_popup"      << std::endl;
    if (menu_no_popup_text) std::cerr << "menu_no_popup_text" << std::endl;
  }

  //---

  // tool button is pressed while checked
  bool downWhileChecked = ((toolbutton->state & QStyle::State_On) &&
                           (toolbutton->state & QStyle::State_Sunken));

  bool buttonPressed = (toolbutton->activeSubControls & QStyle::SC_ToolButton);

  bool menuButtonPressed = false;

  if (menu_popup)
    menuButtonPressed = (toolbutton->activeSubControls & QStyle::SC_ToolButtonMenu);

  bool mouseOverButton = (mouseOverControls == QStyle::SC_ToolButton);

  bool mouseOverMenuButton = false;

  if (menu_popup)
    mouseOverMenuButton = (mouseOverControls == QStyle::SC_ToolButtonMenu);

  bool autoRaise = (toolbutton->state & QStyle::State_AutoRaise);
//bool pressed   = (toolbutton->state & QStyle::State_Sunken);
  bool checked   = (toolbutton->state & QStyle::State_On);
  bool hasFocus  = (toolbutton->state & QStyle::State_HasFocus);
  bool enabled   = (toolbutton->state & QStyle::QStyle::State_Enabled);

  QStyle::State buttonState = (enabled ? QStyle::State_Enabled : QStyle::State_None);

  // button is sunken if pressed or checked
  if      (buttonPressed || checked)
    buttonState |= QStyle::State_Sunken;
  // button is raised if auto raise and mouse over button or menu button or menu button pressed
  else if (autoRaise && (mouseOverButton || mouseOverMenuButton || menuButtonPressed))
    buttonState |= QStyle::State_Raised;
  // button is raised if not auto raise
  else if (! autoRaise)
    buttonState |= QStyle::State_Raised;

  QStyle::State menuButtonState = QStyle::State_None;

  if (menu_popup) {
    menuButtonState = (enabled ? QStyle::State_Enabled : QStyle::State_None);

    // menu button is sunken if pressed
    if      (menuButtonPressed)
      menuButtonState |= QStyle::State_Sunken;
    // menu button is raised if auto raise and mouse over button or menu button or button pressed
    else if (autoRaise && (mouseOverButton || mouseOverMenuButton || buttonPressed))
      menuButtonState |= QStyle::State_Raised;
    // menu button is raised if not auto raise
    else if (! autoRaise)
      menuButtonState |= QStyle::State_Raised;
  }

  //---

  // get button and menu rects
  QRect buttonRect = style->subControlRect(CC_ToolButton, toolbutton, SC_ToolButton, widget);

  QRect menuButtonRect, subMenuRect, subTextRect;

  if      (menu_popup)
    menuButtonRect = style->subControlRect(CC_ToolButton, toolbutton, SC_ToolButtonMenu, widget);
  else if (menu_no_popup) {
    int bw = (menu_no_popup_text ? 8 : 6);

    subMenuRect = QRect(buttonRect.right() - bw, buttonRect.y(), bw - 2, buttonRect.height());
    subTextRect = buttonRect.adjusted(-bw/2,0,-bw,0);
  }

  //---

  QPalette pal = toolbutton->palette;

  QColor bg = pal.background().color();

  QStyleOptionToolButton sopt = *toolbutton;

  // draw button
  bool drawButton = (buttonState & (QStyle::State_Sunken | QStyle::State_Raised));

  if (drawButton) {
    sopt.rect    = buttonRect;
    sopt.state   = buttonState;
    sopt.palette = pal;

    if      (downWhileChecked)
      sopt.palette.setBrush(QPalette::Active, QPalette::Button, bg.darker (100));
    else if (buttonPressed)
      sopt.palette.setBrush(QPalette::Active, QPalette::Button, bg.darker (105));
    else if (mouseOverButton)
      sopt.palette.setBrush(QPalette::Active, QPalette::Button, bg.lighter(105));
    else if (checked)
      sopt.palette.setBrush(QPalette::Active, QPalette::Button, bg.darker (105));

    style->drawPrimitive(QStyle::PE_PanelButtonTool, &sopt, p, widget);
  }

  // draw button focus
  if (hasFocus) {
    QStyleOptionFocusRect focus;

    if      (menu_popup) {
      if (drawButton)
        focus.rect = buttonRect.adjusted(3, 3, -3, -3);
      else
        focus.rect = buttonRect.adjusted(1, 1, -1, -1);
    }
    else if (menu_no_popup) {
      if (drawButton)
        focus.rect = subTextRect.adjusted(6, 3, -3, -3);
      else
        focus.rect = subTextRect.adjusted(6, 1, -3, -1);
    }
    else {
      if (drawButton)
        focus.rect = buttonRect.adjusted(3, 3, -3, -3);
      else
        focus.rect = buttonRect.adjusted(1, 1, -1, -1);
    }

    style->drawPrimitive(QStyle::PE_FrameFocusRect, &focus, p, widget);
  }

  // draw label
  int fw = style->pixelMetric(QStyle::PM_DefaultFrameWidth, &sopt, widget);

  if      (menu_popup)
    sopt.rect = buttonRect.adjusted(fw, fw, -fw, -fw);
  else if (menu_no_popup)
    sopt.rect = subTextRect.adjusted(fw, fw, -fw, -fw);
  else
    sopt.rect = buttonRect.adjusted(fw, fw, -fw, -fw);

  sopt.state   = buttonState;
  sopt.palette = pal;

  sopt.state &= ~QStyle::State_Sunken;

  if (checked || buttonPressed) {
    if (! downWhileChecked)
      sopt.state |= QStyle::State_On;
  }

  style->drawControl(QStyle::CE_ToolButtonLabel, &sopt, p, widget);

  if (menu_popup) {
    // draw menu button
    bool drawMenuButton = (menuButtonState & (QStyle::State_Sunken | QStyle::State_Raised));

    if (drawMenuButton) {
      sopt.rect    = menuButtonRect;
      sopt.state   = menuButtonState;
      sopt.palette = pal;

      if      (menuButtonPressed)
        sopt.palette.setBrush(QPalette::Active, QPalette::Button, bg.darker (105));
      else if (mouseOverMenuButton)
        sopt.palette.setBrush(QPalette::Active, QPalette::Button, bg.lighter(105));

      style->drawPrimitive(QStyle::PE_IndicatorButtonDropDown, &sopt, p, widget);
    }

    // draw menu button focus
    if (hasFocus) {
      QStyleOptionFocusRect focus;

      if (drawMenuButton)
        focus.rect = menuButtonRect.adjusted(1, 2, -1, -2);
      else
        focus.rect = menuButtonRect.adjusted(1, 1, -1, -1);

      style->drawPrimitive(QStyle::PE_FrameFocusRect, &focus, p, widget);
    }
  }

  // draw menu button arrow
  if      (menu_popup || menu_no_popup) {
    if (menu_popup) {
      sopt.rect  = menuButtonRect;
      sopt.state = menuButtonState;
    }
    else {
      sopt.rect  = subMenuRect;
      sopt.state = buttonState;
    }

    sopt.palette = pal;

    style->drawPrimitive(QStyle::PE_IndicatorArrowDown, &sopt, p, widget);
  }
}
