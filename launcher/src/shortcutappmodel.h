#ifndef SHORTCUTAPPMODEL_H
#define SHORTCUTAPPMODEL_H

#include <QtCore/QAbstractListModel>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QFile>
#include <QProcess>
#include <QThread>

struct RegisterApp {
    QString id;
    QString name;
    QString icon;
    bool isBlank;
};

class ShortcutAppModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ShortcutAppModel(QObject *parent = nullptr);
    ~ShortcutAppModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE QString getId(int index) const;
    Q_INVOKABLE QString getName(int index) const;
    Q_INVOKABLE QString getIcon(int index) const;
    Q_INVOKABLE bool isBlank(int index) const;

public slots:
    void changeShortcut(QString id, QString name, QString position);

signals:
    void updateShortcut();

private:
    void getAppQueue();
//    bool confirmIconImage(QString id);
    QString getIconPath(QString id);

    class Private;
    Private *d;
    RegisterApp app;

};

#endif // SHORTCUTAPPMODEL_H
