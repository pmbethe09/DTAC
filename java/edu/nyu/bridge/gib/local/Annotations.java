package edu.nyu.bridge.gib.local;

import com.google.inject.BindingAnnotation;

import java.lang.annotation.Retention;

import static java.lang.annotation.RetentionPolicy.RUNTIME;

public final class Annotations {
  @BindingAnnotation
  @Retention(value = RUNTIME)
  public static @interface Gib {}
}