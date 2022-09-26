#include "Calculator.h"
#include "ui_Calculator.h"

Calculator::Calculator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Calculator)
{
    ui->setupUi(this);
    ui->textEdit_result->setReadOnly(true);

    DrawMatrix(row,column);

    //connect(ui->pushButton_SaveMatrix, &QPushButton::clicked, this, &Calculator::on_pushButton_SaveMatrix_clicked);

}

Calculator::~Calculator()
{
    delete ui;
}

void Calculator::on_actionExit_triggered()
{
    this->close();
}

//считываем матрицы
void Calculator::on_actionOpen_triggered()
{
    QStringList listFileName;

    //Создаём объект класса
    QFileDialog openDialog;

    //Задаём путь, где будет открыто диалоговое окно
    openDialog.setDirectory(QDir::current());

    //Устанавливаем режим выбора файлов
    //QFileDialog::AnyFile          0	Имя файла независимо от того существует файл или нет.
    //QFileDialog::ExistingFile     1	Имя одного существующего файла.
    //QFileDialog::Directory        2	Имя директории. Отображаются и файлы и директории. Однако, нативное окно Windows не поддерживает отображение файлов при выборе директории.
    //QFileDialog::ExistingFiles	3	Имена нескольких существующих файлов.
    //QFileDialog::DirectoryOnly	4	Использует для отображения режим Directory и включенную опцию ShowDirsOnly.
    openDialog.setFileMode(QFileDialog::ExistingFiles);

    //Указать название окна
    openDialog.setWindowTitle(tr("Открыть файл"));

    //Задать необходимые фильтры
    //Ниже в комментарии пример, как задать несколько фильтров
    //"Изображения (*.png *.xpm *.jpg);;Тестовые файлы (*.txt);;Файлы XML (*.xml)"
    openDialog.setNameFilter(tr("Текстовый файл  (*.txt)"));
    if (openDialog.exec())
    {
        //selectedFiles возвращает QStringList - список выбранных файлов
        //Мы выбираем несколько файлов
        listFileName = openDialog.selectedFiles();
    }
    if(listFileName.size() == 2){
        if (p == 2){
            ui->textEdit_result->append(tr("\nОшибка! Невозможно выбрать новые матрицы с файла, поскольку две матрицы уже выбраны. Перед добавлением новых 2-х матриц удалите уже имеющиеся с помощью кнопки 'delete'"));
        }
        else{
            if(p == 1){
                ui->textEdit_result->append(tr("\nОшибка! Невозможно выбрать две матрицы с файла, поскольку одна матрица уже выбрана. Перед добавлением новых 2-х матриц удалите уже имеющуюся с помощью кнопки 'delete'"));
            }
            else{ //p==0
                p = 2;
                error1 = www1.readFromFile(listFileName.at(0));
                error2 = www2.readFromFile(listFileName.at(1));
                if (error1 || error2) {
                    ui->textEdit_result->setText("Ошибка в размерности строки матрицы");
                }
                else
                {
                    ui->textEdit_result->append(tr("\nВывод матрицы 1: "));
                    Output(www1.getMatrix());
                    ui->textEdit_result->append(tr("\nВывод матрицы 2: "));
                    Output(www2.getMatrix());
                }
            }
        }
    }

    if (listFileName.size() == 1){
        if(p == 2){
            ui->textEdit_result->append(tr("\nОшибка! Невозможно выбрать матрицу с файла, поскольку две матрицы уже выбраны. Перед добавлением новой матрицы удалите уже имеющуюся с помощью кнопки 'delete'"));
        }
        else {
            if(p == 1){
                p++;
                error1 = www2.readFromFile(listFileName.at(0));
                if (error1) {
                    ui->textEdit_result->setText("Ошибка в размерности строки матрицы");
                }
                else
                {
                    ui->textEdit_result->append(tr("\nВывод матрицы ")+QString::number(p)+tr(":"));
                    Output(www2.getMatrix());
                }
            }
            else { //p==0
                p++;
                error1 = www1.readFromFile(listFileName.at(0));
                if (error1) {
                    ui->textEdit_result->setText("Ошибка в размерности строки матрицы");
                }
                else
                {
                    ui->textEdit_result->append(tr("\nВывод матрицы ")+QString::number(p)+tr(":"));
                    Output(www1.getMatrix());
                }
            }
        }
    }
}

//вывод матрицы
void Calculator::Output(QVector<QVector<double>> vec)
{
    if (vec.size() != 0)
    {
        for (int i=1;i<=vec.size() ;++i )
        {
            QString str = "";
            for (int j=1;j<=vec[i-1].size() ;++j )
            {
                str += QString::number(vec[i-1][j-1],'f',3) + "\t";
            }
            ui->textEdit_result->append(str);
        }
    }
}

