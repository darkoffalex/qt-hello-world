#include "CalculatorWindow.h"
#include "./ui_CalculatorWindow.h"

#include <iostream>
#include <QKeyEvent>

/**
 * Конструктор окна
 * @param parent Родительский элемент
 */
CalculatorWindow::CalculatorWindow(QWidget *parent) : QWidget(parent), ui_(new Ui::CalculatorWindow())
{
    // Инициализация UI
    ui_->setupUi(this);

    // Связь кнопок и обработчиков (цифры)
    connect(ui_->btn0,&QPushButton::clicked,this,&CalculatorWindow::onNumericButtonPressed);
    connect(ui_->btn1,&QPushButton::clicked,this,&CalculatorWindow::onNumericButtonPressed);
    connect(ui_->btn2,&QPushButton::clicked,this,&CalculatorWindow::onNumericButtonPressed);
    connect(ui_->btn3,&QPushButton::clicked,this,&CalculatorWindow::onNumericButtonPressed);
    connect(ui_->btn4,&QPushButton::clicked,this,&CalculatorWindow::onNumericButtonPressed);
    connect(ui_->btn5,&QPushButton::clicked,this,&CalculatorWindow::onNumericButtonPressed);
    connect(ui_->btn6,&QPushButton::clicked,this,&CalculatorWindow::onNumericButtonPressed);
    connect(ui_->btn7,&QPushButton::clicked,this,&CalculatorWindow::onNumericButtonPressed);
    connect(ui_->btn8,&QPushButton::clicked,this,&CalculatorWindow::onNumericButtonPressed);
    connect(ui_->btn9,&QPushButton::clicked,this,&CalculatorWindow::onNumericButtonPressed);
    connect(ui_->btnDot,&QPushButton::clicked,this,&CalculatorWindow::onNumericButtonPressed);

    // Связь кнопок и обработчиков (операции)
    connect(ui_->btnPlus,&QPushButton::clicked,this,&CalculatorWindow::onOperationButtonPressed);
    connect(ui_->btnMinus,&QPushButton::clicked,this,&CalculatorWindow::onOperationButtonPressed);
    connect(ui_->btnMul,&QPushButton::clicked,this,&CalculatorWindow::onOperationButtonPressed);
    connect(ui_->btnDivide,&QPushButton::clicked,this,&CalculatorWindow::onOperationButtonPressed);

    // Установить фильтр событий приложения из этого объекта (для перехвата некоторых)
    QCoreApplication::instance()->installEventFilter(this);
}

/**
 * Деструктор окна
 */
CalculatorWindow::~CalculatorWindow()
{
    delete ui_;
}

/** S L O T S **/

/**
 * Обработка события НЕ программного изменения текстового поля ввода числа
 * @param text Новый текст
 */
void CalculatorWindow::on_numericField_textEdited(const QString &text)
{
    // Установить новое значение (если валидно)
    lastValidValue_ = CalculatorWindow::formatNumericValue(lastValidValue_);
    ui_->numericField->setText(lastValidValue_);

    // Последнее действие - ввод числа
    lastAction_ = ActionType::AC_NUMBER_ENTER;
}

/**
 * Обработка события нажатия на кнопку очистки
 */
void CalculatorWindow::on_btnClear_clicked()
{
    // Очистка номера
    this->lastValidValue_ = "0";
    this->ui_->numericField->setText(this->lastValidValue_);
    this->ui_->numericField->setFocus();

    // Очистка верхнего поля
    this->ui_->numericFieldUpper->setText("");

    // Очиска буфера
    this->numberBuffer_ = 0;

    // Последнее действие - очистка
    this->lastAction_ = ActionType::AC_CLEAR;
    this->lastOperation_ = OperationType::OP_PLUS;
}

/**
 * Обработка события нажатия на кнопку равно
 */
void CalculatorWindow::on_btnEqual_clicked()
{
    // Выполнить последнюю операцию
    this->performOperation(lastOperation_,this->ui_->numericField->text());

    // Обновить поле номера
    auto newValue = QString::number(this->numberBuffer_,'g');
    this->ui_->numericField->setText(newValue);

    // Очистка верхнего поля
    this->ui_->numericFieldUpper->setText("");

    // Очиска буфера
    this->numberBuffer_ = 0;

    // Последнее действие - очистка
    this->lastAction_ = ActionType::AC_CLEAR;
    this->lastOperation_ = OperationType::OP_PLUS;
}

/**
 * Обработка события нажатия на цифровые кнопки
 */
void CalculatorWindow::onNumericButtonPressed()
{
    // Получить отправителя события (сигнала)
    auto button = qobject_cast<QPushButton*>(sender());

    // Строка с текущим числом
    auto numericValue = ui_->numericField->text();

    // Если последним действием был ввод числа - добавить цифру
    if(lastAction_ == AC_NUMBER_ENTER){
        numericValue.append(button->text());
    }
        // В противном случае начать в вод с цифры
    else{
        numericValue = button->text();
    }

    // Установить новое значение (если валидно)
    lastValidValue_ = CalculatorWindow::formatNumericValue(numericValue,lastValidValue_);
    ui_->numericField->setText(lastValidValue_);

    // Последнее действие - ввод числа
    lastAction_ = ActionType::AC_NUMBER_ENTER;
}

