#ifndef _BRIDGE_Constants_INCLUDED
#define _BRIDGE_Constants_INCLUDED

#define NUM_CARDS 13

namespace dtac {

// ranks as powers of 2
typedef enum _ranks {
  /*TWO=1, THREE, FOUR,
   FIVE, SIX,
   SEVEN, EIGHT,
   NINE, TEN, JACK,
   QUEEN, KING, ACE, NUM_CARDS
   */
  TWO = 1,
  THREE = 2,
  FOUR = 4,
  FIVE = 8,
  SIX = 16,
  SEVEN = 32,
  EIGHT = 64,
  NINE = 128,
  TEN = 256,
  JACK = 512,
  QUEEN = 1024,
  KING = 2048,
  ACE = 4096
} RANK;

static inline RANK operator+(RANK r, int i) {
  return (RANK)(((int)r) << i); /* shift bit left one */
}
static inline RANK operator-(RANK r, int i) {
  return (RANK)(((int)r) >> i); /* shift bit left one */
}
static inline RANK operator--(RANK& r) {
  return r = r - 1; /* shift bit right one */
}
static inline RANK operator++(RANK& r) {
  return r = r + 1; /* shift bit left one */
}

static inline int rank2Int(RANK r) {
  switch (r) {
    case TWO:
      return 0;
    case THREE:
      return 1;
    case FOUR:
      return 2;
    case FIVE:
      return 3;
    case SIX:
      return 4;
    case SEVEN:
      return 5;
    case EIGHT:
      return 6;
    case NINE:
      return 7;
    case TEN:
      return 8;
    case JACK:
      return 9;
    case QUEEN:
      return 10;
    case KING:
      return 11;
    case ACE:
      return 12;
  }
  return -1;
}

static inline char rank2Char(RANK r) {
  static const char* lookup = "23456789TJQKA";
  return lookup[rank2Int(r)];
}

static inline bool operator<(RANK l, RANK r) { return (int)l < (int)r; }
static inline bool operator<=(RANK l, RANK r) { return (int)l <= (int)r; }

typedef enum _suits {
  CLUBS = 0,
  DIAMONDS = 1,
  HEARTS = 2,
  SPADES = 3,
  NOTRUMP = 4,
  NUM_SUITS = 4
} SUIT;

static inline SUIT operator--(SUIT& s) { return s = (SUIT)(((int)s) - 1); }
static inline SUIT operator++(SUIT& s) { return s = (SUIT)(((int)s) + 1); }

/* keep in line with suit enum */
static inline char suit2Char(SUIT s) {
  static const char* lookup = "CDHSN";
  return lookup[(int)s];
}

typedef enum _dirs {
  NORTH = 0,
  EAST = 1,
  SOUTH = 2,
  WEST = 3,
  NUM_DIRECTIONS = 4
} DIRECTION;
static inline DIRECTION operator+(const DIRECTION& s, int i) {
  return (DIRECTION)((((int)s) + i) & 3);
}
static inline DIRECTION operator++(const DIRECTION& s) { return s + 1; }
static inline DIRECTION operator-(const DIRECTION& val, const DIRECTION& base) {
  return (DIRECTION)((((int)val) - ((int)base) + 4) & 3);
}

static inline bool isNS(DIRECTION d) { return !(d & 1); /* 0 or 2 */ }

}  // namespace dtac

#endif  // hpp
