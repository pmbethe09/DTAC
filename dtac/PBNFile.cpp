/*
 *   Copyright (C) 2009 by Paul Bethe   *
 *   pmb309@cs.nyu.edu   *
 *
 * PBN File loader
 * class
 ***************************************************************************/

#include "dtac/PBNFile.hpp"

#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <stdexcept>

#include "dtac/PBNTags.hpp"

using std::unique_ptr;
using std::pair;
using std::make_pair;
using std::endl;
using std::ifstream;
using std::istream;
using std::map;
using std::ostream;
using std::istringstream;
using std::string;
using std::stringstream;
using std::vector;

static const char *tags_inorder[] = {
    Event, Site,    Date,     Round,     Section, Board,
    West,  North,   East,     South,     Dealer,  Vulnerable,
    Deal,  Scoring, Declarer, Contract,  Result,  HomeTeam,
    Room,  Score,   Stage,    VisitTeam, Auction, Play};
const int tags_inorder_size = sizeof(tags_inorder) / sizeof(char *);

static map<string, int> map2Class;

static void init() {
  static int first_pass = 1;
  if (first_pass) {
    for (int i = 0; i < tags_inorder_size; ++i) {
      map2Class.insert(
          map<string, int>::value_type(string(tags_inorder[i]), i));
    }
    first_pass = 0;
  }
}

typedef enum _ptoken {
  LBRACK,
  RBRACK,
  COMMENT,
  EMPTYLINE,
  COMMENTARY, /* { inside these } */
  TAG /*alpha and _*/,
  ANNOTE /*e.g. =1= */,
  VALUE, /* found in quotes " " */
  STAR,
  PEOF,
  ERROR
} PTOKEN;

// tokenizer for PBN format
class Lexer {
 public:
  explicit Lexer(istream &inp)
      : tok_type(STAR), c('*'), lineno(0), fullStream(inp) {}

  PTOKEN tok_type;
  stringstream buf;
  char c;
  int lineno;
  istream &fullStream;
  istringstream this_line;

  // read next line into this_line
  void readline() {
    string s;
    getline(fullStream, s);
    this_line.str(s);
    this_line.clear();
    ++lineno;
    // and get first look_ahead
    c = read();
  }

  // read the next char of this line
  char read() {
    if (!this_line.get(c)) c = '\n';
    return c;
  }
  void plexer();
};

// return and lookahead
#define RETURN_LA(tok) \
  tok_type = tok;      \
  inp.get(c);          \
  return;

void Lexer::plexer() {
  istream &inp = this_line;

  string dummy;

  // lookahead
  if (c == '*')  // init
    inp.get(c);

  while (true) {
    if (!inp) readline();
    if (!fullStream || c == EOF) {
      tok_type = PEOF;
      return;
    }

    switch (c) {
      case '{':  // TODO save, for now, burn comments
        buf.str("");
        buf.clear();  // reset
        inp.get(c);   // burn the { read next
        while (c != '}') {
          if (inp) {
            buf << c;
            inp.get(c);
          } else {
            buf << "\n";
            readline();
          }
        }
        RETURN_LA(COMMENTARY);
      case '[':
        RETURN_LA(LBRACK);
      case ']':
        RETURN_LA(RBRACK);
      case '"': {
        buf.str("");
        buf.clear();  // reset
        inp.get(c);   // burn the quote read next
        while (c != '"' && inp) {
          // NOTE: if \" is allowed, we don't support it
          // also should we detect bad?
          buf << c;
          inp.get(c);
        }
        RETURN_LA(VALUE);
      }
      case '%':  // comment till newline
        RETURN_LA(COMMENT);
      //      getline(inp, dummy); ++this->lineno; break;
      case '=':
        buf.str("");  // reset
        inp.get(c);   // burn '='
        while (isdigit(c)) {
          buf << c;
          inp.get(c);
        }
        if (c != '=') {
          tok_type = ERROR;
          return;
          // msg ? recovery?
        }
        RETURN_LA(ANNOTE);
      case '*':
        RETURN_LA(STAR);
      case '\n':
        // since 'getline' normally eliminates, except when we detect a blank
        // line
        RETURN_LA(EMPTYLINE);  // newline after newline is empty!
      //++this->lineno; break;
      default: {
        // valid chars for a tag [a-zA-Z0-9_]
        if (isalnum(c) || (c == '_') || (c == '-')) {
          buf.str("");  // reset
          tok_type = TAG;
          while (inp && (isalnum(c) || (c == '_') || (c == '-'))) {
            buf << c;
            inp.get(c);
          }
          return;
        }
        // else ignore

      }  // end default
    }    // end case

    inp.get(c);  // read ahead the next character
  }              // while loop

  // while has returns for all cases so won't get here
}

typedef enum _pstate { START, INPAIR } PSTATE;

