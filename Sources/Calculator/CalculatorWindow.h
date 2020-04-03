#pragma once

#include <QWidget>

// Класс для взаимодействия с UI интерфейсом
QT_BEGIN_NAMESPACE
namespace Ui { class CalculatorWindow; }
QT_END_NAMESPACE

class CalculatorWindow final : public QWidget
{
    /**
     * Тип действий пользователя
     */
    enum ActionType
    {
        AC_CLEAR,
        AC_EQUAL,
        AC_PLUS,
        AC_MINUS,
        AC_MULTIPLY,
        AC_DIVIDE,
        AC_PERCENT,
        AC_SIGN,
        AC_NUMBER_ENTER
    };

    /**
     * Тип операций
     */
    enum OperationType
    {
        OP_PLUS,
        OP_MINUS,
        OP_MULTIPLY,
        OP_DIVIDE,
    };

Q_OBJECT

public:
    /**
     * Конструктор окна
     * @param parent Родительский элемент
     */
    explicit CalculatorWindow(QWidget *parent = nullptr);

    /**
     * Деструктор окна
     */
    ~CalculatorWindow() override;


private slots:

    /**
     * Обработка события НЕ программного изменения текстового поля ввода числа
     * @param text Новый текст
     */
    void on_numericField_textEdited(const QString &text);

    /**
     * Обработка события нажатия на кнопку очистки
     */
    void on_btnClear_clicked();

    /**
     * Обработка события нажатия на кнопку равно
     */
    void on_btnEqual_clicked();

    /**
     * Обработка события нажатия на цифровые кнопки
     */
    void onNumericButtonPressed();

    /**
     * Обработка события нажатия на кнопки операций
     */
    void onOperationButtonPressed();

protected:

    /**
     * Перехват событий окна
     * @param obj Объект (элемент) формы
     * @param event Событие
     * @return Остановить распространение события
     */
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    /// Указатель на UI объект
    Ui::CalculatorWindow *ui_;

    /// Последнее корректное значение введенное в числовое поле
    QString lastValidValue_ = "0";

    /// Последнее действие
    ActionType lastAction_ = ActionType::AC_CLEAR;

    /// Последняя операция
    OperationType lastOperation_ = OperationType::OP_PLUS;

    /// Буфер числа
    double numberBuffer_ = 0.0;

    /**
     * Форматировать числовое значение
     * @param inputString Входная строка
     * @param defaultValue Строка в случае неудачного форматирования
     * @return Отформатированная строка
     */
    static QString formatNumericValue(const QString& inputString, const QString& defaultValue = "0");

    /**
     * Выполнить операцию
     * @param operation Тип операции
     * @param currentStrValue Текущее числовое значение в виде строки
     */
    void performOperation(OperationType operation, const QString& currentStrValue);
};