//динамическое выделение блоков LineEdit
void Calculator::DrawMatrix(int _row, int _column)
{
    clearLayout(ui->verticalLayout_Matrix);
    row = _row;
    column = _column;
    lineData = new QLineEdit *[row];
    for (int i=1; i<=row; ++i)
    {
        QHBoxLayout *hbox = new QHBoxLayout();
        lineData[i-1] = new QLineEdit[column];
        for (int j=1; j<=column ; ++j)
        {
            lineData[i-1][j-1].setMinimumSize(41,40);
            lineData[i-1][j-1].setMaximumSize(41,40);

            QRegExp rx("^-{0,1}([0-9]{1}|[1-9]{1}[0-9]*)[.]{1}\\d{,2}$");
            QValidator *validator = new QRegExpValidator(rx, this);

            lineData[i-1][j-1].setValidator(validator);
            hbox->addWidget(&lineData[i-1][j-1],Qt::AlignLeft|Qt::AlignTop);
        }
        ui->verticalLayout_Matrix->addLayout(hbox);
    }
    this->adjustSize();
}

//очищение Layout
void Calculator::clearLayout(QLayout* layout)
{
    while (QLayoutItem* item = layout->takeAt(0))
    {
        if (QWidget* widget = item->widget())
            widget->deleteLater();
        if (QLayout* childLayout = item->layout())
            clearLayout(childLayout);
        delete item;
    }
}

//очищение блоков LineEdit
void Calculator::clearLineEdits(QLineEdit **lineEdits)
{
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            lineEdits[i][j].clear();
        }
    }
}

//вызывается при нажатии на кнопку "Добавление матрицы" - считывание матрицы
void Calculator::on_pushButton_AddMatrix_clicked()
{
    if(p == 2){
        ui->textEdit_result->append(tr("\nОшибка! Невозможно добавить матрицу, поскольку две матрицы уже выбраны. Перед добавлением новой матрицы удалите уже имеющуюся с помощью кнопки 'delete'"));
    }
    else{
        if(p == 1){
            p++;
            www2.Init(row, column);
            for(int i = 0; i < row; i++){
                for(int j = 0; j < column; j++){
                    www2.Mass(i,j,lineData[i][j].text().toDouble());
                }
            }
            ui->textEdit_result->append(tr("\nВывод матрицы 2: "));
            Output(www2.getMatrix());
            clearLineEdits(lineData);
        }
        else{ //p==0
            p++;
            www1.Init(row, column);
            for(int i = 0; i < row; i++){
                for(int j = 0; j < column; j++){
                    www1.Mass(i,j,lineData[i][j].text().toDouble());
                }
            }
            ui->textEdit_result->append(tr("\nВывод матрицы 1: "));
            Output(www1.getMatrix());
            clearLineEdits(lineData);
        }
    }
}

//динамическое выделение памяти для блоков LineEdit при изменении количества строк
void Calculator::on_spinBox_row_valueChanged(int add)
{
    row = add;
    DrawMatrix(row,column);
}

//динамическое выделение памяти для блоков LineEdit при изменении количества столбцов
void Calculator::on_spinBox_column_valueChanged(int add)
{
    column = add;
    DrawMatrix(row,column);
}

//сложение
void Calculator::on_pushButton_Sum_clicked()
{
    if(p == 2){
        result = www1+www2;
        if(result.getMatrix().size() == 0)
        {
            ui->textEdit_result->append("Ошибка в размерности матрицы. Невозможно выполнить сложение. Правило: C[m][n] = A[m][n] + B[m][n]");
        }
        else {
            ui->textEdit_result->append(tr("\nРезультат сложения матриц: "));
            Output(result.getMatrix());
        }
    }
    else{
        if(p == 1){
            ui->textEdit_result->append(tr("\nДля реализации сложения матриц необходимы две матрицы. Выберите вторую необходимую матрицу из файла или введите вручную"));
        }
        else{ //p==0
            ui->textEdit_result->append(tr("\nДля реализации сложения матриц необходимы две матрицы. Выберите необходимые матрицы из файла или введите вручную"));
        }
    }
}

//вычитание
void Calculator::on_pushButton_Sub_clicked()
{
    if(p == 2){
        result = www1-www2;
        if(result.getMatrix().size() == 0)
        {
            ui->textEdit_result->append("Ошибка в размерности матрицы. Невозможно выполнить вычитание. Правило: C[m][n] = A[m][n] - B[m][n]");
        }
        else {
            ui->textEdit_result->append(tr("\nРезультат вычитания матриц: "));
            Output(result.getMatrix());
        }
    }
    else {
        if(p == 1){
            ui->textEdit_result->append(tr("\nДля реализации вычитания матриц необходимы две матрицы. Выберите вторую необходимую матрицу из файла или введите вручную"));
        }
        else{ // p==0
            ui->textEdit_result->append(tr("\nДля реализации вычитания матриц необходимы две матрицы. Выберите необходимые матрицы из файла или введите вручную"));
        }
    }
}

