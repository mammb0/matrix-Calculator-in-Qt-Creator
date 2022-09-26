#include "Matrix.h"
#include <math.h>

//Класс по умолчанию
Matrix::Matrix()
{
    matrix.resize(0);
}

//Класс копирования
Matrix::Matrix(const Matrix &m)
{
    for(int i=0; i<int(matrix.size()); ++i)
    {
        for (int j=0; j<int(matrix[i].size()); ++j)
        {
            matrix[i][j] = m.matrix[i][j];
        }
    }

}

Matrix::~Matrix()
{
    // Перед чтением очистим наш думерный вектор от предыдущей информации
    matrix.clear();
    matrix.resize(0);
}

bool Matrix::readFromFile(QString pathFile)
{
    QFile file;
    int column = 0;


    // Перед чтением очистим наш думерный вектор от предыдущей информации
    Init();


    //Указываем файл
    file.setFileName(pathFile);
    qDebug()<<"readFromFile: Filename = "<<file.fileName();

    //Открыть файл только для чтение и в текстовом режиме
    if (file.open(QIODevice::ReadOnly |QIODevice::Text))
    {
        //Пока не конец файла
        while (!file.atEnd())
        {
            //Читаем построчно
            QString str = file.readLine();
            //Разбить строку по разделителю " " - первый аргумент
            // в функции split
            // второй аргумент - пропускать пустые элементы
            // Результат является список  - объект класса QStringList
            QStringList list = str.split(" ",QString::SkipEmptyParts);

            //Узнаем размер списка. В нашем случае это количество столбцов

            if (column == 0 ) column = list.size();

            if (column != list.size())
            {
                //Не совпадает размерность стоблцов выводим ошибку
                //ui->textEdit->setText("Ошибка в размерности столбца матрицы");
                hasError = true;
                file.close();
                matrix.clear();
                matrix.resize(0);
                break;

            }
            else
            {
                hasError = false;
                QVector<double> vectorRow;

                //Для каждого элемента списка QStringList
                // Элемент в QStringList - QString
                foreach (QString element,list)
                {
                    //Преобразуем элемент в double и добавляем его в vectorRow
                    vectorRow.append(element.toDouble());
                }

                //Добавляем в конец списка QVector наш созданный QVector
                matrix.push_back(vectorRow);
            }

        }
    }

    //Закрываем файл
    file.close();
    return hasError;
}

QVector<QVector<double> > Matrix::getMatrix()
{
    return matrix;
}

void Matrix::setMatrix(QVector<QVector<double> > a)
{
    if(matrix.size() == a.size() && matrix[0].size() == a[0].size()){
        for(int i = 0; i < a.size(); i++){
            for(int j = 0; j < a[0].size(); j++){
                matrix[i][j] = a[i][j];
            }
        }
    }
}

void Matrix::Init()
{
    matrix.clear();
    matrix.resize(0);

}

void Matrix::Init(int row, int column)
{
    matrix.clear();
    // Реализация матрицы заданного размера
    matrix.resize(row);
    for(int i = 0; i < row; i++){
        matrix[i].resize(column);
    }
}

//проверяем размерность матриц
bool Matrix:: CheckSumm(QVector<QVector<double>> vec1, QVector<QVector<double>> vec2){
    int p = false;
    if (vec1[0].size() == vec2[0].size() && vec1.size() == vec2.size())
        p = true;
    return p;
}

//складываем матрицы
QVector<QVector<double>>  Matrix:: Summ(QVector<QVector<double>> vec1, QVector<QVector<double>> vec2){
    QVector<QVector<double>> vec;
    if (CheckSumm(vec1,vec2))
    {
        int i = 1, j = 1;
        vec = vec1;
        for(i=1; i <= vec1.size(); i++ ){
            for(j=1; j <= vec1[i-1].size(); j++){
                vec[i-1][j-1] = vec1[i-1][j-1] + vec2[i-1][j-1];
            }
        }
    }
    return vec;
}

//вычитание матриц
QVector<QVector<double>>  Matrix:: Sub(QVector<QVector<double>> vec1, QVector<QVector<double>> vec2){
    QVector<QVector<double>> vec;
    if (CheckSumm(vec1,vec2))
    {
        int i = 1, j = 1;
        vec = vec1;
        for(i=1; i <= vec1.size(); i++ ){
            for(j=1; j <= vec1[i-1].size(); j++){
                vec[i-1][j-1] = vec1[i-1][j-1] - vec2[i-1][j-1];
            }
        }
    }
    return vec;
}

