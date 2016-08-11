#ifndef LOGIC_H
#define LOGIC_H

#include <QObject>
#include <QQmlEngine>
#include <QJSEngine>
#include <QStandardItemModel>

class Logic : public QObject {
  Q_OBJECT

  Q_PROPERTY(QStringList list READ list WRITE setList NOTIFY listChanged)
  Q_PROPERTY(int moveCounter READ moveCounter WRITE setmoveCounter NOTIFY moveCounterChanged)
public:
  explicit Logic(QObject *parent = nullptr);

  int moveCounter() const;

  Q_INVOKABLE void refresh();
  Q_INVOKABLE void move(int currentIndex);
  Q_INVOKABLE bool checkWin() const;
  Q_INVOKABLE QString identifier(int index) const;

  QStringList list() const;
  int countInvertion() const;

  bool checkOutOfRange(int value);
  bool checkBorder(int value, bool right);

  static QObject *singletone_provider(QQmlEngine *engine, QJSEngine *scriptEngine);

signals:
  void moveCounterChanged(int moveCounter);
  void listChanged(QStringList list);

public slots:
  void setmoveCounter(int moveCounter);
  void setList(QStringList list);

private:
  QStringList m_list;
  int m_moveCounter {0};

};

#endif // LOGIC_H
