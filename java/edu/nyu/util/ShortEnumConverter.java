package edu.nyu.util;

import static com.google.common.base.Preconditions.checkArgument;
import static com.google.common.base.Preconditions.checkNotNull;

import java.util.EnumSet;
import java.util.HashSet;
import java.util.Set;

import com.google.common.base.Converter;
import com.google.common.collect.ImmutableBiMap;

/**
 * Learn to convert enums using shortened versions as strings.
 *
 * @author pbethe
 */
public class ShortEnumConverter<T extends Enum<T>> extends Converter<String, T> {
  private final ImmutableBiMap<String, T> biData;

  private ShortEnumConverter(Class<T> clazz) {
    EnumSet<T> allOf = EnumSet.allOf(clazz);
    ImmutableBiMap.Builder<String, T> builder = ImmutableBiMap.builder();
    Set<String> seen = new HashSet<>();
    for (T e : allOf) {
      String oneChar = oneCharFromEnum(e);
      checkArgument(!seen.contains(oneChar),
          "Ambiguous starting characters, unable to construct 1-char map for: %s", clazz.getName());
      builder.put(oneChar, e);
      seen.add(oneChar);
    }
    this.biData = builder.build();
  }

  private static final String oneCharFromEnum(Enum<?> value) {
    return value.name().substring(0, 1).toUpperCase();
  }

  @Override
  public String doBackward(T arg0) {
    return checkNotNull(biData.inverse().get(arg0), "%s not back convertable", arg0);
  }

  @Override
  public T doForward(String arg0) {
    return checkNotNull(
        biData.get(arg0.substring(0, 1).toUpperCase()), "%s not forward conv", arg0);
  }

  public static <T extends Enum<T>> ShortEnumConverter<T> create(Class<T> clazz) {
    return new ShortEnumConverter<T>(clazz);
  }
}
