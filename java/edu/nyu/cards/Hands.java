package edu.nyu.cards;

import java.util.List;

import com.google.common.base.Converter;
import edu.nyu.cards.gen.Cards;

/** Utilities for {@link Hand} objects. */
public class Hands {
  private Hands() {}

  public static final Converter<Cards.Hand, String> HAND_TO_STRING =
      new Converter<Cards.Hand, String>() {
        @Override
        protected Cards.Hand doBackward(String hand) {
          return Hand.fromString(hand).toProto();
        }

        @Override
        protected String doForward(Cards.Hand arg0) {
          return Hand.fromProto(arg0).toString();
        }
      };

  public static final Converter<Hand, Cards.Hand> HAND_TO_PROTO =
      new Converter<Hand, Cards.Hand>() {
        @Override
        protected Hand doBackward(Cards.Hand arg0) {
          return Hand.fromProto(arg0);
        }

        @Override
        protected Cards.Hand doForward(Hand arg0) {
          return arg0.toProto();
        }
      };

  public static final Converter<Cards.Hand, List<Cards.Card>> HAND_TO_LIST =
      new Converter<Cards.Hand, List<Cards.Card>>() {
        @Override
        protected Cards.Hand doBackward(List<Cards.Card> hand) {
          return Cards.Hand.newBuilder().addAllCards(hand).build();
        }
        @Override
        protected List<Cards.Card> doForward(Cards.Hand hand) {
          return hand.getCardsList();
        }
      };
}
