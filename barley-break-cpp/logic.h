#ifndef LOGIC_H
#define LOGIC_H

#include <QObject>

class QQmlEngine;
class QJSEngine;
class QStandardItemModel;

class Logic : public QObject {
  Q_OBJECT

  Q_PROPERTY(QStringList list READ list WRITE setList NOTIFY listChanged)
  Q_PROPERTY(int moveCounter READ moveCounter WRITE setmoveCounter NOTIFY moveCounterChanged)
  Q_PROPERTY(int BestScore READ BestScore WRITE setBestScore NOTIFY BestScoreChanged)
  Q_PROPERTY(bool victory READ victory NOTIFY victoryChanged)

  explicit Logic(QObject *parent = nullptr);

public:
  Logic(const Logic &) = delete;
  Logic(Logic &&) = delete;
  ~Logic();
  int moveCounter() const;

  Q_INVOKABLE void refresh();
  Q_INVOKABLE void move(int currentIndex);
  Q_INVOKABLE bool checkWin() const;
  Q_INVOKABLE QString identifier(int index) const;

  QStringList list() const;
  int countInvertion() const;
  int BestScore() const;
  bool victory() const;

  bool checkOutOfRange(int value);
  bool checkBorder(int value, bool right);

  void loadScores();
  Q_INVOKABLE void saveScores();

  static QObject *singletone_provider(QQmlEngine *engine, QJSEngine *scriptEngine);



signals:
  void moveCounterChanged(int moveCounter);
  void listChanged(QStringList list);
  void BestScoreChanged(int BestScore);
  void victoryChanged(bool victory);


public slots:
  void setmoveCounter(int moveCounter);
  void setList(QStringList list);
  void setBestScore(int BestScore);


private slots:
  void movement();

private:
  QStringList m_list;
  int m_moveCounter {0};
  static Logic *pInst;
  int m_BestScore;
  bool m_victory { false };

};


#endif // LOGIC_H
