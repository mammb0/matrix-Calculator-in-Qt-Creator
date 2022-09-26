#pragma once

#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QDir>
#include <QList>
#include <QVector>
#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLayoutItem>
#include <QLineEdit>
#include <QRegExp>
#include <QRegExpValidator>

#include "Matrix.h"


QT_BEGIN_NAMESPACE
namespace Ui { class Calculator; }
QT_END_NAMESPACE

/**
 * @brief The Calculator class
 * Класс для работы примера
 */
class Calculator : public QMainWindow
{
    Q_OBJECT

public:
    Calculator(QWidget *parent = nullptr);
    ~Calculator();

private slots:
    void on_actionExit_triggered(); //закрыть файл
    void on_actionOpen_triggered(); //открыть файл
    void on_pushButton_AddMatrix_clicked(); //считать размерность матрицы и матрицу
    void on_spinBox_row_valueChanged(int row); //считывание размерности с spinBox
    void on_spinBox_column_valueChanged(int column); //считывание размерности с spinBox
    void on_pushButton_Sum_clicked(); //вычисление суммы матриц
    void on_pushButton_Sub_clicked(); //вычисление разности матриц
    void on_pushButton_Mul_clicked(); //вычисление произведения матриц
    void on_pushButton_Del_clicked(); //удалить последнюю записанную матрицу
    void on_pushButton_SaveMatrix_clicked();//
    void on_pushButton_Div_clicked();
    void on_pushButton_Det_clicked();
    void on_pushButton_Transp_clicked();
    void on_pushButton_Rev_clicked();

private:
    Ui::Calculator *ui;


    void Output(QVector<QVector<double>> vec); //вывод матрицы
    QVector<Matrix> *massmatrix;
    bool error1 = true,error2 = true;
    Matrix www1, www2, result;
    int row = 1, column = 1, p = 0;
    QLineEdit ** lineData;
    /**
     * @brief Отрисовка динамической матрицы
     */
    void DrawMatrix(int,int); //считыть матрицу из динамически выделенных блоков LineEdit
    void clearLayout(QLayout*);
    void clearLineEdits(QLineEdit**lineEdits);
    QVector<QVector<QLineEdit>> **lineEdits;

};