PBNFile *PBNFile::loadFromFile(const string &file_name) {
  init();

  unique_ptr<PBNDeal> result;
  unique_ptr<PBNFile> fileData(new PBNFile(file_name));

  PSTATE state = START;
  ifstream ifs(file_name.c_str());
  Lexer lex(ifs);
  PTOKEN &tok_type = lex.tok_type;
  stringstream &buf = lex.buf;

  string lasttag;

  bool afterAuction = false, afterPlay = false;

  lex.readline();  // read first line

  while (tok_type != PEOF) {
    lex.plexer();

    switch (state) {
      case START: {
        switch (tok_type) {
          case COMMENTARY:
            result->commentary.push_back(buf.str());
            continue;
          case LBRACK:
            state = INPAIR;
            continue;
          case TAG:  // assume a bid or play
            // cerr << buf.str() << endl;
            if (afterPlay)
              result->plays.push_back(buf.str());
            else if (afterAuction)
              result->bids.push_back(buf.str());
            else {
              throw std::logic_error(
                  string("free text for non Auction or Play: ") + buf.str());
            }
            continue;
          case ANNOTE: {
            string s(buf.str());
            // cerr << s << endl;

            // should we verify that this is the right #?
            // int num;
            // buf >> num;
            // line up with index of current bid
            // ASSERT num == result->bid_explanation.length()
            if (afterPlay)
              result->play_explanation.push_back(
                  make_pair(result->plays.size() - 1, ""));
            else if (afterAuction)
              result->bid_explanation.push_back(
                  make_pair(result->bids.size() - 1, ""));
            else {
              throw std::logic_error(
                  string("free text for non Auction or Play: "));
            }
            continue;
          }
          case COMMENT:
            lex.readline();
            continue;
          case STAR:       // star and
          case EMPTYLINE:  // empty line delimits deals
            // EO Deal - reset
            if (result) {
              fileData->data.push_back(result.release());
              afterAuction = false;
              afterPlay = false;
              lex.readline();
            }
            continue;
          case PEOF:
            return fileData.release();
          default:
            throw std::logic_error(string("Unexpected token type:"));
        }  // end switch tok_type
        break;
      }  // end case START
      case INPAIR: {
        switch (tok_type) {
          case RBRACK:
            state = START;
            continue;
          case TAG:
            lasttag = buf.str();
            continue;
          case VALUE: {
            // put into the right slot

            if (lasttag == "Note") {  // note on last guy
              int idx;
              char dummy;
              string info;
              buf >> idx >> dummy >> info;  // format "N:abc"
              if (afterPlay)                // ensure size?
                result->play_explanation[idx - 1].second = info;
              else if (afterAuction)  // ensure size?
                result->bid_explanation[idx - 1].second = info;
              else
                throw std::logic_error(
                    string("Unexpected Note - no auction/play info") + info);
              buf.clear();
              continue;
            }
            // else

            if (!result) result.reset(new PBNDeal);

            map<string, int>::const_iterator it = map2Class.find(lasttag);
            if (it == map2Class.end()) {
              // assume a valid optional tag - no validation yet
              result->optional_tags.push_back(make_pair(lasttag, buf.str()));
              continue;
            }

            int idx = it->second;
            PBNDeal *deal = &(*result);
            const int mult = sizeof(string) / sizeof(char);
            string &slot =
                *((string *)((reinterpret_cast<char *>(deal)) + idx * mult));
            slot.operator=(buf.str());

            // cerr << lasttag << " <- " << slot << endl;

            if (idx == PBNTags::AUCTION)  // lasttag == "Auction")
              afterAuction = true;
            else if (idx == PBNTags::PLAY)  // lasttag == "Play")
              afterPlay = true;

            continue;
          }
          default:
            throw std::logic_error(string("Unexpected token"));
        }  // end switch tok_type
        break;
      }  // end case INPAIR
    }    // end switch

  }  // end while ! EOF

  return fileData.release();
}

static inline void writeExpl(ostream &os, const vector<string> &bids,
                             const vector<pair<int, string> > &expl) {
  vector<pair<int, string> >::const_iterator pnote_it = expl.begin();
  for (int i = 0; i < bids.size(); ++i) {
    if (i & 3)  // non 4 based (so not sol
      os << " ";
    os << bids[i];
    if (pnote_it != expl.end() && pnote_it->first == i) {
      os << " =" << (1 + std::distance(expl.begin(), pnote_it)) << "=";
      ++pnote_it;
    }
    if ((i & 3) == 3) os << endl;
  }
  if ((bids.size() & 3))  // non size 4 based, need extra newl
    os << endl;

  for (int i = 0; i < expl.size(); ++i) {
    os << "[Note \"" << (1 + i) << ":" << expl[i].second << "\"]" << endl;
  }
}

static inline void writeTag(ostream &os, const char *tag, const string &value) {
  os << "[" << tag << " ";
  os << "\"" << value << "\"]" << endl;
}

PBNFile::~PBNFile() {
  for (vector<PBNDeal *>::iterator it = data.begin(); it != data.end(); ++it) {
    if (*it != NULL) {
      delete *it;
    }
  }
}

// this is in here, to get access to the enums
void PBNDeal::printDeal(ostream &os) const {
  const int mult = sizeof(string) / sizeof(char);
  for (int i = 0, size = (int)PBNTags::VISITTEAM; i <= size; ++i) {
    // this assumes no vtable at the front
    const string &slot =
        *((const string *)((reinterpret_cast<const char *>(this)) + i * mult));
    writeTag(os, tags_inorder[i], slot);
  }
  // OK, written up to (not incl) Auction
  // do optional tags
  for (int i = 0; i < optional_tags.size(); ++i) {
    const pair<string, string> &p = optional_tags[i];
    writeTag(os, p.first.c_str(), p.second);
  }

  // do comments
  for (int i = 0; i < commentary.size(); ++i) {
    os << "{" << endl << commentary[i] << "}" << endl;
  }

  // do Auction
  writeTag(os, Auction, auction);
  writeExpl(os, bids, bid_explanation);

  // do Play
  writeTag(os, Play, play);
  writeExpl(os, plays, play_explanation);
  os << "*" << endl;
}
