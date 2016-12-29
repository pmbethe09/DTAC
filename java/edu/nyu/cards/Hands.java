package edu.nyu.cards;

import com.google.common.base.Converter;
import edu.nyu.cards.gen.Cards;

/** Utilities for {@link Hand} objects. */
public class Hands {
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
}