//умножение
void Calculator::on_pushButton_Mul_clicked()
{   
    if(p == 2){
        result = www1*www2;
        if(result.getMatrix().size() == 0)
        {
            ui->textEdit_result->append("Ошибка в размерности матрицы. Невозможно выполнить умножение. Правило: C[m1][m2] = A[m1][n] * B[n][m2]");
        }
        else {
            ui->textEdit_result->append(tr("\nРезультат умножения матриц: "));
            Output(result.getMatrix());
        }
    }
    else{
        if(p == 1){
            ui->textEdit_result->append(tr("\nДля реализации умножения матриц необходимы две матрицы. Выберите вторую необходимую матрицу из файла или введите вручную"));
        }
        else{
            ui->textEdit_result->append(tr("\nДля реализации умножения матриц необходимы две матрицы. Выберите необходимые матрицы из файла или введите вручную"));
        }
    }
}

//удаление последней записанной матрицы
void Calculator::on_pushButton_Del_clicked()
{
    if(p == 0){
        ui->textEdit_result->append("\nВы уже удалили все существующие матрицы");
    }
    else{
        if(p == 1){
            p--;
            www1.Init();
            ui->textEdit_result->append("\nВы удалили матрицу 1");
        }
        else{
            p--;
            www2.Init();
            ui->textEdit_result->append("\nВы удалили матрицу 2");
        }
    }
}

void Calculator::on_pushButton_SaveMatrix_clicked()
{
    if(p == 2){
        www2.Init();
        p--;
    }
    if(p == 1){
        www1.Init();
        p--;
    }
    www1 = result;
    ui->textEdit_result->append("\nРезультат вычисления сохранен в матрицу 1 для продолжения работы с полученной матрицей. Матрица 1 содержит следующие значения:");
    p++;
    Output(www1.getMatrix());
}

//деление
void Calculator::on_pushButton_Div_clicked()
{
    if(p == 2){
        result.Init(www1.Row(),www1.Column());
        result.setMatrix(www1.Div(www1.getMatrix(), www2.getMatrix()));
        if(result.Row() == 0)
        {
            ui->textEdit_result->append(tr("Невозможно выполнить деление. C[m][n] = A[m][n]/B[n][n] = A*B^(-1). Матрица В должна быть квадраной и ее определитель не должен быть равен 0."));
        }
        else {
            ui->textEdit_result->append(tr("\nРезультат деления матриц: "));
            Output(result.getMatrix());
        }
    }
    else{
        if(p == 1){
            ui->textEdit_result->append(tr("\nДля реализации деления матриц необходимы две матрицы. Выберите вторую необходимую матрицу из файла или введите вручную"));
        }
        else{ //p==0
            ui->textEdit_result->append(tr("\nДля реализации деления матриц необходимы две матрицы. Выберите необходимые матрицы из файла или введите вручную"));
        }
    }
}

//определитель
void Calculator::on_pushButton_Det_clicked()
{
    double det;
    if(p == 2){
        ui->textEdit_result->append(tr("\nДля вычисления определителя матрицы необходима одна матрица. Удалите вторую записанную матрицу с помощью кнопки 'delete'"));
    }
    else{
        if(p == 1){
            if(www1.CheckDet(www1.getMatrix())){
                det = www1.Det(www1.getMatrix());
                ui->textEdit_result->append(tr("\nОпределитель матрицы 1: ") + QString::number(det));
            }
            else{
                ui->textEdit_result->append(tr("\nНевозможно вычислить определитель, поскольку матрица не квадратная"));
            }
        }
        else{ //p==0
            ui->textEdit_result->append(tr("\nДля вычисления определителя матрицы необходима одна матрица. Выберите необходимую матрицу из файла или введите вручную"));
        }
    }
}

//транспонированная
void Calculator::on_pushButton_Transp_clicked()
{
    if(p == 2){
        ui->textEdit_result->append(tr("\nДля вычисления транспонированной матрицы необходима одна матрица. Удалите вторую записанную матрицу с помощью кнопки 'delete'"));
    }
    else{
        if(p == 1){   
            result.Init(www1.Column(),www1.Row());
            result.setMatrix(www1.Transp(www1.getMatrix()));
            ui->textEdit_result->append(tr("\nТранспонированная матрица 1:"));
            Output(result.getMatrix());
        }
        else{ //p==0
            ui->textEdit_result->append(tr("\nДля вычисления транспонированной матрицы необходима одна матрица. Выберите необходимую матрицу из файла или введите вручную"));
        }
    }
}

//обратная (дооформить if else)
void Calculator::on_pushButton_Rev_clicked()
{
    if(p == 2){
        ui->textEdit_result->append(tr("\nДля вычисления обратной матрицы необходима одна матрица. Удалите вторую записанную матрицу с помощью кнопки 'delete'"));
    }
    else{
        if(p == 1){
            result.Init(www1.Row(),www1.Column());
            result.setMatrix(www1.Rev(www1.getMatrix()));
            if(result.Row() == 0){
                ui->textEdit_result->append(tr("\nНевозможно вычислить обратную матрицу, поскольку матрица не квадратная или определитель матрицы равено 0"));
            }
            else{
                ui->textEdit_result->append(tr("\nМатрица, обратная матрице 1:"));
                Output(result.getMatrix());
            }

        }
        else{ //p==0
            ui->textEdit_result->append(tr("\nДля вычисления обратной матрицы необходима одна матрица. Выберите необходимую матрицу из файла или введите вручную"));
        }
    }
}

