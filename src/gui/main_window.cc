#include "main_window.h"

#include "./ui_main_window.h"


MainWindow::MainWindow(Controller* controller, QWidget* parent)
   : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  setTexts();
  connectEvents();
}

void MainWindow::setTexts() {
  ui->path_btn.setText(QString::fromWCharArray(path_btn_text[FORMATOF_ENGLISH]));
  ui->scan_btn.setText(QString::fromWCharArray(scan_btn_text[FORMATOF_ENGLISH]));
  QStringList headers;
  headers << QString::fromWCharArray(file_header_text[FORMATOF_ENGLISH]) << QString::fromWCharArray(format_header_text[FORMATOF_ENGLISH]);
  ui->result_table.setHorizontalHeaderLabels(headers);
}

void MainWindow::connectEvents() {
  connect(ui->path_btn, &QPushButton::click, onPathButtonPress); 
  connect(ui->scan_btn, &QPushButton::click, onScanButtonPress);  
}

void MainWindow::onPathButtonPress() {
  QStringList files = QFileDialog::getOpenFileNames(this);
  ui->path_edit.setText(files.join(';'));
}

void MainWindow::onScanButtonPress() {
  QStringList files = ui->path_edit.text().split(';');
  for (const auto& file :  files) {
    ui->result_table.insertRow(ui->result_table.rowCount());
    ui->result_table.item(ui->result_table.rowCount() - 1, 0)->setText(file);
    std::vector<std::reference_wrapper<const formatof::Format>> fmts = formats.formatOf(file.toStdString());
    if (fmts.empty()) {
      ui->result_table.item(ui->result_table.rowCount() - 1, 1)->setText(QString("(undeterminde)"));
    } else {
      auto i = fmts.begin();
      ui->result_table.item(ui->result_table.rowCount() - 1, 1)->setText(QString(i->get().name.c_str()));
      for (++i; i != fmts.end(); ++i) {
        ui->result_table.insertRow(ui->result_table.rowCount());
        ui->result_table.item(ui->result_table.rowCount() - 1, 1)->setText(QString(i->get().name.c_str()));
      }
    }
  }
}

MainWindow::~MainWindow() { delete ui; }
