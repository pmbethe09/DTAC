#ifndef PBNTAGS_GEN_HPP
#define PBNTAGS_GEN_HPP

struct PBNTags {
  enum TAGS {
    EVENT = 0,
    SITE,
    DATE,
    ROUND,
    SECTION,
    BOARD,
    WEST,
    NORTH,
    EAST,
    SOUTH,
    DEALER,
    VULNERABLE,
    DEAL,
    SCORING,
    DECLARER,
    CONTRACT,
    RESULT,
    HOMETEAM,
    ROOM,
    SCORE,
    STAGE,
    VISITTEAM,
    AUCTION,
    PLAY
  };
};

#define Event "Event"
#define Site "Site"
#define Date "Date"
#define Round "Round"
#define Section "Section"
#define Board "Board"
#define West "West"
#define North "North"
#define East "East"
#define South "South"
#define Dealer "Dealer"
#define Vulnerable "Vulnerable"
#define Deal "Deal"
#define Scoring "Scoring"
#define Declarer "Declarer"
#define Contract "Contract"
#define Result "Result"
#define Auction "Auction"
#define HomeTeam "HomeTeam"
#define Room "Room"
#define Score "Score"
#define Stage "Stage"
#define VisitTeam "VisitTeam"
#define Play "Play"

#endif /* HPP */
