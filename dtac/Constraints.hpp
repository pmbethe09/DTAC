#ifndef _BRIDGE_Constraints_INCLUDED
#define _BRIDGE_Constraints_INCLUDED

/// for constraining hand samples

namespace dtac {

class Hand;

// generic concept of a constraint, for now testing
class IConstraint {
 public:
  virtual bool holds(const Hand& h) const = 0;
};

class Balanced : public virtual IConstraint {
 public:
  virtual bool holds(const Hand& h) const;
};

class Unbalanced : public Balanced {
 public:
  virtual bool holds(const Hand& h) const;
};

class AnyShape : public virtual IConstraint {
 public:
  virtual bool holds(const Hand& h) const { return true; }
};

class PointRange : public virtual IConstraint {
 public:
  int low, high;
  PointRange(int low, int high);
  virtual bool holds(const Hand& h) const;
};

}  // namespace dtac

#endif /* hpp */
