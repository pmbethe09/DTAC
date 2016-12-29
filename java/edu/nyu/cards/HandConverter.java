package edu.nyu.cards;

import com.google.common.base.Converter;

import edu.nyu.cards.gen.Cards;

public class HandConverter extends Converter<Hand, Cards.Hand> {
  @Override
  protected Hand doBackward(Cards.Hand arg0) {
    return Hand.fromProto(arg0);
  }

  @Override
  protected Cards.Hand doForward(Hand arg0) {
    return arg0.toProto();
  }
}
