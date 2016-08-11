#include "logic.h"
#include <QDebug>
#include <QMessageBox>
#include <QThread>
#include <QTimer>

Logic::Logic(QObject *parent) : QObject(parent) {

  srand(time(0));
  for (unsigned short i = 0; i < 16; ++i) {
    m_list << QString::number(i);
  }
  refresh();
}

int Logic::moveCounter() const {
  return m_moveCounter;
}

void Logic::refresh() {
  auto size = m_list.size();
  QVector<int> arr = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
  auto getIdentifier = [&arr]() {
                            int value = arr[std::rand() % arr.size()];
                            arr.removeOne(value);
                            return QString::number(value);
                          };

  for(unsigned short i = 0; i < size; ++i) {
    m_list[i] = getIdentifier();
  }
  if (countInvertion() % 2 != 0) {
    m_list.move(size - 2, size - 1);
  }
  emit listChanged(m_list);
}

void Logic::move(int currentIndex) {
  Q_UNUSED(currentIndex)
}

bool Logic::checkWin() const {
  return false;
}

QString Logic::identifier(int index) {
  return m_list[index];
}

int Logic::countInvertion() const {
  auto size = m_list.size();
  int counter = 0;
  int row = 0;

  for (int i = 0, j = 1; i < size - 1; ++i, ++j) {
    if (m_list[i] > m_list[j]) {
      ++counter;
    }
    if (m_list[j] == "0") {
      row = static_cast<int>(j / 4);
    }
  }
  return counter + row;
}

bool Logic::checkOutOfRange(int value) {
  Q_UNUSED(value)
  return false;
}

bool Logic::checkBorder(int value, bool right) {
  Q_UNUSED(value)
  Q_UNUSED(right)
  return true;
}

QObject *Logic::singletone_provider(QQmlEngine *engine, QJSEngine *scriptEngine) {
  Q_UNUSED(engine)
  Q_UNUSED(scriptEngine)

  return new Logic();
}

QStringList Logic::list() const
{
  return m_list;
}

void Logic::setmoveCounter(int moveCounter) {
  if (m_moveCounter == moveCounter) {
    return;
  }

  m_moveCounter = moveCounter;
  emit moveCounterChanged(moveCounter);
}

void Logic::setList(QStringList list)
{
  if (m_list == list)
    return;

  m_list = list;
  emit listChanged(list);
}
