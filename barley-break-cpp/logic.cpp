#include <QVector>

#include "logic.h"

LogicDestroyer Logic::m_destroyer;
Logic *Logic::pInst = nullptr;

Logic::Logic(QObject *parent) : QObject(parent) {

  for (unsigned short i = 1; i < 16; ++i) {
    m_list << QString::number(i);
  }
  m_list << QString::number(0);
  connect(this, &Logic::listChanged, this, &Logic::movement);
  //refresh();
}

int Logic::moveCounter() const {
  return m_moveCounter;
}

void Logic::refresh() {
  auto size = m_list.size();
  QVector<int> arr = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
  auto getIdentifier = [&arr]() {
    int value = arr[qrand() % arr.size()];
    arr.removeOne(value);
    return QString::number(value);
  };

  for (unsigned short i = 0; i < size; ++i) {
    m_list[i] = getIdentifier();
  }
  if (countInvertion() % 2 != 0) {
    m_list.move(size - 2, size - 1);
  }

  emit listChanged(m_list);
  setmoveCounter(0);
}

void Logic::move(int currentIndex) {
  if (checkOutOfRange(currentIndex + 1)
      && checkBorder(currentIndex, true)
      && m_list[currentIndex + 1] == "0") {
    m_list.move(currentIndex, currentIndex + 1); listChanged(m_list);
  }
  else if (checkOutOfRange(currentIndex - 1)
           && checkBorder(currentIndex, false)
           && m_list[currentIndex - 1] == "0") {
    m_list.move(currentIndex, currentIndex - 1); listChanged(m_list);
  }
  else if (checkOutOfRange(currentIndex + 4)
           && m_list[currentIndex + 4] == "0") {
    m_list.move(currentIndex, currentIndex + 4);
    m_list.move(currentIndex + 3, currentIndex); listChanged(m_list);
  }
  else if (checkOutOfRange(currentIndex - 4)
           && m_list[currentIndex - 4] == "0") {
    m_list.move(currentIndex, currentIndex - 4);
    m_list.move(currentIndex - 3, currentIndex); listChanged(m_list);
  }

  if (checkWin()) {
    emit Victory();
  }
}

bool Logic::checkWin() const {
  auto size = m_list.size();
  int i = 1;
  for (auto it : m_list) {
    if (i < size && it != QString::number(i++)) {
      return false;
    }
  }
  return true;
}

QString Logic::identifier(int index) const {
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
      row = j / 4;
    }
  }
  return counter + row;
}

bool Logic::checkOutOfRange(int value) {

  if (value < 0 || value >= m_list.size()) {
    return false;
  }
  return true;
}

bool Logic::checkBorder(int value, bool right) {
  auto size = m_list.size();
  for (int i = (right ? 3 : 0); i < size; i += 4) {
    if (value == i) {
      return false;
    }
  }
  return true;
}

QObject *Logic::singletone_provider(QQmlEngine *engine, QJSEngine *scriptEngine) {
  Q_UNUSED(engine)
  Q_UNUSED(scriptEngine)

  if (!pInst) {
    pInst = new Logic();
    m_destroyer.setInstance(pInst);
  }
  return pInst;
}

QStringList Logic::list() const {
  return m_list;
}

void Logic::setmoveCounter(int moveCounter) {
  if (m_moveCounter == moveCounter) {
    return;
  }

  m_moveCounter = moveCounter;
  emit moveCounterChanged(moveCounter);
}

void Logic::setList(QStringList list) {
  if (m_list == list)
    return;

  m_list = list;
  emit listChanged(list);
}

void Logic::movement() {
  setmoveCounter(m_moveCounter + 1);
}