/**
 * Обработка события нажатия на кнопки операций
 */
void CalculatorWindow::onOperationButtonPressed()
{
    // Выполнить последнюю операцию
    this->performOperation(lastOperation_,ui_->numericField->text());

    // Обновить поле номера
    auto newValue = QString::number(numberBuffer_,'g');
    ui_->numericField->setText(newValue);

    // Получить отправителя события (сигнала)
    auto button = qobject_cast<QPushButton*>(sender());

    // Смена последней операции и действия
    if(button->objectName() == "btnPlus"){
        this->lastAction_ = ActionType::AC_PLUS;
        this->lastOperation_ = OperationType::OP_PLUS;
    } else if (button->objectName() == "btnMinus"){
        this->lastAction_ = ActionType::AC_MINUS;
        this->lastOperation_ = OperationType::OP_MINUS;
    } else if (button->objectName() == "btnMul"){
        this->lastAction_ = ActionType::AC_MULTIPLY;
        this->lastOperation_ = OperationType::OP_MULTIPLY;
    } else if (button->objectName() == "btnDivide"){
        this->lastAction_ = ActionType::AC_DIVIDE;
        this->lastOperation_ = OperationType::OP_DIVIDE;
    }
}

/** E V E N T  F I L T E R S **/

/**
 * Перехват событий окна
 * @param obj Объект (элемент) формы
 * @param event Событие
 * @return Остановить распространение события
 */
bool CalculatorWindow::eventFilter(QObject *obj, QEvent *event)
{
    // Если событие исходит от поля ввода и это событие нажатия кнопок на клавиатуре
    if (obj == ui_->numericField && event->type() == QEvent::KeyPress)
    {
        // Если последнее действие это не ввод числа - обнулить поле
        // Это позволит вводить новое значение после каждого совершенного действия
        if(lastAction_ != ActionType::AC_NUMBER_ENTER){
            ui_->numericField->setText("0");
        }
    }

    return QObject::eventFilter(obj, event);
}

/** M E T H O D S **/

/**
 * Форматировать числовое значение
 * @param inputString Входная строка
 * @param defaultValue Строка в случае неудачного форматирования
 * @return Отформатированная строка
 */
QString CalculatorWindow::formatNumericValue(const QString &inputString, const QString &defaultValue)
{
    // Копировать строку для дальнейшего форматирования
    auto formatted = inputString;

    // Если в строке больше 1 символов и если строка начинается сноля, после которого не следует запятой
    // очиститьс троку от начинающихся нулей
    if(formatted.size() > 1){
        if(formatted[0] == '0' && formatted[1] != ',') formatted.remove(QRegExp("^[0]*"));
    }

    // Если строка не пуста и если первый символ это запятая - добавить перед ней 0
    if(!formatted.isEmpty()){
        if(formatted[0] == ',') formatted.prepend('0');
    }
        // Если строка пуста - ее значение равно нулю
    else{
        formatted = "0";
    }

    // Строка для конвертирования в число (заменяем запятые на точку)
    auto toConvert = formatted;
    toConvert.replace(',','.');

    // Попытка конвертирования
    bool isNumber = false;
    toConvert.toDouble(&isNumber);

    // Если удалось - вернуть новое отформатированное значение
    if(isNumber){
        return formatted;
    }

    // Вернуть значение по умолчанию
    return defaultValue;
}

/**
 * Выполнить операцию
 * @param operation Тип операции
 * @param currentStrValue Текущее числовое значение в виде строки
 */
void CalculatorWindow::performOperation(CalculatorWindow::OperationType operation, const QString &currentStrValue)
{
    bool success;
    auto numeric = currentStrValue.toDouble(&success);
    auto upperString = ui_->numericFieldUpper->text();
    bool signRequired = !upperString.isEmpty();

    if(success){
        switch(operation)
        {
            case OperationType::OP_PLUS:
                upperString.append((signRequired ? " + " : "") + currentStrValue);
                numberBuffer_ += numeric;
                break;
            case OperationType::OP_MINUS:
                upperString.append((signRequired ? " - " : "") + currentStrValue);
                numberBuffer_ -= numeric;
                break;
            case OperationType::OP_DIVIDE:
                upperString.append((signRequired ? " / " : "") + currentStrValue);
                numberBuffer_ /= numeric;
                break;
            case OperationType::OP_MULTIPLY:
                upperString.append((signRequired ? " * " : "") + currentStrValue);
                numberBuffer_ *= numeric;
                break;
        }

        ui_->numericFieldUpper->setText(upperString);
    }
}






