package edu.nyu.cards;

import java.util.List;

import com.google.common.base.Converter;
import edu.nyu.cards.gen.Cards;

/** Utilities for {@link Hand} objects. */
public final class Hands {
  private Hands() {}

  public static final Converter<Cards.Hand, String> HAND_TO_STRING =
      Converter.from(h -> Hand.fromProto(h).toString(), h -> Hand.fromString(h).toProto());

  public static final Converter<Hand, Cards.Hand> HAND_TO_PROTO =
      Converter.from(Hand::toProto, Hand::fromProto);

  public static final Converter<Cards.Hand, List<Cards.Card>> HAND_TO_LIST =
      Converter.from(Cards.Hand::getCardsList, c -> Cards.Hand.newBuilder().addAllCards(c).build());
}
