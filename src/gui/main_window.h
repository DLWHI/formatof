#ifndef FORMATOF_GUI_MAIN_WINDOW_H_
#define FORMATOF_GUI_MAIN_WINDOW_H_

#include <QLineEdit>
#include <QMainWindow>

#include "../core/format_list.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

  public:
    MainWindow(Controller* controller, QWidget* parent = nullptr);
    ~MainWindow();
  

  private:
    void setTexts();
    void connectEvents();

    void onPathButtonPress();
    void onScanButtonPress();

    Ui::MainWindow* ui;

    formatof::FormatDeterminer formats;
};
#endif  // FORMATOF_GUI_MAIN_WINDOW_H_ 

