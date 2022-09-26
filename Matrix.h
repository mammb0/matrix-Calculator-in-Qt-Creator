#pragma once

#include <QObject>
#include <QDebug>
#include <QFile>
#include <QMainWindow>


class Matrix: public QObject
{
    Q_OBJECT
public:
    Matrix();
    Matrix(const Matrix&);
    ~Matrix();
    /**
     * @brief Чтение из файла.\n
     * Функция для чтение из файла. Путь к файлу
     * указываеться в качестве параметра.
     * @param pathFile - путь к файлу
     */
    bool readFromFile(QString pathFile);

    QVector<QVector<double>> getMatrix();
    void setMatrix(QVector<QVector<double>>);

    void Init();
    void Init(int row, int column);
    bool CheckSumm(QVector<QVector<double>> vec1, QVector<QVector<double>> vec2); //проверка на сумму/разность
    QVector<QVector<double>> Summ(QVector<QVector<double>> vec1, QVector<QVector<double>> vec2); //сумма
    QVector<QVector<double>> Sub(QVector<QVector<double>> vec1, QVector<QVector<double>> vec2); //разность
    QVector<QVector<double>> Multiplication(QVector<QVector<double>> vec1, QVector<QVector<double>> vec2); //умножение
    bool CheckMultiplikation(QVector<QVector<double>> vec1, QVector<QVector<double>> vec2); //проверка на умножение
    QVector<QVector<double>> Div(QVector<QVector<double>> vec1, QVector<QVector<double>> vec2); //деление
    double Det(QVector<QVector<double>> vec); //определитель
    bool CheckDet(QVector<QVector<double>> vec);
   // bool Det0();
    QVector<QVector<double>> Rev(QVector<QVector<double>> vec); //обратная
    QVector<QVector<double>> Transp(QVector<QVector<double>> vec); //транспонированная
    void Mass(int i, int j, double a); //записывает в массив
    QVector<QVector<double>> Alg(QVector<QVector<double>> vec, int p, int q); //для алгебраического дополнения


    //Функция возвращает количество строк
    int Row();
    //Функция возвращает количество стоблцов
    int Column();


    //Оператор сложения для класса Matrix
    friend Matrix &operator+(Matrix &a, Matrix &b);
    friend Matrix &operator-(Matrix &a, Matrix &b);
    friend Matrix &operator*(Matrix &a, Matrix &b);
    //Оператор присваивания для класса Matrix
    Matrix& operator=(Matrix &a);


private:

    bool hasError = false;

    QVector<QVector<double>> matrix;
};

