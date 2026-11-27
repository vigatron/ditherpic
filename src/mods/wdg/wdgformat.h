#ifndef WDGFORMAT_H
#define WDGFORMAT_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QSize>

class WdgFormat : public QWidget
{
    Q_OBJECT

public:
    explicit WdgFormat(QWidget *parent = nullptr);

protected:
    void showEvent(QShowEvent *event) override;

private:
    void initFormats();
    void initButtons();
    void selectFormat(int index);

private slots:
    void onFormatSelected(int index);
    void onCloseClicked();

private:
    QListWidget *m_listWidget;
    QPushButton *m_selectButton;
    QPushButton *m_closeButton;
    QList<QPair<QSize, QString>> m_formats;
};

#endif // WDGFORMAT_H
