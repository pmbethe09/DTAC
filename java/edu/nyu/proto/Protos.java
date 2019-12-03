package edu.nyu.proto;

import com.google.protobuf.Message;
import com.google.protobuf.TextFormat;
import java.io.*;

/** General utilites for protos. */
public final class Protos {
  private Protos() {}

  /** Returns a protobuf parsed from the given file. */
  public static <P extends Message> P loadFromFile(File file, P example) throws IOException {
    return parseText(new BufferedReader(new FileReader(file)), example);
  }

  /** Returns a protobuf parsed from the given reader. */
  @SuppressWarnings("unchecked") /* OK - as example used to get builder. */
  public static <P extends Message> P parseText(Reader reader, P example) throws IOException {
    Message.Builder builder = example.newBuilderForType();
    TextFormat.merge(reader, builder);
    return (P) builder.build();
  }

  /**
   * Returns a protobuf parsed from the given String using the example (usually
   * P.getDefaultInstance()).
   */
  public static <P extends Message> P parseText(String input, P example) throws IOException {
    return parseText(new StringReader(input), example);
  }

  /**
   * Returns a protobuf parsed from the given String using the example (usually
   * P.getDefaultInstance()).
   */
  public static <P extends Message> P parseUnchecked(String input, P example) {
    try {
      return parseText(new StringReader(input), example);
    } catch (IOException e) {
      throw new IllegalArgumentException(String.format("unable to parse input '%s'", input), e);
    }
  }
}