QVector<QVector<double>> Matrix::  Multiplication(QVector<QVector<double>> vec1, QVector<QVector<double>> vec2){
    QVector<QVector<double>> vec(vec1.size());
    if (CheckMultiplikation(vec1,vec2))
    {
        int i = 0, j = 0, k = 0;
        double c = 0;
        for(i = 0; i  < vec1.size(); i++)
        {
            vec[i].resize(vec2[0].size());
        }

        for(i=0; i < vec1.size(); i++ ){
            for(j=0; j < vec2[0].size(); j++){
                for(k=0; k < vec2.size(); k++){
                    c += vec1[i][k]*vec2[k][j];
                }
                vec[i][j] = c;
                c = 0;
            }
        }
    }
    return vec;
}

bool Matrix::  CheckMultiplikation(QVector<QVector<double>> vec1, QVector<QVector<double>> vec2){
    bool p = false;
    if (vec1[1].size() == vec2.size())
        p = true;
    return p;
}

QVector<QVector<double>> Matrix:: Div(QVector<QVector<double>> vec1, QVector<QVector<double>> vec2){
    //память
    //vec0=vec2^(-1)
    QVector<QVector<double>> vec0 (vec2.size());
    for(int i = 0; i  < vec2.size(); i++)
    {
        vec0[i].resize(vec2[0].size());
    }
    vec0 = Rev(vec2); //вычисление обратной матрицы
    //умножение
    QVector<QVector<double>> vec(vec1.size());
    if (CheckMultiplikation(vec1,vec0))
    {
        int i = 0, j = 0, k = 0;
        double c = 0;
        for(i = 0; i  < vec1.size(); i++)
        {
            vec[i].resize(vec0[0].size());
        }

        for(i=0; i < vec1.size(); i++ ){
            for(j=0; j < vec0[0].size(); j++){
                for(k=0; k < vec0.size(); k++){
                    c += vec1[i][k]*vec0[k][j];
                }
                vec[i][j] = c;
                c = 0;
            }
        }
    }
    return vec;
}

//определитель
double Matrix:: Det(QVector<QVector<double>> vec){
    double det = 1, tmp, coef;
    int i, j = 0, p, flag = 0, q;
    for (i = 0; i < vec.size()-1; i++){ //проверяем 0 на диагонали
        if(vec[i][j] == 0){
            for (p = i + 1; p < vec.size(); p++){
                if(vec[p][j] != 0){ //нашли строку р с не 0 на диагонали
                    for(q = 0; q < vec.size(); q ++){ //меняем местами строчки p и i (Идем по строке)
                        tmp = vec[i][q];
                        vec[i][q] = vec[p][q];
                        vec[p][q] = tmp;
                    }
                    det = det*(-1);
                    flag ++; //изменили флаг - нашли строку с не 0 на диагонали, иначе 0 на диагонали и det=0
                }
            }
            if (flag == 0) return 0; //на диагонали будет 0
        }
        for(p = i; p < vec.size(); p ++){ //единички
            for (q = j; q < vec.size(); q ++){
                if (q == j){ //запомним на первом делении коэффициент
                    if(vec[p][j] == 0){
                        break;
                    }
                    else{
                        coef = vec[p][q];
                        det = det*coef;
                    }
                }

                vec[p][q] = vec[p][q]/coef; //на диагонали = 1
            }
        }
        for(p = i + 1; p < vec.size(); p ++){ //зануление от mass[i+1][j] до последнего значения в столбце
            for (q = vec.size() - 1; q >= 0; q --){
                if(vec[p][j] == 0){
                    break;
                }
                else {
                    vec[p][q] = vec[p][q] - vec[i][q];
                }
            }
        }
        j ++;
    }
    det = det * vec[vec.size()-1][vec.size()-1];
    return det;
}

bool Matrix::  CheckDet(QVector<QVector<double>> vec){
    bool p = true;
    if (vec.size() != vec[0].size())
        p = false;
    return p;
}

