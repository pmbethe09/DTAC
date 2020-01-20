package edu.nyu.cards;

import com.google.auto.value.AutoValue;
import com.google.common.collect.ImmutableSet;
import com.google.errorprone.annotations.Immutable;
import edu.nyu.cards.gen.Cards;

import java.util.Iterator;

import static com.google.common.collect.ImmutableSet.toImmutableSet;

@AutoValue
@AutoValue.CopyAnnotations
@Immutable
@SuppressWarnings("Immutable") // we enforce only read operations on hand
public abstract class HandView implements Iterable<Cards.Card> {
  HandView() {}

  // Internal mutable hand
  abstract Hand hand();

  public static HandView of(Hand hand) {
    return new AutoValue_HandView(hand);
  }

  @Override
  public Iterator<Cards.Card> iterator() {
    return hand().asList().iterator();
  }

  public int size(Cards.Suit suit) {
    return hand().size(suit);
  }

  public ImmutableSet<Cards.Card> suit(Cards.Suit suit) {
    return hand().suit(suit).stream()
            .map(r -> edu.nyu.cards.Cards.card(suit, r))
            .collect(toImmutableSet());
  }

  public boolean hasCard(Cards.Card card) {
    return hand().hasCard(card);
  }
}
