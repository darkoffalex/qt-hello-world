#include "CalculatorWindow.h"

#include <QApplication>
#include <QFontDatabase>
#include <QIcon>
#include <QtPlugin>

/**
 * Точка входа
 * @param argc Кол-во аргументов
 * @param argv Аргументы
 * @return Код выхода
 */
int main(int argc, char* argv[])
{

    // Если QT линкуется статически, нужно статически импортировать плагин
#ifdef QT_STATIC_BUILD
    Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
#endif

    // Инициализация QT
    QApplication app(argc, argv);

    // Добавить шрифт из ресурсов
    QFontDatabase::addApplicationFont(":/fonts/Lato-Light.ttf");

    // Основное окно
    CalculatorWindow calculatorWindow;
    calculatorWindow.setWindowIcon(QIcon(":/icons/icon.png"));
    calculatorWindow.show();

    // Исполнение приложения
    return QApplication::exec();
}