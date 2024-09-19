#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_browseFile_clicked();

    void on_pushButton_encodeFile_clicked();

    void on_pushButton_decodeFile_clicked();

    void on_pushButton_browseDir_clicked();

    void on_pushButton_encodeDir_clicked();

    void on_pushButton_verifyDir_clicked();

    void on_pushButton_encodeText_clicked();

    void on_pushButton_decodeText_clicked();

    void on_comboBox_level_currentIndexChanged(int index);

    void on_comboBox_mode_currentIndexChanged(int index);

    void on_comboBox_padding_currentIndexChanged(int index);

    void on_lineEdit_key_editingFinished();

    void on_lineEdit_iv_editingFinished();

private:
    Ui::MainWindow *ui;

    void initUI();
};
#endif // MAINWINDOW_H