//обратная матрица
QVector<QVector<double>> Matrix:: Rev(QVector<QVector<double>> vec){
    int i, j;
    QVector<QVector<double>> vec_rev;
    QVector<QVector<double>> vec_alg; //алгебраические дополнения, размер равен vec
    vec_alg.resize(vec.size());
    for(i = 0; i < vec.size(); i++){
        vec_alg[i].resize(vec[0].size());
    }
    QVector<QVector<double>> vec0;//после вычеркивания строки и столбца на пересечении, размер -1
    vec0.resize(vec.size() - 1);
    for(i = 0; i < vec.size() - 1; i++){
        vec0[i].resize(vec[0].size() - 1);
    }
    double det = Det(vec); //определитель
    if(CheckDet(vec) && det != 0){ //если есть определитель
        //QVector<QVector<double>> vec_rev; // обратная матрица, транспонированная для полученного vec
        vec_rev.resize(vec[0].size());
        for(i = 0; i < vec[0].size(); i++){
            vec_rev[i].resize(vec.size());
        }
        //алг. доп
        for(i = 0; i < vec_alg.size(); i++){
            for(j = 0; j < vec_alg[0].size(); j++){
                vec0 = Alg(vec, i, j);
                //double d = Det(vec0);
                //double v = pow(-1, i + j);
                vec_alg[i][j] = pow(-1, i + j) * Det(vec0);
            }
        }
        vec_rev = Transp(vec_alg);//трансп
        //деление vec_rev = vec_rev/det
        for(i = 0; i < vec_rev.size(); i++){
            for(j = 0; j < vec_rev[0].size(); j++){
                vec_rev[i][j] = vec_rev[i][j]/det;
            }
        }
    }
    return vec_rev;
}

QVector<QVector<double>> Matrix::Alg(QVector<QVector<double>> vec, int p, int q){
    int i, j, k = 0, l = 0;
    //после вычеркивания строки и столбца на пересечении, размер -1
    QVector<QVector<double>> vec0;
    vec0.resize(vec.size() - 1);
    for(i = 0; i < vec.size() - 1; i++){
        vec0[i].resize(vec[0].size() - 1);
    }
    for(i = 0; i < vec.size(); i++){
        if(i != p){
            for(j = 0; j < vec[0].size(); j++){
                if (j != q){
                    vec0[k][l] = vec[i][j];
                    l++;
                    if(l == vec[0].size()-1){
                        k++;
                        l = 0;
                    }
                }
            }
        }
    }
    return vec0;
}

int Matrix::Row()
{
    int n;
    n = matrix.size();
    return  n;
}

int Matrix::Column()
{
    int n;
    n = matrix[0].size();
    return  n;
}



QVector<QVector<double>> Matrix:: Transp(QVector<QVector<double>> vec){
    //замена строки на столбец
    int i=0, j = 0;
    //память
    QVector<QVector<double>> vec0 (vec[0].size());
    for(i = 0; i  < vec[0].size(); i++)
    {
        vec0[i].resize(vec.size());
    }
    //замена строки на стоблец
    for(i=0; i < vec0.size(); i++ ){
        for(j=0; j < vec0[0].size(); j++){
            vec0[i][j] = vec[j][i];
        }
    }
    return vec0;
}

void Matrix::Mass(int i, int j, double a){
    if(i < int(matrix.size()) && j < int(matrix[0].size())){
        matrix[i][j] = a;
    }
}

//сложение
Matrix &operator+(Matrix &a, Matrix &b)
{
    Matrix *c = new Matrix();
    if(c->CheckSumm(a.getMatrix(),b.getMatrix()))
    {
        c->Init(a.Row(),a.Column());
        for(int i=0; i<a.Row(); ++i)
        {
            for(int j=0; j<a.Column();++j)
            {
                c->matrix[i][j] = a.matrix[i][j] + b.matrix[i][j];
            }
        }
    }

    return *c;
}

//вычитание
Matrix &operator-(Matrix &a, Matrix &b)
{
    Matrix *c = new Matrix();
    if(c->CheckSumm(a.getMatrix(),b.getMatrix()))
    {
        c->Init(a.Row(),a.Column());
        for(int i=0; i<a.Row(); ++i)
        {
            for(int j=0; j<a.Column();++j)
            {
                c->matrix[i][j] = a.matrix[i][j] - b.matrix[i][j];
            }
        }
    }

    return *c;
}

//умножение
Matrix &operator*(Matrix &a, Matrix &b)
{
    Matrix *c = new Matrix();
    if(c->CheckMultiplikation(a.getMatrix(),b.getMatrix()))
    {
        c->Init(a.Row(),b.Column());
        int i = 0, j = 0, k = 0;
        double p = 0;
        for(i=0; i < c->matrix.size(); i++ ){
            for(j=0; j < c->matrix[0].size(); j++){
                for(k=0; k < b.Row(); k++){
                    p += a.matrix[i][k]*b.matrix[k][j];
                }
                c->matrix[i][j] = p;
                p = 0;
            }
        }
    }
    return *c;
}

//присваивание
Matrix &Matrix::operator =(Matrix &a)
{
    if(a.getMatrix().size() != 0){
        this->Init(a.Row(), a.Column());
        for(int i=0; i< a.Row(); i++)
        {
            for(int j = 0; j < a.Column(); j++){
                matrix[i][j] = a.matrix[i][j];
            }
        }
    }
    else{
        this->Init();
    }
    return *this; //возвращает объект который сгенерировал вызов
}
