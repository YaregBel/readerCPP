#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QFontMetrics>
#include <QTextEdit>

#include <QPushButton>
#include <QVBoxLayout>
#include <QStringList>



QT_BEGIN_NAMESPACE
class QLabel;
class QAction;
class QActionGroup;
class QMenu;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

#ifndef QT_NO_CONTEXTMENU
    void contextMenuEvent(QContextMenuEvent *event) override;
#endif  QT_NO_CONTEXTMENU

private slots:
    void newFile();
    void open();
    void save();
    void print();
    void undo();
    void redo();
    void cut();
    void copy();
    void paste();
    void bold();
    void italic();
    void leftAlign();
    void rightAlign();
    void justify();
    void center();
    void setLineSpacing();
    void setParagraphSpacing();
    void about();
    void aboutQt();

    // void getTextArea();
    // void showPage(int index);

    void nextPage();
    void prevPage();

protected:
    void resizeEvent(QResizeEvent *event) override;


private:
    void createActions();
    void createMenus();

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *formatMenu;
    QMenu *helpMenu;
    QActionGroup *alignmentGroup;
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *printAct;
    QAction *exitAct;
    QAction *undoAct;
    QAction *redoAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *boldAct;
    QAction *italicAct;
    QAction *leftAlignAct;
    QAction *rightAlignAct;
    QAction *justifyAct;
    QAction *centerAct;
    QAction *setLineSpacingAct;
    QAction *setParagraphSpacingAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QLabel *infoLabel;


    QLabel *label;
    QPushButton *nextButton;
    QPushButton *prevButton;
    QString text;
    QStringList pages;
    int currentPage;

    // Разбиваем текст на страницы
    void paginateText();
    // Обновляем текст на текущей странице
    void updatePageDisplay();

};
#endif // MAINWINDOW_H
