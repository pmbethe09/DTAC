package edu.nyu.proto;

import java.io.*;
import java.nio.file.Files;

import javax.annotation.Nullable;

import com.google.protobuf.Message;
import com.google.protobuf.TextFormat;

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

  /** Returns a protobuf parsed from the given String using the example (usually
   * P.getDefaultInstance()). */
  public static <P extends Message> P parseText(String input, P example) throws IOException {
    return parseText(new StringReader(input), example);
  }
}
