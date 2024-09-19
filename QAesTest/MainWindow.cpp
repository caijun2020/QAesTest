#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "QAesTool.h"
#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_browseFile_clicked()
{
    QString defaultLocalDir = ui->lineEdit_filePath->text();

    QString fileName = QFileDialog::getOpenFileName( this,
                                                 tr("Select the file"),
                                                 defaultLocalDir,
                                                 tr("files(*.*)"));
    if (!fileName.isEmpty())
    {
        ui->lineEdit_filePath->setText(fileName);
    }
}

void MainWindow::on_pushButton_encodeFile_clicked()
{
    QAesTool::instance()->encodeFile(ui->lineEdit_filePath->text());
}

void MainWindow::on_pushButton_decodeFile_clicked()
{
    QAesTool::instance()->decodeFile(ui->lineEdit_filePath->text());
}

void MainWindow::on_pushButton_browseDir_clicked()
{
    QString defaultLocalDir = ui->lineEdit_dirPath->text();

    QString directory = QFileDialog::getExistingDirectory( this,
                                                 tr("Select the Directory"),
                                                 defaultLocalDir,
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);

    directory = QDir::fromNativeSeparators(directory);
    if (!directory.isEmpty())
    {
        ui->lineEdit_dirPath->setText(directory);
    }
}

void MainWindow::on_pushButton_encodeDir_clicked()
{

}

void MainWindow::on_pushButton_verifyDir_clicked()
{

}

void MainWindow::on_pushButton_encodeText_clicked()
{
    if(!ui->textEdit_input->toPlainText().isEmpty())
    {
        QString encode = QAesTool::instance()->encodeString(ui->textEdit_input->toPlainText(), ui->checkBox_base64->isChecked());
        ui->textEdit_output->setPlainText(encode);
    }
}

void MainWindow::on_pushButton_decodeText_clicked()
{
    if(!ui->textEdit_output->toPlainText().isEmpty())
    {
        QString decode = QAesTool::instance()->decodeString(ui->textEdit_output->toPlainText(), ui->checkBox_base64->isChecked());
        ui->textEdit_input->setPlainText(decode);
    }
}

void MainWindow::initUI()
{
    ui->lineEdit_key->setText(QAesTool::instance()->getKey());
    ui->lineEdit_iv->setText(QAesTool::instance()->getIV());
    ui->comboBox_level->setCurrentIndex((int)QAesTool::instance()->getAesLevel());
    ui->comboBox_mode->setCurrentIndex((int)QAesTool::instance()->getAesMode());
    ui->comboBox_padding->setCurrentIndex((int)QAesTool::instance()->getAesPadding());
}

void MainWindow::on_comboBox_level_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    QAesTool::instance()->initEncryption((QAESEncryption::Aes)ui->comboBox_level->currentIndex(),
                                         (QAESEncryption::Mode)ui->comboBox_mode->currentIndex(),
                                         (QAESEncryption::Padding)ui->comboBox_padding->currentIndex());
}

void MainWindow::on_comboBox_mode_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    QAesTool::instance()->initEncryption((QAESEncryption::Aes)ui->comboBox_level->currentIndex(),
                                         (QAESEncryption::Mode)ui->comboBox_mode->currentIndex(),
                                         (QAESEncryption::Padding)ui->comboBox_padding->currentIndex());
}

void MainWindow::on_comboBox_padding_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    QAesTool::instance()->initEncryption((QAESEncryption::Aes)ui->comboBox_level->currentIndex(),
                                         (QAESEncryption::Mode)ui->comboBox_mode->currentIndex(),
                                         (QAESEncryption::Padding)ui->comboBox_padding->currentIndex());
}

void MainWindow::on_lineEdit_key_editingFinished()
{
    QAesTool::instance()->initKey(ui->lineEdit_key->text());
}

void MainWindow::on_lineEdit_iv_editingFinished()
{
    QAesTool::instance()->initIV(ui->lineEdit_iv->text());
}
