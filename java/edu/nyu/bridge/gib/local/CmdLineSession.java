package edu.nyu.bridge.gib.local;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import javax.inject.Inject;

/** GiB session which shells out to Gib.exe. */
public class CmdLineSession implements LocalGibPlayer.Session {
  private final Process proc;

  @Inject
  CmdLineSession(@Annotations.Gib String gibPath) {
    try {
      proc = new ProcessBuilder(gibPath).start();
    } catch (IOException e) {
      throw new IllegalArgumentException(e);
    }
  }

  @Override
  public void close() throws IOException {
    proc.destroy();
  }

  @Override
  public InputStream getInputStream() {
    return proc.getInputStream();
  }

  @Override
  public OutputStream getOutputStream() {
    return proc.getOutputStream();
  }
}
