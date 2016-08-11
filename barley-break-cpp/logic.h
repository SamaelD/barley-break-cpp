#ifndef LOGIC_H
#define LOGIC_H

#include <QObject>

class QQmlEngine;
class QJSEngine;
class QStandardItemModel;

class Logic;

class LogicDestroyer {
public:
  LogicDestroyer() :pLogic(nullptr) {}
  ~LogicDestroyer() { delete pLogic; }
  void setInstance(Logic *pointer) { pLogic = pointer; }

private:
  Logic *pLogic;
};

class Logic : public QObject {
  Q_OBJECT

  Q_PROPERTY(QStringList list READ list WRITE setList NOTIFY listChanged)
  Q_PROPERTY(int moveCounter READ moveCounter WRITE setmoveCounter NOTIFY moveCounterChanged)

  explicit Logic(QObject *parent = nullptr);
  Logic(const Logic &) = default;
  Logic(Logic &&) = default;
  ~Logic() = default;

public:
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
  void Victory();

public slots:
  void setmoveCounter(int moveCounter);
  void setList(QStringList list);

private slots:
  void movement();

private:
  QStringList m_list;
  int m_moveCounter {0};

  static LogicDestroyer m_destroyer;
  static Logic *pInst;
};


#endif // LOGIC_H
